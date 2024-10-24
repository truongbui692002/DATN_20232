/*
 * common_user.c
 *
 *  Created on: May 30, 2024
 *      Author: DuyTruong
 */

#include <Third_lib/cJSON.h>
#include "main.h"
#include "common_user.h"

bool uart_parse_data(char *uart_data, configInfo_obj_t *configData_obj)
{
	bool ret = 0;
	cJSON *root = cJSON_Parse(uart_data);
	if (root == NULL)
		return ret;
	cJSON *elem = NULL;
	int config_dev = cJSON_GetObjectItem(root, "config_dev")->valueint;
	if (!(config_dev == 1))
	{
		return ret;
	}
	cJSON_ArrayForEach(elem, root)
	{
		ret = 1;

		if (elem->string)
		{
			char *elem_str = elem->string;
			if (!strcmp(elem_str, "config_dev"))
			{
				configData_obj->config_device = elem->valueint;
			}
			else if (!strcmp(elem_str, "mode"))
			{
				strcpy((char *)configData_obj->mode_device, elem->valuestring);
			}
			else if (!strcmp(elem_str, "res"))
			{
				strcpy((char *)configData_obj->res_device, elem->valuestring);
			}
			else if (!strcmp(elem_str, "vol_start"))
			{
				configData_obj->start_vol = elem->valuedouble;
			}
			else if (!strcmp(elem_str, "vol_end"))
			{
				configData_obj->end_vol = elem->valuedouble;
			}
			else if (!strcmp(elem_str, "scan_rate"))
			{
				configData_obj->scan_rate = elem->valueint;
			}
			else if (!strcmp(elem_str, "num_cycle"))
			{
				configData_obj->num_cycle = elem->valueint;
			}
		}
	}
	return ret;
}


