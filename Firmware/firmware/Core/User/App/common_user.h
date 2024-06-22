/*
 * common_user.h
 *
 *  Created on: May 30, 2024
 *      Author: DuyTruong
 */

#ifndef USER_APP_COMMON_USER_H_
#define USER_APP_COMMON_USER_H_

#include "stdint.h"
#include "stdbool.h"
#include "dac_api.h"
#include "adc_api.h"
#include "log.h"
#include "string.h"
#include "math.h"


typedef struct
{
	int config_device;
	int stop_mea;
	char mode_device[4];
	char res_device[3];
	double start_vol;
	double end_vol;
	double scan_rate;
	uint8_t num_cycle;
	bool config_done;
}configInfo_obj_t;

typedef struct
{
	int mode_dev;
	int res_dev;
	double target_volt;
	double start_volt;
	double current_volt;
	double step_volt;
	double scan_rate_volt;
	int num_cycle;
} deviceInfo_obj_t;

typedef struct
{
	bool rxBufAvai;
	uint16_t rxBufSize;
	bool startDev;
	bool devCfg;
}systemHandle_t;

typedef struct
{
	double potential;
	double current;
}measureData_t;

extern measureData_t measureData;

bool uart_parse_data(char *uart_data, configInfo_obj_t *configData_obj);

#endif /* USER_APP_COMMON_USER_H_ */
