/* HTTP Restful API Server Example

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/
#include <string.h>
#include "esp_system.h"
#include "esp_wifi.h"
#include "sdkconfig.h"
#include "driver/gpio.h"
#include "esp_vfs_semihost.h"
#include "esp_vfs_fat.h"
#include "esp_spiffs.h"
#include "sdmmc_cmd.h"
#include "nvs_flash.h"
#include "esp_netif.h"
#include "esp_event.h"
#include "esp_log.h"
#include "mdns.h"
#include "lwip/apps/netbiosns.h"
#include "protocol_examples_common.h"
#include "cJSON.h"
#if CONFIG_EXAMPLE_WEB_DEPLOY_SD
#include "driver/sdmmc_host.h"
#endif
#if CONFIG_EXAMPLE_WEB_DEPLOY_SDSPI
#include "driver/spi_common.h"
#include "driver/sdspi_host.h"
#endif

// Pin mapping
#if CONFIG_IDF_TARGET_ESP32

#define PIN_NUM_MISO 2
#define PIN_NUM_MOSI 15
#define PIN_NUM_CLK  14
#define PIN_NUM_CS   13

#elif CONFIG_IDF_TARGET_ESP32S2

// adapted for internal test board ESP-32-S3-USB-OTG-Ev-BOARD_V1.0 (with ESP32-S2-MINI-1 module)
#define PIN_NUM_MISO 37
#define PIN_NUM_MOSI 35
#define PIN_NUM_CLK  36
#define PIN_NUM_CS   34

#elif CONFIG_IDF_TARGET_ESP32C3
#define PIN_NUM_MISO 6
#define PIN_NUM_MOSI 4
#define PIN_NUM_CLK  5
#define PIN_NUM_CS   1

#endif //CONFIG_IDF_TARGET_ESP32 || CONFIG_IDF_TARGET_ESP32S2

#if CONFIG_IDF_TARGET_ESP32S2
#define SPI_DMA_CHAN    host.slot
#elif CONFIG_IDF_TARGET_ESP32C3
#define SPI_DMA_CHAN    SPI_DMA_CH_AUTO
#else
#define SPI_DMA_CHAN    1
#endif

#define MDNS_INSTANCE "esp home web server"
#define EXAMPLE_ESP_WIFI_SSID      CONFIG_ESP_WIFI_SSID
#define EXAMPLE_ESP_WIFI_PASS      CONFIG_ESP_WIFI_PASSWORD
#define EXAMPLE_ESP_WIFI_CHANNEL   CONFIG_ESP_WIFI_CHANNEL
#define EXAMPLE_MAX_STA_CONN       CONFIG_ESP_MAX_STA_CONN

static const char *TAG = "example";

esp_err_t start_rest_server(const char *base_path);

// TBD Separate module
static cJSON *wifi; // TBD better name

static void initialise_mdns(void)
{
    mdns_init();
    mdns_hostname_set(CONFIG_EXAMPLE_MDNS_HOST_NAME);
    mdns_instance_name_set(MDNS_INSTANCE);

    mdns_txt_item_t serviceTxtData[] = {
        {"board", "esp32"},
        {"path", "/"}
    };

    ESP_ERROR_CHECK(mdns_service_add("ESP32-WebServer", "_http", "_tcp", 80, serviceTxtData,
                                     sizeof(serviceTxtData) / sizeof(serviceTxtData[0])));
}

// TBD: Probably to be removed
#if CONFIG_EXAMPLE_WEB_DEPLOY_SEMIHOST
esp_err_t init_fs(void)
{
    esp_err_t ret = esp_vfs_semihost_register(CONFIG_EXAMPLE_STORAGE_MOUNT_POINT, CONFIG_EXAMPLE_HOST_PATH_TO_MOUNT);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "Failed to register semihost driver (%s)!", esp_err_to_name(ret));
        return ESP_FAIL;
    }
    return ESP_OK;
}
#endif

#if CONFIG_EXAMPLE_WEB_DEPLOY_SD
esp_err_t init_fs(void)
{
    sdmmc_host_t host = SDMMC_HOST_DEFAULT();
    sdmmc_slot_config_t slot_config = SDMMC_SLOT_CONFIG_DEFAULT();

    gpio_set_pull_mode(15, GPIO_PULLUP_ONLY); // CMD
    gpio_set_pull_mode(2, GPIO_PULLUP_ONLY);  // D0
    gpio_set_pull_mode(4, GPIO_PULLUP_ONLY);  // D1
    gpio_set_pull_mode(12, GPIO_PULLUP_ONLY); // D2
    gpio_set_pull_mode(13, GPIO_PULLUP_ONLY); // D3

    esp_vfs_fat_sdmmc_mount_config_t mount_config = {
        .format_if_mount_failed = true,
        .max_files = 4,
        .allocation_unit_size = 16 * 1024
    };

    sdmmc_card_t *card;
    esp_err_t ret = esp_vfs_fat_sdmmc_mount(CONFIG_EXAMPLE_STORAGE_MOUNT_POINT, &host, &slot_config, &mount_config, &card);
    if (ret != ESP_OK) {
        if (ret == ESP_FAIL) {
            ESP_LOGE(TAG, "Failed to mount filesystem.");
        } else {
            ESP_LOGE(TAG, "Failed to initialize the card (%s)", esp_err_to_name(ret));
        }
        return ESP_FAIL;
    }
    /* print card info if mount successfully */
    sdmmc_card_print_info(stdout, card);
    return ESP_OK;
}
#endif

