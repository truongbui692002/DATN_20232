/*
 * dac_types.h
 *
 *  	Created on: Mar 17, 2024
 *      Author: TruongBVD
 */
#ifndef _DAC_TYPES_H_
#define _DAC_TYPES_H_

#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>


struct DAC_DeviceTypes_s;
struct DAC_Function_s;

typedef void (*DAC_Handler_t)(struct DAC_DeviceTypes_s *dev);

typedef struct DAC_DeviceTypes_s
{
    struct DAC_Function_s *func;

    DAC_Handler_t handleSent;
    DAC_Handler_t handleError;
    DAC_Handler_t handleReceived;
    DAC_Handler_t handleReceiveTimeout;
    DAC_Handler_t handleReceiveFailed;

} DAC_DeviceTypes_t;

typedef enum
{
    DACSpiSpeedLow,
    DACSpiSpeedHigh
} DACSpiSpeed_t;


typedef struct DAC_Function_s
{
    /**
     * Function that activates the chip-select, sends header, read data and
     * disable the chip-select.
     */
    void (*spiRead)(DAC_DeviceTypes_t *dev, const void *header, size_t headerLength,
                    void *data, size_t dataLength);

    /**
     * Function that activates the chip-select, sends header, sends data and
     * disable the chip-select.
     */
    void (*spiWrite)(DAC_DeviceTypes_t *dev,
                     const void *data, size_t dataLength);

    /**
     * Sets the SPI bus speed. Take as argument:
     *	 - DACSpiSpeedLow: <= 4MHz
     *	 - DACSpiSpeedHigh: <= 20MHz
     */
    void (*spiSetSpeed)(DAC_DeviceTypes_t *dev, DACSpiSpeed_t speed);

    /**
     * Waits at least 'delay' miliseconds.
     */
    void (*delayms)(DAC_DeviceTypes_t *dev, unsigned int delay);

    /**
     * Resets the DAC1000 by pulling the reset pin low and then releasing it.
     * This function is optional, if not set softreset via SPI will be used.
     */
    void (*reset)(DAC_DeviceTypes_t *dev);
} DAC_Function_t;

#endif
