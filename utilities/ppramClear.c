/*
 * ppramClear.c
 *
 * Copyright (C) 2016
 * Takashi Horikawa (meistervonperf@gmail.com)
 *
 */

#include <stdio.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/mman.h>
#include "ppram.h"

#define SEGSIZE (16 * 1024 * 1024)

main(int argc, char *argv[])
{
  int fd;
  int l, i, c;
  long n = 4, size;
  long *area;

  if (argc > 1) n = atoi(argv[1]);

  fd = open( DeviceFileName, O_RDWR );
  if (fd < 0) { perror("open"); }

  size = n * SEGSIZE;
  area = (long *) mmap( NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
  if (area == NULL) { perror("mmap"); }

  for( i = 0; i < (size / sizeof(long)); i++) {
    *(area + i) = 0;
  }
}