#if CONFIG_EXAMPLE_WEB_DEPLOY_SDSPI
esp_err_t init_fs(void)
{
    esp_err_t ret;

    // Use settings defined above to initialize SD card and mount FAT filesystem.
    // Note: esp_vfs_fat_sdmmc/sdspi_mount is all-in-one convenience functions.
    // Please check its source code and implement error recovery when developing
    // production applications.
    ESP_LOGI(TAG, "Using SPI peripheral");

    sdmmc_host_t host = SDSPI_HOST_DEFAULT();
    spi_bus_config_t bus_cfg = {
        .mosi_io_num = PIN_NUM_MOSI,
        .miso_io_num = PIN_NUM_MISO,
        .sclk_io_num = PIN_NUM_CLK,
        .quadwp_io_num = -1,
        .quadhd_io_num = -1,
        .max_transfer_sz = 4000,
    };
    ret = spi_bus_initialize(host.slot, &bus_cfg, SPI_DMA_CHAN);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "Failed to initialize bus.");
        return ESP_FAIL;
    }

    // This initializes the slot without card detect (CD) and write protect (WP) signals.
    // Modify slot_config.gpio_cd and slot_config.gpio_wp if your board has these signals.
    sdspi_device_config_t slot_config = SDSPI_DEVICE_CONFIG_DEFAULT();
    slot_config.gpio_cs = PIN_NUM_CS;
    slot_config.host_id = host.slot;

    esp_vfs_fat_sdmmc_mount_config_t mount_config = {
        .format_if_mount_failed = true,
        .max_files = 4,
        .allocation_unit_size = 16 * 1024
    };

    sdmmc_card_t *card;
    ESP_LOGI(TAG, "Mounting filesystem");
    ret = esp_vfs_fat_sdspi_mount(CONFIG_EXAMPLE_STORAGE_MOUNT_POINT, &host, &slot_config, &mount_config, &card);
    if (ret != ESP_OK) {
        if (ret == ESP_FAIL) {
            ESP_LOGE(TAG, "Failed to mount filesystem.");
        } else {
            ESP_LOGE(TAG, "Failed to initialize the card (%s)", esp_err_to_name(ret));
        }
        return ESP_FAIL;
    }
    ESP_LOGI(TAG, "Filesystem mounted");

    /* print card info if mount successfully */
    sdmmc_card_print_info(stdout, card);
    return ESP_OK;
}
#endif

