#pragma once

#include "esp_err.h"

extern void settings_provider_init();
extern void settings_provider_free();

extern const char * get_wifi_settings();
extern esp_err_t set_wifi_settings(char * settings, size_t len);
extern int get_wifi_mode();
extern const char * get_wifi_ssid();
extern const char * get_wifi_password();

extern const char * get_modbus_settings();
extern esp_err_t set_modbus_settings(char * settings, size_t len);
