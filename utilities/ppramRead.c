/*
 * ppramRead.c
 *
 * Copyright (C) 2016
 * Takashi Horikawa (meistervonperf@gmail.com)
 *
 */

#include <stdio.h>
#include <fcntl.h>
#include <errno.h>
#include "ppram.h"

main()
{
  int fd;
  int s, l = 0, i;
  char buf[16];

  fd = open( DeviceFileName, O_RDWR );
  if (fd < 0) { perror("open"); }

  while( (s = read( fd, buf, 16 )) != EOF ) {
    printf( "%08x:", l++ * 16 );
    for ( i = 0; i < s; i++) {
      printf( " %02x", buf[i] & 0xff );
    }
    printf( "\n", buf[i] );
  }
}
