#ifndef _COMMON_USER_H_
#define _COMMON_USER_H_

#include "header.h"
#include "app_ble.h"

typedef struct {
    int accept_conf;
    int num_mode;
    int num_res;
    double vol_start;
    double vol_end;
    double scan_rate;
    int num_cycle;
} configInfo_t;

typedef struct {
    int start;
    int stop;
    int potential;
    int current;
} uart_obj_t;

union intBytes
{
    /* data */
    int data;
    uint8_t byte[4];
};



void uint8_array_to_string(uint8_t* values, size_t length, char* buffer);
void string_to_uint8_array(const char* str, uint8_t* array, size_t length);
void floatToBytes(float value, uint8_t *bytes_array);
void int_to_bytes(int num, uint8_t *bytes);

void parse_data_a_ble(char *data_a_ble, int len);
void parse_data_b_ble(char *data_b_ble, int len);
esp_err_t uart_parse_data(char *uart_data, uart_obj_t *uart_obj);

#endif
