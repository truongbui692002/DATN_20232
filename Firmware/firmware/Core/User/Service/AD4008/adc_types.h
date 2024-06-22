/*
 * adc_types.h
 *
 *  	Created on: Mar 17, 2024
 *      Author: TruongBVD
 */
#ifndef _ADC_TYPES_H_
#define _ADC_TYPES_H_

#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>


struct ADC_DeviceTypes_s;
struct ADC_Function_s;

typedef void (*ADC_Handler_t)(struct ADC_DeviceTypes_s *dev);

typedef struct ADC_DeviceTypes_s
{
    struct ADC_Function_s *func;

    ADC_Handler_t handleSent;
    ADC_Handler_t handleError;
    ADC_Handler_t handleReceived;
    ADC_Handler_t handleReceiveTimeout;
    ADC_Handler_t handleReceiveFailed;

} ADC_DeviceTypes_t;

typedef enum
{
    ADCSpiSpeedLow,
    ADCSpiSpeedHigh
} ADCSpiSpeed_t;


typedef struct ADC_Function_s
{
    /**
     * Function that activates the chip-select, sends header, read data and
     * disable the chip-select.
     */
    void (*spiRead)(ADC_DeviceTypes_t *dev,
                    void *data, size_t dataLength);

    /**
     * Function that activates the chip-select, sends header, sends data and
     * disable the chip-select.
     */
    void (*spiWrite)(ADC_DeviceTypes_t *dev,
                     const void *data, size_t dataLength);

    /**
     * Sets the SPI bus speed. Take as argument:
     *	 - ADCSpiSpeedLow: <= 4MHz
     *	 - ADCSpiSpeedHigh: <= 20MHz
     */
    void (*spiSetSpeed)(ADC_DeviceTypes_t *dev, ADCSpiSpeed_t speed);

    /**
     * Waits at least 'delay' miliseconds.
     */
    void (*delayms)(ADC_DeviceTypes_t *dev, unsigned int delay);

    /**
     * Resets the ADC1000 by pulling the reset pin low and then releasing it.
     * This function is optional, if not set softreset via SPI will be used.
     */
    void (*reset)(ADC_DeviceTypes_t *dev);
} ADC_Function_t;

#endif
