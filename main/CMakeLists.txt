idf_component_register(SRCS "nodma_temp_wifi.c"
                    INCLUDE_DIRS "."
                    REQUIRES wifi_manager temperature_sensor nvs_flash)
