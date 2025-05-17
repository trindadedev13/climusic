#include "cm_string.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cm_bool.h"
#include "cm_string.h"

cm_string cm_string_encode_spaces(const cm_string input) {
  size_t len = strlen(input);
  cm_string output = malloc(len * 3 + 1);
  if (!output)
    return NULL;
  cm_string p = output;
  for (size_t i = 0; i < len; ++i) {
    if (input[i] == ' ') {
      strcpy(p, "%20");
      p += 3;
    } else {
      *p++ = input[i];
    }
  }
  *p = '\0';
  return output;
}

cm_bool cm_string_ends_with(const cm_string input, const cm_string end) {
  if (!input || !end)
    return false;
  size_t ilen = strlen(input);
  size_t elen = strlen(end);
  if (ilen > elen) {
    return strncmp(input + ilen - elen, end, elen) == 0;
  }
  return false;
}