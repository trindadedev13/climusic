#ifndef CM_GUI_H
#define CM_GUI_H

#include <stdarg.h>

#include "cm_colors.h"
#include "cm_screen.h"
#include "cm_string.h"
#include "cm_vec.h"

#define CM_GUI_FILL_CHAR "■"

// Note: funcions with '_d' sufix draw things dynamically
// Note: funcions with '_ic' sufix draw 'in cursor' position

// Draw a rectangle with with defined color in specify x, y, width, height
void cm_gui_draw_rect(cm_color_pair color,
                      struct cm_vec2,
                      int width,
                      int height);

// Draw a text with defined color in specific x & y
void cm_gui_draw_text(cm_color_pair color,
                      struct cm_vec2,
                      const cm_string fmt,
                      ...);

// Draw a text in dynamic y & x
void cm_gui_draw_text_d(struct cm_screen*,
                        cm_color_pair color,
                        const cm_string fmt,
                        ...);

// Draw a text in dynamic y & x with y + 1
void cm_gui_draw_textln_d(struct cm_screen*,
                          cm_color_pair color,
                          const cm_string fmt,
                          ...);

// Draw a text in cursor position
void cm_gui_draw_text_ic(cm_color_pair color, const cm_string fmt, ...);

// Draw a error text in specific x & y with prefix
void cm_gui_draw_error(struct cm_vec2, const cm_string prefix);

// Draw a error text in specific x & y with prefix
void cm_gui_draw_error_d(struct cm_screen*, const cm_string prefix);

#endif