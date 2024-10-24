#include "sdcard.h"

static const char *TAG = "SDCARD";

const char mount_point[] = MOUNT_POINT;
sdmmc_host_t host = SDSPI_HOST_DEFAULT();
sdmmc_card_t *card;
esp_err_t ret;

extern bool write_sd;

extern char TxBuffer[256];
extern char data_collect[MAX_CHAR_SIZE];

extern int voltage[100];
extern float current[100];
extern int i;

// void s_example_write_file(const char *path, char *data)
// {
//     ESP_LOGI(TAG, "Opening file %s", path);
//     FILE *f = fopen(path, "w");
//     if (f == NULL) {
//         ESP_LOGE(TAG, "Failed to open file for writing");
//     }
//     fprintf(f, data);
//     fclose(f);
//     ESP_LOGI(TAG, "File written");

// }

void s_example_read_file(const char *path)
{
    ESP_LOGI(TAG, "Reading file %s", path);
    FILE *f = fopen(path, "r");
    if (f == NULL) {
        ESP_LOGE(TAG, "Failed to open file for reading");
    }
    char line[MAX_CHAR_SIZE];
    fgets(line, sizeof(line), f);
    fclose(f);

    // strip newline
    char *pos = strchr(line, '\n');
    if (pos) {
        *pos = '\0';
    }
    ESP_LOGI(TAG, "Read from file: '%s'", line);
}

void sdcard_init(void)
{
    
    // Options for mounting the filesystem.
    // If format_if_mount_failed is set to true, SD card will be partitioned and
    // formatted in case when mounting fails.
    esp_vfs_fat_sdmmc_mount_config_t mount_config = {
#ifdef CONFIG_EXAMPLE_FORMAT_IF_MOUNT_FAILED
        .format_if_mount_failed = true,
#else
        .format_if_mount_failed = false,
#endif // EXAMPLE_FORMAT_IF_MOUNT_FAILED
        .max_files = 5,
        .allocation_unit_size = 16 * 1024
    };
    
    
    ESP_LOGI(TAG, "Initializing SD card");

    // Use settings defined above to initialize SD card and mount FAT filesystem.
    // Note: esp_vfs_fat_sdmmc/sdspi_mount is all-in-one convenience functions.
    // Please check its source code and implement error recovery when developing
    // production applications.
    ESP_LOGI(TAG, "Using SPI peripheral");

    // By default, SD card frequency is initialized to SDMMC_FREQ_DEFAULT (20MHz)
    // For setting a specific frequency, use host.max_freq_khz (range 400kHz - 20MHz for SDSPI)
    // Example: for fixed frequency of 10MHz, use host.max_freq_khz = 10000;

    spi_bus_config_t bus_cfg = {
        .mosi_io_num = PIN_NUM_MOSI,
        .miso_io_num = PIN_NUM_MISO,
        .sclk_io_num = PIN_NUM_CLK,
        .quadwp_io_num = -1,
        .quadhd_io_num = -1,
        .max_transfer_sz = 4000,
    };
    ret = spi_bus_initialize(host.slot, &bus_cfg, SDSPI_DEFAULT_DMA);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "Failed to initialize bus.");
    }

    // This initializes the slot without card detect (CD) and write protect (WP) signals.
    // Modify slot_config.gpio_cd and slot_config.gpio_wp if your board has these signals.
    sdspi_device_config_t slot_config = SDSPI_DEVICE_CONFIG_DEFAULT();
    slot_config.gpio_cs = PIN_NUM_CS;
    slot_config.host_id = host.slot;

    ESP_LOGI(TAG, "Mounting filesystem");
    ret = esp_vfs_fat_sdspi_mount(mount_point, &host, &slot_config, &mount_config, &card);

    if (ret != ESP_OK) {
        if (ret == ESP_FAIL) {
            ESP_LOGE(TAG, "Failed to mount filesystem. "
                     "If you want the card to be formatted, set the CONFIG_EXAMPLE_FORMAT_IF_MOUNT_FAILED menuconfig option.");
        } else {
            ESP_LOGE(TAG, "Failed to initialize the card (%s). "
                     "Make sure SD card lines have pull-up resistors in place.", esp_err_to_name(ret));
        }
    }
    ESP_LOGI(TAG, "Filesystem mounted");

    // Card has been initialized, print its properties
    sdmmc_card_print_info(stdout, card);
}

void sdcard_task(void *param)
{
    sdcard_init();

    while(1)
    {
        if(write_sd == true)
        {
            const char *file_data = MOUNT_POINT"/potentio.csv";
            ESP_LOGI(TAG, "Opening file %s", file_data);
            FILE *f = fopen(file_data, "a");
            if (f == NULL) {
                ESP_LOGE(TAG, "Failed to open file for writing");
            }
            fprintf(f, data_collect);
            for (int j = 0; j < i; j++)
            {
                // ESP_LOGI(TAG, "%d - %f - %d", voltage[j], current[j], i);        
                snprintf(data_collect, MAX_CHAR_SIZE, "%d;%f\n", voltage[j], current[j]);
                fprintf(f, data_collect);
            }
            fclose(f);
            ESP_LOGI(TAG, "File written");

            s_example_read_file(file_data);

            // // All done, unmount partition and disable SPI peripheral
            // esp_vfs_fat_sdcard_unmount(mount_point, card);
            // ESP_LOGI(TAG, "Card unmounted");

            // //deinitialize the bus after all devices are removed
            // spi_bus_free(host.slot);
            write_sd = false;
        }        
    }
}