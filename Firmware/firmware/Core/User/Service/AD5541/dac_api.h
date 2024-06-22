/*
 * dac_api.h
 *
 *  	Created on: Mar 17, 2024
 *      Author: TruongBVD
 */

#ifndef _DAC_API_H_
#define _DAC_API_H_

#include <dac_func.h>
#include <dac_types.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

/******************* Macros and Constants Definitions *************************/
/******************************************************************************/
/*! Communication definitions */

/*! Device Characteristics */
#define MAX_OUT_VOLTAGE(x)      ((x) * 65535/65536.0) /*!< Maximum output voltage
                                                      for a known value of Vref */

void Delay_us (uint16_t us);
void DACInit(DAC_DeviceTypes_t *dev);
/*! Sends to DAC register through SPI the 16-bit data-word. */
void AD5541A_SetRegisterValue(DAC_DeviceTypes_t *dev, uint16_t registerValue);
/*! Sets the output voltage. */
double AD5441A_SetVotage(DAC_DeviceTypes_t *dev, double voltage);
/*! Get method for register value. */
unsigned short AD5541A_GetRegValue(void);
/*! Get method for output voltage. */
double AD5541A_GetOutVoltage(void);




#endif
