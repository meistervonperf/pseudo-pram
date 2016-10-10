/*
 * ppram_file.h
 *
 * Copyright (C) 2016
 * Takashi Horikawa (meistervonperf@gmail.com)
 *
 */

#ifndef PPRAM_FILE_H_
#define PPRAM_FILE_H_
#include <linux/compiler.h> /* __must_check */
#include <linux/types.h>    /* ssize_t */

__must_check int register_device(void); /* returns 0 when the processing result is OK */
void 			 unregister_device(void); 

void			 *getPpramAddress(void);
ssize_t			 getPpramSize(void);

#endif //PPRAM_FILE_H_
