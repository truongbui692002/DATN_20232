#ifndef _HEADER_H_
#define _HEADER_H_
#include <math.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <inttypes.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "freertos/queue.h"
#include "freertos/ringbuf.h"
#include "freertos/semphr.h"

#include "nvs_flash.h"

#include "esp_log.h"
#include "esp_system.h"
#include "esp_sleep.h"

#include "esp_bt.h"
#include "esp_gap_ble_api.h"
#include "esp_gatts_api.h"
#include "esp_bt_defs.h"
#include "esp_bt_main.h"
#include "esp_gatt_common_api.h"

#include "sdkconfig.h"

#include "driver/gpio.h"
#include "driver/uart.h"

#include "driver/sdmmc_host.h"
#include "driver/sdspi_host.h"
#include "sdmmc_cmd.h"
#include <sys/unistd.h>
#include <sys/stat.h>
#include "esp_vfs_fat.h"

#include "esp_adc/adc_oneshot.h"
#include "esp_adc/adc_cali.h"
#include "esp_adc/adc_cali_scheme.h"

#define FIRMWARE_VER "1.0.0"
#define HARDWARE_VER "1.0.0"

#define UART2_TXD_PIN 4
#define UART2_RXD_PIN 16

// Pin mapping when using SPI mode.
#define PIN_NUM_MISO 34
#define PIN_NUM_MOSI 26
#define PIN_NUM_CLK 25
#define PIN_NUM_CS 13
#define SPI_DMA_CHAN host.slot

#endif