#if CONFIG_EXAMPLE_WEB_DEPLOY_SF
esp_err_t init_fs(void)
{
    esp_vfs_spiffs_conf_t conf = {
        .base_path = CONFIG_EXAMPLE_STORAGE_MOUNT_POINT,
        .partition_label = NULL,
        .max_files = 5,
        .format_if_mount_failed = false
    };
    esp_err_t ret = esp_vfs_spiffs_register(&conf);

    if (ret != ESP_OK) {
        if (ret == ESP_FAIL) {
            ESP_LOGE(TAG, "Failed to mount or format filesystem");
        } else if (ret == ESP_ERR_NOT_FOUND) {
            ESP_LOGE(TAG, "Failed to find SPIFFS partition");
        } else {
            ESP_LOGE(TAG, "Failed to initialize SPIFFS (%s)", esp_err_to_name(ret));
        }
        return ESP_FAIL;
    }

    size_t total = 0, used = 0;
    ret = esp_spiffs_info(NULL, &total, &used);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "Failed to get SPIFFS partition information (%s)", esp_err_to_name(ret));
    } else {
        ESP_LOGI(TAG, "Partition size: total: %d, used: %d", total, used);
    }
    return ESP_OK;
}
#endif

static void wifi_event_handler(void* arg, esp_event_base_t event_base,
                                    int32_t event_id, void* event_data)
{
    if (event_id == WIFI_EVENT_AP_STACONNECTED) {
        wifi_event_ap_staconnected_t* event = (wifi_event_ap_staconnected_t*) event_data;
        ESP_LOGI(TAG, "station "MACSTR" join, AID=%d",
                 MAC2STR(event->mac), event->aid);
    } else if (event_id == WIFI_EVENT_AP_STADISCONNECTED) {
        wifi_event_ap_stadisconnected_t* event = (wifi_event_ap_stadisconnected_t*) event_data;
        ESP_LOGI(TAG, "station "MACSTR" leave, AID=%d",
                 MAC2STR(event->mac), event->aid);
    }
}

void wifi_init_softap(void)
{
    esp_netif_create_default_wifi_ap();

    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));

    ESP_ERROR_CHECK(esp_event_handler_instance_register(WIFI_EVENT,
                                                        ESP_EVENT_ANY_ID,
                                                        &wifi_event_handler,
                                                        NULL,
                                                        NULL));

    // TBD: Read SSID and PASSWORD from NVS, use defaults only if credentials not present in NVS.
    // TBD: SSID/PASSWORD default values configured in menuconfig
    // wifi_config_t wifi_config = {
    //     .ap = {
    //         .ssid = EXAMPLE_ESP_WIFI_SSID,
    //         .ssid_len = strlen(EXAMPLE_ESP_WIFI_SSID),
    //         .channel = EXAMPLE_ESP_WIFI_CHANNEL, // TBD add to configuration
    //         .password = EXAMPLE_ESP_WIFI_PASS,
    //         .max_connection = EXAMPLE_MAX_STA_CONN,  // TBD add to configuration
    //         .authmode = WIFI_AUTH_WPA_WPA2_PSK  // TBD add to configuration
    //     },
    // };
    char * wifi_ssid = cJSON_GetObjectItem(wifi, "ssid")->valuestring;
    int wifi_ssid_len = strlen(wifi_ssid);
    char * wifi_password = cJSON_GetObjectItem(wifi, "password")->valuestring;
    int wifi_password_len = strlen(wifi_ssid);
    wifi_config_t wifi_config = {
        .ap = {
            .ssid_len = wifi_ssid_len,
            .channel = EXAMPLE_ESP_WIFI_CHANNEL, // TBD add to configuration
            .max_connection = EXAMPLE_MAX_STA_CONN,  // TBD add to configuration
            .authmode = WIFI_AUTH_WPA_WPA2_PSK  // TBD add to configuration
        },
    };
    strncpy((char *)wifi_config.ap.ssid, wifi_ssid, 32);
    wifi_config.ap.ssid_len = wifi_ssid_len;
    strncpy((char *)wifi_config.ap.password, wifi_password, 64);

    // if (strlen(EXAMPLE_ESP_WIFI_PASS) == 0) {
    if (wifi_password_len == 0) {
        wifi_config.ap.authmode = WIFI_AUTH_OPEN;
    }

    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_AP));
    int wifi_mode = cJSON_GetObjectItem(wifi, "mode")->valueint;
    ESP_ERROR_CHECK(esp_wifi_set_mode(wifi_mode));
    switch (wifi_mode) {
        case WIFI_MODE_AP:
        ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_AP, &wifi_config));
        break;
        case WIFI_MODE_STA:
        ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_STA, &wifi_config));
        break;
    }
    // ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_AP, &wifi_config));
    ESP_ERROR_CHECK(esp_wifi_start());

    // ESP_LOGI(TAG, "wifi_init_softap finished. SSID:%s password:%s channel:%d",
    //          EXAMPLE_ESP_WIFI_SSID, EXAMPLE_ESP_WIFI_PASS, EXAMPLE_ESP_WIFI_CHANNEL);
    ESP_LOGI(TAG, "wifi_init_softap finished. SSID:%s password:%s channel:%d",
        cJSON_GetObjectItem(wifi, "ssid")->valuestring,
        cJSON_GetObjectItem(wifi, "password")->valuestring,
        EXAMPLE_ESP_WIFI_CHANNEL);
}

