#pragma once

extern void settings_prvider_init();
extern void settings_prvider_free();

extern const char * get_wifi_settings_str();
extern int get_wifi_mode();
extern const char * get_wifi_ssid();
extern const char * get_wifi_password();

extern const char * get_modbus_settings_str();
