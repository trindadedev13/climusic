#ifndef CM_STRING_H
#define CM_STRING_H

#include "cm_bool.h"

typedef char* cm_string;

cm_string cm_string_encode_spaces(const cm_string input);

cm_bool cm_string_ends_with(const cm_string input, const cm_string end);

#endif