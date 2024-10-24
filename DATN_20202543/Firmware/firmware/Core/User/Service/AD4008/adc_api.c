/*
 * adc_api.h
 *
 *  	Created on: Mar 17, 2024
 *      Author: TruongBVD
 */

#include <adc_api.h>
#include "main.h"

extern SPI_HandleTypeDef hspi1;


/******************************************************************************/
/************************ Functions Definitions *******************************/
/******************************************************************************/

void adcdummyHandler(ADC_DeviceTypes_t *dev)
{
    ;
}

void ADCInit(ADC_DeviceTypes_t *dev)
{
    dev->func = &ADCFunc;
    dev->handleSent = adcdummyHandler;
	dev->handleError = adcdummyHandler;
	dev->handleReceived = adcdummyHandler;
	dev->handleReceiveTimeout = adcdummyHandler;
	dev->handleReceiveFailed = adcdummyHandler;
}

// Private Functions --------------------------------------------------

// Delay for at least max possible conversion time (2.1us)
static void AD4008_Conversion_Delay(void)
{
  // 16 MHz CPU, 40*(1/16*10^6) = 2.5us
  for(int i=0; i<40; i++)
  {
    asm("nop");
  }
}

uint16_t AD4008_single_conversion(ADC_DeviceTypes_t *dev, uint16_t adc_data)
{
	uint8_t buf[2] = {0, 0};
	adc_data = 0;

	HAL_GPIO_WritePin(ADC_CS_GPIO_Port, ADC_CS_Pin, GPIO_PIN_SET);

//	AD4008_Conversion_Delay();

	dev->func->spiRead(dev, &buf, 2);
//	HAL_SPI_Receive(&hspi1, &buf[0], 1, 100);
//	HAL_SPI_Receive(&hspi1, &buf[1], 1, 100);

	HAL_GPIO_WritePin(ADC_CS_GPIO_Port, ADC_CS_Pin, GPIO_PIN_RESET);

	adc_data = ((buf[0] << 8) | buf[1] ) & 0xFFFF;

	return adc_data;

}



