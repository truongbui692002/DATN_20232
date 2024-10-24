/*
 * log.h
 *
 *  Created on: Mar 21, 2024
 *      Author: DuyTruong
 */

#ifndef _LOG_H_
#define _LOG_H_

#include <stdio.h>
#include <string.h>
#include "stm32f1xx.h"

/* Console color */
#define KNRM  "\x1B[0m"
#define KRED  "\x1B[31m"
#define KGRN  "\x1B[32m"
#define KYEL  "\x1B[33m"
#define KBLU  "\x1B[34m"
#define KMAG  "\x1B[35m"
#define KCYN  "\x1B[36m"
#define KWHT  "\x1B[37m"

extern char * time_str(void);

#define LOG_EN 	1
#if LOG_EN == 1

#define ___FILENAME___ (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__)

#define log_print(type, fmt, args...) do { \
		printf("%s[%08ld][%s:%03d] " fmt KNRM "\r\n", type, HAL_GetTick(), ___FILENAME___,  __LINE__ ,  ##args); \
} while (0)


#define logi(fmt, args...) log_print("\x1B[32m[I]", fmt, ##args)
#define logw(fmt, args...) log_print("\x1B[33m[W]", fmt, ##args)
#define loge(fmt, args...) log_print("\x1B[31m[E]", fmt, ##args)

#define Logi logi
#define Loge loge
#define Logw logw

#else
#define logi(...)
#define loge(...)
#define logd(...)
#define Logi(...)
#define Loge(...)
#define Logd(...)
#endif
#endif /* INC_LOG_H_ */

