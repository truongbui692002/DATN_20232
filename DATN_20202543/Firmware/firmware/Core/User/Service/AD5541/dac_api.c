/*
 * dac_api.c
 *
 *  	Created on: Mar 17, 2024
 *      Author: TruongBVD
 */

#include <dac_api.h>
#include "main.h"

extern SPI_HandleTypeDef hspi1;
//extern TIM_HandleTypeDef htim2;

uint16_t mRegValue;
double  mOutputVoltage;
double	mVref;

/******************************************************************************/
/************************ Functions Definitions *******************************/
/******************************************************************************/

void dummyHandler(DAC_DeviceTypes_t *dev)
{
    ;
}

void DACInit(DAC_DeviceTypes_t *dev)
{
	/*! Initialize class members */
	mVref = 5.125;  /*!< Vref = 5 V */
	mOutputVoltage = 0;
	mRegValue = 0;

    dev->func = &DACFunc;
    dev->handleSent = dummyHandler;
	dev->handleError = dummyHandler;
	dev->handleReceived = dummyHandler;
	dev->handleReceiveTimeout = dummyHandler;
	dev->handleReceiveFailed = dummyHandler;
}

void AD5541A_SetRegisterValue(DAC_DeviceTypes_t *dev, uint16_t registerValue)
{
	unsigned char registerWord[2] = {0, 0};

	/*! Check if parameter is valid */
		if((registerValue >= 0x0) && (registerValue <= 0xFFFF))
		{
			mRegValue = registerValue;
		}
		else if(registerValue < 0)
		{
			mRegValue = 0;
		}
		else
		{
			mRegValue = 0xFF;
		}

		registerWord[0] = (mRegValue >> 8);
		registerWord[1] = (mRegValue & 0xFF);
		dev->func->spiWrite(dev, registerWord, 2);
		mOutputVoltage = mVref * ((double) mRegValue / 65536);
}

/***************************************************************************//**
 * @brief Sets the output voltage.
 *
 * @param voltage - Output voltage. Accepted values: 0 - 2.5
 *
 * @return actualVoltage - Voltage value closest to the desired value that can
                           be outputed by the device.
*******************************************************************************/
double AD5441A_SetVotage(DAC_DeviceTypes_t *dev, double voltage)
{
	double tempFloat  = 0; /*!< Variable used for storring float values. */
	/*! Check if parameter is valid */
		if((voltage >= 0) && (voltage <= MAX_OUT_VOLTAGE(mVref)))
	    {
		  /*! Find the binary value corresponding to the output resistance */
		  tempFloat = (voltage / (double)mVref) * 65536;
		  mRegValue = (unsigned short) tempFloat;
		  /*! Round up the value */
		  if(tempFloat - mRegValue >= 0.5)
		  {
			  mRegValue ++;
		  }
		}
		else if(voltage < 0)
		{
			mRegValue = 0;
		}
		else
		{
			mRegValue = 0xFFFF;
		}
		AD5541A_SetRegisterValue(dev, mRegValue);
		mOutputVoltage = mVref * ((double) mRegValue / 65536);

		return(mOutputVoltage);
}


