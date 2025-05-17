#include "cm_vec.h"

struct cm_vec2 cm_vec_make_vec2(int y, int x) {
  struct cm_vec2 vec;
  vec.x = x;
  vec.y = y;
  return vec;
}