#ifndef __SDCARD_H
#define __SDCARD_H

#include "header.h"

#define MAX_CHAR_SIZE    512

#define MOUNT_POINT "/sdcard"

void s_example_write_file(const char *path, char *data);
void s_example_read_file(const char *path);

void sdcard_init(void);
void sdcard_task(void *param);

#endif 
