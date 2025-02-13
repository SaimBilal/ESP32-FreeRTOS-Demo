#pragma once
#include "driver/temperature_sensor.h"

#ifdef __cplusplus
extern "C" {
#endif

// Initialize the temperature sensor
void temp_sensor_init(temperature_sensor_handle_t *tsens);

// Read the current temperature in Celsius
float temp_sensor_read(temperature_sensor_handle_t tsens);

#ifdef __cplusplus
}
#endif