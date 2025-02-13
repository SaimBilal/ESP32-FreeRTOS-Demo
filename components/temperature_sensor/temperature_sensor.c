#include "temperature_sensor.h"
#include "esp_log.h"

static const char *TAG = "temp_sensor";

void temp_sensor_init(temperature_sensor_handle_t *tsens) {
    // Configure the temperature sensor with a default range of 10°C to 50°C
    temperature_sensor_config_t temp_sensor_config = TEMPERATURE_SENSOR_CONFIG_DEFAULT(10, 50);
    ESP_ERROR_CHECK(temperature_sensor_install(&temp_sensor_config, tsens));

    // Enable the temperature sensor
    ESP_ERROR_CHECK(temperature_sensor_enable(*tsens));
    ESP_LOGI(TAG, "Temperature sensor initialized and enabled");
}

float temp_sensor_read(temperature_sensor_handle_t tsens) {
    float temp_c = 0;
    ESP_ERROR_CHECK(temperature_sensor_get_celsius(tsens, &temp_c));
    return temp_c;
}