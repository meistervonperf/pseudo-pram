/*
 * body.c
 *
 * Copyright (C) 2016
 * Takashi Horikawa (meistervonperf@gmail.com)
 *
 */

#include "ppram_file.h"
#include <linux/init.h>
#include <linux/module.h>
#include <linux/vmalloc.h>
#include <asm/cacheflush.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Takashi Horikawa");

static int num = 4;
static int type = 0;
module_param(num, int, S_IRUGO);
module_param(type, int, S_IRUGO);

static ssize_t size = (16 * 1024 * 1024);
static void *address;
static bool changed = false;

void *getPpramAddress(void) { return address; }
ssize_t getPpramSize(void) { return num * size; }

static int __init init_ppram(void)
{
  int result;

  address = vmalloc_user(num * size);
  if (address == NULL) return -ENOMEM;

  if (type == 0)
    set_memory_wc(address - NULL, (num * size) >> PAGE_SHIFT);
  else
    set_memory_uc(address - NULL, (num * size) >> PAGE_SHIFT);
  changed = true;

  if ((result = register_device()) != 0) {
    /* register_device() failed, thus cancel the memory setup */
    if (changed) set_memory_wb(address - NULL, (num * size) >> PAGE_SHIFT);
    vfree(address);
  }
  return result;
}

static void __exit exit_ppram(void)
{
  unregister_device();
  if (address) {
    if (changed) set_memory_wb(address - NULL, (num * size) >> PAGE_SHIFT);
    vfree(address);
  }
}

module_init(init_ppram)
module_exit(exit_ppram)
