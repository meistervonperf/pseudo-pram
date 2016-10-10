/*
 * ppram_file.c
 *
 * Copyright (C) 2016
 * Takashi Horikawa (meistervonperf@gmail.com)
 *
 */

#include "ppram_file.h"
#include <linux/mm.h>
#include <linux/fs.h> 	     /* file stuff */
#include <linux/kernel.h>    /* printk() */
#include <linux/errno.h>     /* error codes */
#include <linux/module.h>  /* THIS_MODULE */
#include <linux/cdev.h>      /* char device stuff */
#include <asm/uaccess.h>  /* copy_to_user() */
#include <linux/device.h>  /* class_[create|destroy](), device_[create|destroy]() */


static ssize_t ppram_read(
                           struct file *file_ptr
                        , char __user *user_buffer
                        , size_t count
                        , loff_t *possition)
{
   if( *possition >= getPpramSize() )
      return 0;

   if( *possition + count > getPpramSize() )
      count = getPpramSize() - *possition;

   if( copy_to_user(user_buffer, getPpramAddress() + *possition, count) != 0 )
      return -EFAULT;   

   *possition += count;
   return count;
}


static const struct vm_operations_struct ppram_file_vm_ops = {
};

int ppram_mmap(
	        struct file * file_ptr
	     , struct vm_area_struct * vma)
{	
  ssize_t usize = vma->vm_end - vma->vm_start;

  if (usize > getPpramSize()) return -EAGAIN;

  file_accessed(file_ptr);
  vma->vm_ops = &ppram_file_vm_ops;
  if (remap_vmalloc_range(vma, getPpramAddress(), 0)) {
    return -EAGAIN;
  }
  return 0;
}


static struct file_operations ppram_fops = 
{
   .owner   = THIS_MODULE,
   .read    = ppram_read,
   .mmap    = ppram_mmap,
};


static dev_t first;         // Global variable for the first device number
static struct cdev c_dev;     // Global variable for the character device structure
static struct class *cl;     // Global variable for the device class
static const char device_name[] = "pseudo-pram";

int register_device(void)
{
  int result;
  
  result = alloc_chrdev_region( &first, 0, 1, device_name );
  
  if( 0 > result )
    {
      printk( KERN_ALERT "Device Registration failed\n" );
      return -1;
    }

  printk( KERN_ALERT "Major number is: %d\n", first >> 20);

  if ( (cl = class_create( THIS_MODULE, "chardev" ) ) == NULL )
    {
      printk( KERN_ALERT "Class creation failed\n" );
      unregister_chrdev_region( first, 1 );
      return -1;
    }
  
  if( device_create( cl, NULL, first, NULL, device_name ) == NULL )
    {
      printk( KERN_ALERT "Device creation failed\n" );
      class_destroy(cl);
      unregister_chrdev_region( first, 1 );
      return -1;
    }
  
  cdev_init( &c_dev, &ppram_fops );
  
  if( cdev_add( &c_dev, first, 1 ) == -1)
    {
      printk( KERN_ALERT "Device addition failed\n" );
      device_destroy( cl, first );
      class_destroy( cl );
      unregister_chrdev_region( first, 1 );
      return -1;
    }
  
  return 0;
}

void unregister_device(void)
{
  cdev_del( &c_dev );
  device_destroy( cl, first );
  class_destroy( cl );
  unregister_chrdev_region( first, 1 );
  
  printk(KERN_ALERT "Device unregistered\n");
}
