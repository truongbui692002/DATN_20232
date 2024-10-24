#include "uart_user.h"
#include "common_user.h"
#include "sdcard.h"

static const char *TAG = "UART";

char data_collect[MAX_CHAR_SIZE];

extern char data_ble_recv_a[256];
extern char data_ble_recv_b[256];

char data_a_ble_write[256] = {0};
char data_b_ble_write[256] = {0};

int argu_cfg_done = 0;

int msg_size_ble = 0;
int msg_size = 0;
char RxBuffer[256] = {0};
char TxBuffer[256] = {0};

extern configInfo_t configInfo;

extern char Mode_dev[4];
extern char Res_dev[4];

bool start_dev = false;
bool write_sd = false;

uart_obj_t uart_obj = {0};

int voltage[100] = {0};
float current[100] = {0};
int i = 0;
void uart_init(void)
{
    uart_config_t uart_cfg = {
        .baud_rate = 115200,
        .data_bits = UART_DATA_8_BITS,
        .parity = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
        .source_clk = UART_SCLK_APB,
    };
    uart_driver_install(UART_NUM_2, 256 * 2, 0, 0, NULL, 0);
    uart_param_config(UART_NUM_2, &uart_cfg);
    uart_set_pin(UART_NUM_2, UART2_TXD_PIN, UART2_RXD_PIN, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE);
}

void uart_user_task(void *param)
{

    uart_init();
    esp_err_t ret;
    while (1)
    {
        msg_size = uart_read_bytes(UART_NUM_2, RxBuffer, sizeof(RxBuffer), 10 / portTICK_PERIOD_MS);
        msg_size_ble =strlen(data_ble_recv_a);
        data_ble_recv_a[msg_size_ble] = '\0';
        if(msg_size)
            {
                RxBuffer[msg_size] = '\0';
                ESP_LOGI(TAG, "Data from STM: %s, MSG_Size:%d", RxBuffer, msg_size);
                
                if ( (strcmp(RxBuffer, "Config_done") != 0) && (strcmp(RxBuffer, "$") != 0 )){
                    // ESP_LOGI(TAG,"Hellooooooo");
                    ret = uart_parse_data(RxBuffer, &uart_obj);    
                    if (ret == ESP_OK)
                    {
                        ESP_LOGI(TAG, "start:%d",uart_obj.start);
                        voltage[i] = uart_obj.potential;
                        current[i] = uart_obj.current/100.0;
                        i++;
                        ESP_LOGI(TAG, "%d - %f - %d", voltage[i -1], current[i-1], i);        
                        if (uart_obj.start == 1)
                        {
                            start_dev = true;
                            ESP_LOGI(TAG, "DATA WRITE TO APP: %d-%d", uart_obj.potential, uart_obj.current);
                        }    
                    } 
                } else if (strcmp(RxBuffer, "$") == 0) {
                    start_dev = false;
                    ESP_LOGI(TAG, "start: %d", start_dev);
                }
                       
            }
          
        if(strcmp(data_ble_recv_a, "GATT_Connected") == 0)
            {
                sprintf(TxBuffer, "GATT_Connected" );
                uart_write_bytes(UART_NUM_2, TxBuffer, strlen(TxBuffer));
                    
                ESP_LOGI(TAG, "Data recv_a from BLE: %s", data_ble_recv_a);
                bzero(data_ble_recv_a, msg_size_ble);
            }
        if( configInfo.accept_conf == 1)
        {        
                    if (configInfo.num_mode == 1) {
                        sprintf(TxBuffer, "{\"config_dev\":%d,\"mode\":\"%s\",\"res\":\"%s\",\"vol_start\":%.1f,\"vol_end\":%.1f,\"scan_rate\":%.1f}", configInfo.accept_conf, Mode_dev, Res_dev, configInfo.vol_start, configInfo.vol_end, configInfo.scan_rate);
                    } else if (configInfo.num_mode == 2) {
                        sprintf(TxBuffer, "{\"config_dev\":%d,\"mode\":\"%s\",\"res\":\"%s\",\"vol_start\":%.1f,\"vol_end\":%.1f,\"scan_rate\":%.1f,\"num_cycle\":%d}", configInfo.accept_conf, Mode_dev, Res_dev, configInfo.vol_start, configInfo.vol_end, configInfo.scan_rate, configInfo.num_cycle);
                    }
                    vTaskDelay(100);
                    uart_write_bytes(UART_NUM_2, TxBuffer, strlen(TxBuffer));
                    snprintf(data_collect, MAX_CHAR_SIZE, "%s\n", TxBuffer);
                    ESP_LOGI(TAG, "%s", TxBuffer);
                    // ESP_LOGI(TAG, "Data recv from BLE: %s, Acc_conf: %d", data_ble_recv_a, accept_conf);
                    memset(&configInfo, 0 , sizeof(configInfo));
        }
        if (strcmp(data_ble_recv_a, "Start_device") == 0)
        {
            memset(voltage, 0, sizeof(voltage));
            memset(current, 0, sizeof(current));
            i = 0;
            sprintf(TxBuffer, "Start_device" );
            start_dev = true;
            uart_write_bytes(UART_NUM_2, TxBuffer, strlen(TxBuffer));                  
            ESP_LOGI(TAG, "Data recv from BLE: %s", data_ble_recv_a);
            bzero(data_ble_recv_a, msg_size_ble);           
        } 
        if (strcmp(data_ble_recv_a, "Save_data") == 0)
        {          
            write_sd =true;
            ESP_LOGI(TAG, "Data recv from BLE: %s", data_ble_recv_a);
            bzero(data_ble_recv_a, msg_size_ble);     
        }
        vTaskDelay(50);
    }  
}