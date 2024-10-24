#include "common_user.h"
#include "cJSON.h"

char *endptr; // Con trỏ để lưu vị trí kết thúc của chuỗi số

char Mode_dev[4] = {0};
char Res_dev[4] = {0};

char *acce_config;
char *mode;
char *res;
char *vol_s;
char *vol_e;
char *scan_r;
char *num_cycl;

static char argu1[2] = {0};         //accept config
static char argu2[2] = {0};         //mode device
static char argu3[2] = {0};         //res device
static char argu4[7] = {0};         //vol start
static char argu5[7] = {0};         //vol end
static char argu6[5] = {0};         //scan rate
static char argu7[4] = {0};         //num cycle

configInfo_t configInfo = {0};

void uint8_array_to_string(uint8_t* values, size_t length, char* buffer) {
    for (size_t i = 0; i < length; i++) {
        buffer[i] = (char)values[i];
    }
    buffer[length] = '\0'; // Kết thúc chuỗi
}

// Hàm chuyển đổi chuỗi sang mảng uint8_t
void string_to_uint8_array(const char* str, uint8_t* array, size_t length) {
    for (size_t i = 0; i < length; i++) {
        array[i] = (uint8_t)str[i];
    }
    array[length] = '\0';
}

// Hàm chuyển đổi float thành mảng byte
void floatToBytes(float value, uint8_t *bytes_array) {
    memcpy(bytes_array, &value, sizeof(float));
}

void int_to_bytes(int num, uint8_t *bytes) {
    bytes[0] = (num >> 24) & 0xFF;
    bytes[1] = (num >> 16) & 0xFF;
    bytes[2] = (num >> 8) & 0xFF;
    bytes[3] = num & 0xFF;
}

void parse_data_a_ble(char *data_a_ble, int len)
{
    //Get value to accept config 
    strncpy(argu1, data_a_ble+0, 2);
    acce_config = strtok(argu1, "@");
    configInfo.accept_conf = atoi(acce_config);
    ESP_LOGI(GATT_TAG, "Accept config: %d", configInfo.accept_conf);

    //Get mode device 
    strncpy(argu2, data_a_ble+2, 2);
    mode = strtok(argu2, "@");
    configInfo.num_mode = atoi(mode);
    if (configInfo.num_mode == 1) {
        sprintf(Mode_dev, "LSV");
    }
    else if (configInfo.num_mode == 2) {
        sprintf(Mode_dev, "CV");
    }
    ESP_LOGI(GATT_TAG, "Mode: %s", Mode_dev);
    
    //Get res device 
    strncpy(argu3, data_a_ble+4, 2);
    mode = strtok(argu3, "@");
    configInfo.num_res = atoi(mode);
    if (configInfo.num_res == 1) {
        sprintf(Res_dev, "mA");
    }
    else if (configInfo.num_res == 2) {
        sprintf(Res_dev, "μA");
    }
    else if (configInfo.num_res == 3) {
        sprintf(Res_dev, "nA");
    }
    ESP_LOGI(GATT_TAG, "Res: %s", Res_dev);
}

void parse_data_b_ble(char *data_b_ble, int len)
{
    // Get voltage start for device
    strncpy(argu4, data_b_ble+0, 6);
    vol_s = strtok(argu4, "@");
    configInfo.vol_start = strtod(vol_s, &endptr);
    ESP_LOGI(GATT_TAG, "Start voltage: %.1f", configInfo.vol_start);

    strncpy(argu5, data_b_ble+strlen(vol_s)+1, 6);
    vol_e = strtok(argu5, "@");
    configInfo.vol_end = strtod(vol_e, &endptr);
    ESP_LOGI(GATT_TAG, "End voltage: %.1f", configInfo.vol_end);

    //Get scan rate for device
    strncpy(argu6 , data_b_ble+strlen(vol_s)+strlen(vol_e)+2, 4);
    scan_r = strtok(argu6, "@");
    configInfo.scan_rate = strtod(scan_r, &endptr);
    ESP_LOGI(GATT_TAG, "Scan rate: %.1f", configInfo.scan_rate);

    //Get num cycle 
    if (configInfo.num_mode == 1) {
        return;
    } else if (configInfo.num_mode == 2) {
        strncpy(argu7 , data_b_ble+strlen(vol_s)+strlen(vol_e)+strlen(scan_r)+3, 3);
        num_cycl = strtok(argu7, "@");
        configInfo.num_cycle = atoi(num_cycl);
        ESP_LOGI(GATT_TAG, "num_cycle: %d", configInfo.num_cycle);
    }
}

esp_err_t uart_parse_data(char *uart_data, uart_obj_t *uart_obj)
{
    cJSON *root = cJSON_Parse(uart_data);
    if (root == NULL)
        return ESP_FAIL;
    cJSON *elem = NULL;
    cJSON_ArrayForEach(elem, root)
    {
        if (elem->string)
        {
            char *elem_str = elem->string;
            if(strcmp(elem_str, "start") == 0)
            {
                uart_obj->start = elem->valueint;
            }
            else if(strcmp(elem_str, "stop") == 0)
            {
                uart_obj->start = elem->valueint;
            }
            else if(!strcmp(elem_str, "potential"))
            {
                uart_obj->potential = elem->valueint;
            }
            else if(!strcmp(elem_str, "current"))
            {
                uart_obj->current = elem->valueint;
            }
        }
    }
    return ESP_OK;
}