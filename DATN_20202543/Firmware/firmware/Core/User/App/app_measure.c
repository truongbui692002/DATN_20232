/*
 * app_measure.c
 *
 *  Created on: May 30, 2024
 *      Author: DuyTruong
 */

#include "main.h"
#include "stimer.h"
#include "common_user.h"

DAC_DeviceTypes_t dev1;
ADC_DeviceTypes_t dev2;

deviceInfo_obj_t deviceInfo_t = {0};
void MAX4617_Channel(int C, int B, int A)
{
	if(A == 0) HAL_GPIO_WritePin(SW_A_GPIO_Port, SW_A_Pin, 0);
	else if(A == 1) HAL_GPIO_WritePin(SW_A_GPIO_Port, SW_A_Pin, 1);

	if(B == 0) HAL_GPIO_WritePin(SW_B_GPIO_Port, SW_B_Pin, 0);
	else if(B == 1) HAL_GPIO_WritePin(SW_B_GPIO_Port, SW_B_Pin, 1);

	if(C == 0) HAL_GPIO_WritePin(SW_C_GPIO_Port, SW_C_Pin, 0);
	else if(C == 1) HAL_GPIO_WritePin(SW_C_GPIO_Port, SW_C_Pin, 1);
}


void Config_dev(configInfo_obj_t *configInfo_t)
{
	if (configInfo_t->config_done == false)
	{
		if(strstr(configInfo_t->mode_device, "LSV") != NULL) {
				deviceInfo_t.mode_dev =1;
			} else if(strstr(configInfo_t->mode_device, "CV") != NULL) {
				deviceInfo_t.mode_dev =2;
			}
			if(strstr(configInfo_t->res_device, "mA") != NULL) {
				deviceInfo_t.res_dev =1;
			} else if(strstr(configInfo_t->res_device, "μA") != NULL) {
				deviceInfo_t.res_dev =2;
			} else if(strstr(configInfo_t->res_device, "nA") != NULL) {
				deviceInfo_t.res_dev =3;
			}
			deviceInfo_t.target_volt = configInfo_t->end_vol/1000.0;
			deviceInfo_t.current_volt = configInfo_t->start_vol/1000.0;
			deviceInfo_t.step_volt = configInfo_t->scan_rate/1000.0;
			deviceInfo_t.scan_rate_volt = configInfo_t->scan_rate*1.0;
			deviceInfo_t.num_cycle = configInfo_t->num_cycle;
			deviceInfo_t.start_volt = configInfo_t->start_vol/1000.0;
			if(deviceInfo_t.res_dev == 1) {
				MAX4617_Channel(0, 0, 1); //R=1500
			} else if (deviceInfo_t.res_dev == 2) {
				MAX4617_Channel(0, 1, 0); // R =15k
			} else if (deviceInfo_t.res_dev == 3) {
				MAX4617_Channel(0, 0, 0); // R = 1M
			}
				configInfo_t->config_done = true;
	}

	if (configInfo_t->config_done)
	{
		uint8_t *cmd = (uint8_t*)"Config_done";
		HAL_UART_Transmit(&huart2, cmd, strlen((char*)cmd), 1000);
		Logi("Mode: %d - Res: %d - Current vol: %.1f - End vol:%.1f - scan rate: %.1f - num cycle: %d",deviceInfo_t.mode_dev, deviceInfo_t.res_dev, deviceInfo_t.num_cycle, deviceInfo_t.current_volt, deviceInfo_t.target_volt, deviceInfo_t.scan_rate_volt, deviceInfo_t.num_cycle);
		for(int i = 0; i<4; i++){
			HAL_GPIO_TogglePin(LED_G_GPIO_Port, LED_G_Pin);
			HAL_Delay(300);
		}
		HAL_GPIO_WritePin(LED_G_GPIO_Port, LED_G_Pin, 1);
		Logi("Config device done");
	}
}

void appMeasureInit(void)
{
	ADCInit(&dev2);
	DACInit(&dev1);
}




