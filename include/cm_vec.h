#ifndef CM_VEC_H
#define CM_VEC_H

#include "cm_inline.h"

struct cm_vec2 {
  int x;
  int y;
};

#define cm_vec2(y, x) cm_vec_make_vec2(y, x)

CM_INLINE static struct cm_vec2 cm_vec_make_vec2(int y, int x) {
  struct cm_vec2 vec;
  vec.x = x;
  vec.y = y;
  return vec;
}

#endif