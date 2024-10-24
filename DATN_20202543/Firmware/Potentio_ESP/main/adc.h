#ifndef __ADC_H
#define __ADC_H

#include "header.h"

#define ADC2_UNIT                   ADC_UNIT_2
#define ADC_ATTEN                   ADC_ATTEN_DB_11
#define ADC_BIT_WIDTH               SOC_ADC_DIGI_MAX_BITWIDTH
#define ADC2_CHAN3                  ADC_CHANNEL_3

static bool adc_calibration_init(adc_unit_t unit, adc_atten_t atten, adc_cali_handle_t *out_handle);
static void adc_calibration_deinit(adc_cali_handle_t handle);
void adc_task(void *param);

#endif
