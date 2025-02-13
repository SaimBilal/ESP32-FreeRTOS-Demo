#include "wifi_ap.h"
#include "esp_wifi.h"
#include "esp_netif.h"
#include "lwip/sockets.h"
#include "esp_log.h"
#include "esp_netif_ip_addr.h"

static const char *TAG = "wifi_ap";

// Initialize WiFi AP
void wifi_ap_init(void) {
    // Initialize TCP/IP stack
    ESP_ERROR_CHECK(esp_netif_init());

    // Create default event loop
    ESP_ERROR_CHECK(esp_event_loop_create_default());

    // Create default WiFi AP and store the netif pointer
    esp_netif_t *ap_netif = esp_netif_create_default_wifi_ap();
    if (ap_netif == NULL) {
        ESP_LOGE(TAG, "Failed to create default WiFi AP netif");
        return;
    }

    // Initialize WiFi with default config
    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));

    // Configure WiFi AP settings
    wifi_config_t wifi_config = {
        .ap = {
            .ssid = "ESP32-S3_AP",       // SSID of the AP
            .password = "password123",   // Password (min 8 characters)
            .max_connection = 1,         // Max number of connected clients
            .authmode = WIFI_AUTH_WPA2_PSK // Authentication mode
        }
    };

    // Set WiFi mode to AP
    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_AP));

    // Set WiFi configuration
    ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_AP, &wifi_config));

    // Start WiFi
    ESP_ERROR_CHECK(esp_wifi_start());

    // Log the AP's IP address
    esp_netif_ip_info_t ip_info;
    ESP_ERROR_CHECK(esp_netif_get_ip_info(ap_netif, &ip_info));
    ESP_LOGI(TAG, "WiFi AP started with SSID: %s", wifi_config.ap.ssid);
    ESP_LOGI(TAG, "AP IP Address: " IPSTR, IP2STR(&ip_info.ip));
}

// Initialize TCP server and return the server socket
int tcp_server_init(int port) {
    int sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sock < 0) {
        ESP_LOGE(TAG, "Failed to create TCP socket");
        return -1;
    }

    // Configure server address
    struct sockaddr_in server_addr = {
        .sin_family = AF_INET,
        .sin_port = htons(port),
        .sin_addr.s_addr = htonl(INADDR_ANY)
    };

    // Bind the socket to the server address
    if (bind(sock, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        ESP_LOGE(TAG, "Failed to bind TCP socket");
        close(sock);
        return -1;
    }

    // Listen for incoming connections
    if (listen(sock, 1) < 0) {
        ESP_LOGE(TAG, "Failed to listen on TCP socket");
        close(sock);
        return -1;
    }

    ESP_LOGI(TAG, "TCP server started on port %d", port);
    return sock;
}

// Send data to the connected client
void tcp_send(int client_sock, void *data, size_t size) {
    if (client_sock < 0) {
        ESP_LOGE(TAG, "Invalid client socket");
        return;
    }

    ssize_t sent = send(client_sock, data, size, 0);
    if (sent < 0) {
        ESP_LOGE(TAG, "Failed to send data to client");
    } else {
        ESP_LOGI(TAG, "Sent %d bytes to client", sent);
    }
}