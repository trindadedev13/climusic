#ifndef CM_VEC_H
#define CM_VEC_H

struct cm_vec2 {
  int x;
  int y;
};

#define cm_vec2(y, x) cm_vec_make_vec2(y, x)

struct cm_vec2 cm_vec_make_vec2(int y, int x);

#endif