#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "esp_log.h"
#include "temperature_sensor.h"
#include "wifi_ap.h"
#include <string.h>
#include <lwip/sockets.h>
#include <errno.h>
#include "nvs_flash.h"

static const char *TAG = "main";

// Circular buffer configuration
#define BUFFER_SIZE 512
#define SAMPLE_INTERVAL_MS 10
#define TRANSMISSION_INTERVAL_MS 5200

// Circular buffer structure
typedef struct {
    float data[BUFFER_SIZE];
    volatile uint16_t head;
    volatile uint16_t tail;
    volatile uint16_t count;
    SemaphoreHandle_t mutex;
} CircularBuffer;

// Global variables
static temperature_sensor_handle_t tsens;
static CircularBuffer temp_buffer;
static TaskHandle_t temp_task_handle = NULL;
static TaskHandle_t wifi_task_handle = NULL;

// Circular buffer functions
void circular_buffer_init(CircularBuffer *cb) {
    memset(cb->data, 0, sizeof(cb->data));
    cb->head = 0;
    cb->tail = 0;
    cb->count = 0;
    cb->mutex = xSemaphoreCreateMutex();
}

bool circular_buffer_add(CircularBuffer *cb, float value) {
    if (xSemaphoreTake(cb->mutex, portMAX_DELAY) == pdTRUE) {
        if (cb->count < BUFFER_SIZE) {
            cb->data[cb->head] = value;
            cb->head = (cb->head + 1) % BUFFER_SIZE;
            cb->count++;
            xSemaphoreGive(cb->mutex);
            return true;
        }
        xSemaphoreGive(cb->mutex);
    }
    return false;
}

bool circular_buffer_read(CircularBuffer *cb, float *output, uint16_t *size) {
    if (xSemaphoreTake(cb->mutex, portMAX_DELAY) == pdTRUE) {
        *size = cb->count;
        if (*size > 0) {
            // Copy entire buffer contents
            uint16_t read_size = *size;
            for (uint16_t i = 0; i < read_size; i++) {
                output[i] = cb->data[(cb->tail + i) % BUFFER_SIZE];
            }
            
            // Reset buffer
            cb->head = 0;
            cb->tail = 0;
            cb->count = 0;
            
            xSemaphoreGive(cb->mutex);
            return true;
        }
        xSemaphoreGive(cb->mutex);
    }
    return false;
}

// Temperature task (Core 0)
void temp_task(void *pvParam) {
    ESP_LOGI(TAG, "Temperature task started");

    while (1) {
        // Read temperature
        float temperature = temp_sensor_read(tsens);
        
        // Add to circular buffer
        if (!circular_buffer_add(&temp_buffer, temperature)) {
            ESP_LOGW(TAG, "Buffer full, dropping temperature reading");
        }

        // Delay between samples
        vTaskDelay(pdMS_TO_TICKS(SAMPLE_INTERVAL_MS));
    }
}

// WiFi task (Core 1)
void wifi_task(void *pvParam) {
    int server_sock = -1;
    int client_sock = -1;
    
    // Initialize TCP server
    server_sock = tcp_server_init(3333);
    if (server_sock < 0) {
        ESP_LOGE(TAG, "Failed to initialize TCP server");
        vTaskDelete(NULL);
        return;
    }

    ESP_LOGI(TAG, "TCP Server initialized, waiting for connections");

    while (1) {
        // Accept client connection if not already connected
        if (client_sock < 0) {
            client_sock = accept(server_sock, NULL, NULL);
            if (client_sock < 0) {
                ESP_LOGE(TAG, "Failed to accept client connection: errno %d", errno);
                vTaskDelay(pdMS_TO_TICKS(1000));
                continue;
            }
            ESP_LOGI(TAG, "Client connected");
        }

        // Prepare buffer for transmission
        float transmission_buffer[BUFFER_SIZE];
        uint16_t data_size = 0;

        // Read from circular buffer
        if (circular_buffer_read(&temp_buffer, transmission_buffer, &data_size)) {
            if (data_size > 0) {
                // Send data
                ssize_t sent = send(client_sock, transmission_buffer, 
                                    data_size * sizeof(float), 0);
                
                if (sent < 0) {
                    ESP_LOGE(TAG, "Failed to send data: errno %d", errno);
                    close(client_sock);
                    client_sock = -1;
                    continue;
                }
                
                ESP_LOGI(TAG, "Sent %d temperature readings", data_size);
            }
        }

        // Wait before next transmission
        vTaskDelay(pdMS_TO_TICKS(TRANSMISSION_INTERVAL_MS));
    }
}

// Main application entry point
void app_main(void) {
    ESP_LOGI(TAG, "Starting application...");

    // Initialize NVS
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);

    // Initialize circular buffer
    circular_buffer_init(&temp_buffer);
    
    //initialize Temp sensor
    temp_sensor_init(&tsens);

    // Initialize WiFi
    wifi_ap_init();

    // Create tasks pinned to cores
    xTaskCreatePinnedToCore(temp_task, "TempTask", 4096, NULL, 5, &temp_task_handle, 0);
    xTaskCreatePinnedToCore(wifi_task, "WifiTask", 4096, NULL, 5, &wifi_task_handle, 1);

    ESP_LOGI(TAG, "Tasks created. System is now in STREAM mode.");
}