#ifndef CM_BUFFER_H
#define CM_BUFFER_H

#include <stdio.h>

#include "cm_string.h"

struct cm_buffer {
  char* data;
  size_t size;
  size_t capacity;
};

#endif