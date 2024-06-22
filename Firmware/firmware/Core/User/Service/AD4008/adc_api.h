#ifndef _ADC_API_H_
#define _ADC_API_H_

#include <adc_func.h>
#include <adc_types.h>
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
#define AD400X_READ_COMMAND		0x54
#define AD400X_WRITE_COMMAND	0x14
#define AD400X_RESERVED_MSK		0xE0

#define AD400X_TURBO_MODE(x)		(((x) & 0x1) << 1)
#define AD400X_HIGH_Z_MODE(x)		(((x) & 0x1) << 2)
#define AD400X_SPAN_COMPRESSION(x)	(((x) & 0x1) << 3)
#define AD400X_EN_STATUS_BITS(x)	(((x) & 0x1) << 4)

#define NUM_SAMPLES 10 // Số lượng mẫu sử dụng để tính trung bình

enum ad400x_supported_dev_ids {
	ID_AD4003,
	ID_AD4007,
	ID_AD4008,
	ID_AD4011,
	ID_AD4020,
	ID_ADAQ4003,
};

extern const uint16_t ad400x_device_resol[];

struct ad400x_dev {
	/* Device Settings */
	enum ad400x_supported_dev_ids dev_id;
};

struct ad400x_init_param {
	/* Device Settings */
	enum ad400x_supported_dev_ids dev_id;
	bool turbo_mode;
	bool high_z_mode;
	bool span_compression;
	bool en_status_bits;
};


void ADCInit(ADC_DeviceTypes_t *dev);
uint16_t AD4008_single_conversion(ADC_DeviceTypes_t *dev, uint16_t adc_data);



#endif
