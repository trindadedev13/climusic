#include "cm_screen.h"

#include <stdio.h>
#include <stdlib.h>

#include <ncurses.h>

#include "cm_colors.h"
#include "cm_vec.h"

// The screen variable, IT SHOULD BE INITIALIZED IN MAIN!
struct cm_screen* defcms = NULL;

struct cm_screen* cm_screen_create() {
  struct cm_screen* screen;
  screen = malloc(sizeof(struct cm_screen));
  if (!screen) {
    perror("Failed to allocate memory for screen");
    return NULL;
  }
  screen->yMax = 0;
  screen->xMax = 0;

  screen->cursor = malloc(sizeof(struct cm_cursor));
  if (!screen->cursor) {
    perror("Failed to allocate memory for screen->cursor");
    return NULL;
  }
  screen->cursor->y = 0;
  screen->cursor->x = 0;

  screen->print_cursor = malloc(sizeof(struct cm_cursor));
  if (!screen->print_cursor) {
    perror("Failed to allocate memory for screen->cursor");
    return NULL;
  }
  screen->print_cursor->y = 0;
  screen->print_cursor->x = 0;

  initscr();                 // init ncurses
  cm_screen_disable_echo();  // disable input
  cbreak();                  // send all char automatically without ENTER
  keypad(stdscr, TRUE);      // allow especial chars
  cm_colors_init(CM_COLOR_BACKGROUND_ID);  // init colors and background color
  cm_colors_set_background_color(
      CM_COLOR_BACKGROUND_PAIR);  // defines background color
  cm_colors_enable_color(
      CM_COLOR_PRIMARY_PAIR);  // enable primary color for all
  curs_set(1);                 // sets cursor visiblity to visible

  // gets terminal scr maxX & maxY to variables
  getmaxyx(stdscr, screen->yMax, screen->xMax);

  cm_screen_clear();

  return screen;
}

void cm_screen_clear_line(struct cm_screen* screen, int y) {
  int oy = screen->cursor->y;
  int ox = screen->cursor->x;
  cm_screen_move(cm_vec2(y, 0));
  clrtoeol();
  cm_screen_move(cm_vec2(oy, 0));
  cm_screen_refresh();
}

void cm_screen_end(struct cm_screen* screen) {
  cm_colors_disable_color(CM_COLOR_PRIMARY_PAIR);  // disable color
  cm_screen_end_ncurses();
  free(screen->cursor);
  free(screen->print_cursor);
  free(screen);
}