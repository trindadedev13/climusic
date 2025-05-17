#include "cm_gui.h"

#include <errno.h>
#include <ncurses.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>

#include "cm_colors.h"
#include "cm_screen.h"
#include "cm_string.h"
#include "cm_vec.h"

void cm_gui_draw_rect(cm_color_pair color,
                      struct cm_vec2 vec,
                      int width,
                      int height) {
  if (width < 2 || height < 2)
    return;

  // enable color
  cm_colors_enable_color(color);

  // top left corner
  mvaddch(vec.y, vec.x, ACS_ULCORNER);

  // top right corner
  mvaddch(vec.y, vec.x + width - 1, ACS_URCORNER);

  // bottom left corner
  mvaddch(vec.y + height - 1, vec.x, ACS_LLCORNER);

  // bottom right corner
  mvaddch(vec.y + height - 1, vec.x + width - 1, ACS_LRCORNER);

  // horizontal lines
  for (int i = 1; i < width - 1; i++) {
    mvaddch(vec.y, vec.x + i, ACS_HLINE);
    mvaddch(vec.y + height - 1, vec.x + i, ACS_HLINE);
  }

  // vertical lines
  for (int i = 1; i < height - 1; i++) {
    mvaddch(vec.y + i, vec.x, ACS_VLINE);
    mvaddch(vec.y + i, vec.x + width - 1, ACS_VLINE);
  }

  // fill rect
  for (int y = 1; y < height - 1; y++) {
    for (int x = 1; x < width - 1; x++) {
      mvaddch(vec.y + y, vec.x + x, ACS_BLOCK);
    }
  }

  // disable color
  cm_colors_disable_color(color);
}

void cm_gui_draw_text(cm_color_pair color,
                      struct cm_vec2 vec,
                      const cm_string fmt,
                      ...) {
  char buffer[1024];

  va_list args;
  va_start(args, fmt);
  vsnprintf(buffer, sizeof(buffer), fmt, args);
  va_end(args);

  cm_colors_enable_color(color);
  mvprintw(vec.y, vec.x, "%s", buffer);
  cm_colors_disable_color(color);
  cm_screen_refresh();
}

void cm_gui_draw_text_d(struct cm_screen* screen,
                        cm_color_pair color,
                        const cm_string fmt,
                        ...) {
  char buffer[1024];

  va_list args;
  va_start(args, fmt);
  vsnprintf(buffer, sizeof(buffer), fmt, args);
  va_end(args);

  cm_gui_draw_text(color,
                   cm_vec2(screen->print_cursor->y, screen->print_cursor->x),
                   "%s", buffer);
  screen->print_cursor->x = screen->print_cursor->x + strlen(buffer);
}

void cm_gui_draw_textln_d(struct cm_screen* screen,
                          cm_color_pair color,
                          const cm_string fmt,
                          ...) {
  char buffer[1024];

  va_list args;
  va_start(args, fmt);
  vsnprintf(buffer, sizeof(buffer), fmt, args);
  va_end(args);

  cm_gui_draw_text(color,
                   cm_vec2(screen->print_cursor->y++, screen->print_cursor->x),
                   "%s", buffer);
  // screen->print_cursor->x = screen->print_cursor->x + strlen(buffer);
}

void cm_gui_draw_text_ic(cm_color_pair color, const cm_string fmt, ...) {
  char buffer[1024];

  va_list args;
  va_start(args, fmt);
  vsnprintf(buffer, sizeof(buffer), fmt, args);
  va_end(args);

  cm_colors_enable_color(color);
  printw("%s", buffer);
  cm_colors_disable_color(color);
  cm_screen_refresh();
}

void cm_gui_draw_error(struct cm_vec2 vec, const cm_string prefix) {
  const cm_string errmsg = strerror(errno);
  cm_gui_draw_text(CM_COLOR_RED_PAIR, vec, "%s : %s\n", prefix, errmsg);
}

void cm_gui_draw_error_d(struct cm_screen* screen, const cm_string prefix) {
  const cm_string errmsg = strerror(errno);
  cm_gui_draw_text_d(screen, CM_COLOR_RED_PAIR, "%s : %s\n", prefix, errmsg);
}