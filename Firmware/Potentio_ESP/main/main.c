#include "header.h"
#include "logo.h"
#include "app_ble.h"
#include "uart_user.h"
#include "sdcard.h"
#include "adc.h"

static const char *TAG = "MAIN";

void app_main()
{
    esp_err_t err;

    err = nvs_flash_init();

    if (err == ESP_ERR_NVS_NO_FREE_PAGES)
    {
        ESP_ERROR_CHECK(nvs_flash_erase());
        err = nvs_flash_init();
    }
    ESP_ERROR_CHECK(err);
    printf("%s", Poten_LOGO);

    ble_init();
    
    xTaskCreate(&uart_user_task, "UART", 8192, NULL, 10, NULL);
    xTaskCreate(&sdcard_task, "SDCARD", 4096, NULL, 9, NULL);
    xTaskCreate(&adc_task, "ADC", 2048, NULL, 9, NULL);
}