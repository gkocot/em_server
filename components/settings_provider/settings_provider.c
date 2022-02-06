#include <stdio.h>
#include <errno.h>
#include "esp_system.h"
#include "esp_log.h"
#include "cJSON.h"
#include "settings_provider.h"

// DEBUG
#include <dirent.h>

static const char * TAG = "settings_provider";

static char * wifi_settings_str = NULL;
static char * modbus_settings_str = NULL;
static cJSON * wifi_cJSON = NULL;
// static cJSON * modbus_cJSON;

static char * load_config(const char * path)
{
    FILE * f = fopen(path, "r");
    if (f == NULL) {
        ESP_LOGE(TAG, "fopen(%s, \"r\") failed, errno %d\n", path, errno);
        return NULL;
    }

    int fseek_result;
    if ((fseek_result = fseek(f, 0, SEEK_END)) != 0) {
        ESP_LOGE(TAG, "fseek() failed, fseek_result %d, errno %d\n", fseek_result, errno);
        goto load_config_error_close_file;
    }

    long size;
    if ((size = ftell(f)) < 0) {
        ESP_LOGE(TAG, "ftell() failed, errno %d\n", errno);
        goto load_config_error_close_file;
    }
    
    if ((fseek_result = fseek(f, 0, SEEK_SET)) != 0) {
        ESP_LOGE(TAG, "fseek() failed, fseek_result %d, errno %d\n", fseek_result, errno);
        goto load_config_error_close_file;
    }

    char * config;
    if ((config = malloc(size + 1)) == NULL) {
        ESP_LOGE(TAG, "malloc() failed\n");
        goto load_config_error_close_file;
    }

    size_t fread_result;
    if ((fread_result = fread(config, size, 1, f)) != 1) {
        ESP_LOGE(TAG, "fread() failed\n");
        goto load_config_error_free;
    }

    fclose(f);
    config[size] = 0;
    return config;

load_config_error_free:
    free(config);

load_config_error_close_file:
    fclose(f);
    return NULL;
}

static esp_err_t save_config(const char * path, char * config, size_t len)
{
    FILE * f = fopen(path, "w");
    if (f == NULL) {
        ESP_LOGE(TAG, "fopen(%s, \"w\") failed, errno %d\n", path, errno);
        return ESP_FAIL;
    }


    if (fwrite(config, len, 1, f) != 1) {
        ESP_LOGE(TAG, "fwrite() failed\n");
        fclose(f);
        return ESP_FAIL;
    }

    fclose(f);
    return ESP_OK;
}


extern void settings_provider_init()
{
    // DEBUG
    // const char *storage_root_path = CONFIG_EXAMPLE_STORAGE_MOUNT_POINT"/";
    // DIR *dir = opendir(storage_root_path);
    // if (dir == NULL) {
    //     ESP_LOGE(TAG, "ERROR opening %s, errno=%d\n", storage_root_path, errno);
    //     // return ESP_FAIL;
    // }
    // else {
    //     ESP_LOGI(TAG, "SUCCESS opening %s, errno=%d\n", storage_root_path, errno);
    //     struct dirent *d;
    //     while ((d = readdir(dir)) != NULL) {
    //         printf("%s %d\n", d->d_name, d->d_type);
    //     }
    // }
    // DEBUG

    if ((wifi_settings_str = load_config(CONFIG_WIFI_SETTINGS_STORAGE)) == NULL) {
        ESP_LOGE(TAG, "Loading WiFi settings failed\n");
    };

    if ((modbus_settings_str = load_config(CONFIG_MODBUS_SETTINGS_STORAGE)) == NULL) {
        ESP_LOGE(TAG, "Loading Modbus settings failed\n");
    };

    // Example how to get nested objects.
    // wifi = cJSON_GetObjectItem(cJSON_Parse(str_config), "wifi");
    wifi_cJSON = cJSON_Parse(wifi_settings_str);
    // modbus_cJSON = cJSON_Parse(modbus_settings_str);
}

extern const char * get_wifi_settings()
{
    if (wifi_settings_str != NULL) {
        return wifi_settings_str;
    }
    else {
        return "{}"; // TBD: Hardcoded default settings.
    }
}

extern esp_err_t set_wifi_settings(char * settings, size_t len)
{
    if (save_config(CONFIG_WIFI_SETTINGS_STORAGE, settings, len) != ESP_OK) {
        ESP_LOGE(TAG, "Saving WiFi settings failed\n");
        return ESP_FAIL;
    };

    char * new_wifi_settings_str = NULL;
    if ((new_wifi_settings_str = load_config(CONFIG_WIFI_SETTINGS_STORAGE)) == NULL) {
        ESP_LOGE(TAG, "Reloading WiFi settings failed\n");
        return ESP_FAIL;
    };

    free(wifi_settings_str);
    wifi_settings_str = new_wifi_settings_str;
    return ESP_OK;
}

extern int get_wifi_mode()
{
    return cJSON_GetObjectItem(wifi_cJSON, "mode")->valueint;
}

extern const char * get_wifi_ssid()
{
    return cJSON_GetObjectItem(wifi_cJSON, "ssid")->valuestring;
}

extern const char * get_wifi_password()
{
    return cJSON_GetObjectItem(wifi_cJSON, "password")->valuestring;
}

extern const char * get_modbus_settings()
{
    if (modbus_settings_str != NULL) {
        return modbus_settings_str;
    }
    else {
        return "{}"; // TBD: Hardcoded default settings.
    }
}

extern esp_err_t set_modbus_settings(char * settings, size_t len)
{
    if (save_config(CONFIG_MODBUS_SETTINGS_STORAGE, settings, len) != ESP_OK) {
        ESP_LOGE(TAG, "Saving Modbus settings failed\n");
        return ESP_FAIL;
    };

    char * new_modbus_settings_str = NULL;
    if ((new_modbus_settings_str = load_config(CONFIG_MODBUS_SETTINGS_STORAGE)) == NULL) {
        ESP_LOGE(TAG, "Reloading Modbus settings failed\n");
        return ESP_FAIL;
    };

    free(modbus_settings_str);
    modbus_settings_str = new_modbus_settings_str;
    return ESP_OK;
}

extern void settings_provider_free()
{
    free(wifi_settings_str);
    wifi_settings_str = NULL;
    cJSON_Delete(wifi_cJSON);
    wifi_cJSON = NULL;

    free(modbus_settings_str);
    modbus_settings_str = NULL;
    // cJSON_Delete(wifi_cJSON);
    // wifi_cJSON = NULL;
}