static esp_err_t load_config()
{
    const char *file_config = CONFIG_EXAMPLE_STORAGE_MOUNT_POINT"/conf/emconfig.json";
    ESP_LOGI(TAG, "Opening file %s", file_config);
    FILE *f = fopen(file_config, "r");
    if (f == NULL) {
        ESP_LOGE(TAG, "Failed to open %s\n", file_config);
        return ESP_FAIL;
    }
    int fseek_result = fseek(f, 0, SEEK_END);
    ESP_LOGI(TAG, "fseek_result: %d\n", fseek_result);

    long fsize = ftell(f);
    ESP_LOGI(TAG, "emconfig.json (%ld)\n", fsize);
    
    fseek_result = fseek(f, 0, SEEK_SET);  /* same as rewind(f); */
    ESP_LOGI(TAG, "fseek_result: %d\n", fseek_result);

    char *str_config = malloc(fsize + 1);
    size_t fread_result = fread(str_config, fsize, 1, f);
    ESP_LOGI(TAG, "fread_result: %u\n", fread_result);

    fclose(f);
    str_config[fsize] = 0;
    ESP_LOGI(TAG, "emconfig.json: %s\n", str_config);

    // TBD Error checking.
    wifi = cJSON_GetObjectItem(cJSON_Parse(str_config), "wifi");
    // int red = cJSON_GetObjectItem(root, "red")->valueint;
    // int green = cJSON_GetObjectItem(root, "green")->valueint;
    // int blue = cJSON_GetObjectItem(root, "blue")->valueint;
    // ESP_LOGI(TAG, "Light control: red = %d, green = %d, blue = %d", red, green, blue);
    // cJSON_Delete(root);

    free(str_config);
    return ESP_OK;
}

void app_main(void)
{
    ESP_ERROR_CHECK(nvs_flash_init());
    ESP_ERROR_CHECK(init_fs());
    ESP_ERROR_CHECK(load_config());
    // TBD In the example_connect() there is waiting for IP addresses, see on_got_ip(), do we need that?
    // ESP_ERROR_CHECK(example_connect());
    ESP_ERROR_CHECK(esp_netif_init());
    ESP_ERROR_CHECK(esp_event_loop_create_default());
    initialise_mdns();
    netbiosns_init();
    netbiosns_set_name(CONFIG_EXAMPLE_MDNS_HOST_NAME);
    wifi_init_softap();
    ESP_ERROR_CHECK(start_rest_server(CONFIG_EXAMPLE_WEB_BASE_PATH));
}
