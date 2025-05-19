#ifndef CM_SCREEN_H
#define CM_SCREEN_H

#include <ncurses.h>

#include "cm_inline.h"
#include "cm_string.h"
#include "cm_vec.h"

#define CM_KEY_UP KEY_UP
#define CM_KEY_LEFT KEY_LEFT
#define CM_KEY_DOWN KEY_DOWN
#define CM_KEY_RIGHT KEY_RIGHT

struct cm_cursor {
  int x;
  int y;
};

struct cm_screen {
  // the terminal cursor info
  struct cm_cursor* cursor;
  // the cursor for prints (DONT USE IT TO GET CURSOR POSITION)
  struct cm_cursor* print_cursor;
  // the max x and y (like width and height)
  int xMax;
  int yMax;
};

// The screen variable, IT SHOULD BE INITIALIZED IN MAIN!
// Use this variable for any screen action
extern struct cm_screen* defcms;

// Creates the screen
// Allocates, initializes values, configures ncurses
struct cm_screen* cm_screen_create();

// Update. Should be called in loop.
CM_INLINE static void cm_screen_update(struct cm_screen* screen) {
  getyx(stdscr, screen->cursor->y, screen->cursor->x);
}

// Clear Screen.
CM_INLINE static void cm_screen_clear() {
  clear();
}

// Refresh Screen.
CM_INLINE static void cm_screen_refresh() {
  refresh();
}

// Move cursor to cm_vec2(y, x).
CM_INLINE static void cm_screen_move(struct cm_vec2 vec) {
  move(vec.y, vec.x);
}

// Returns the char input of terminal (ncurses).
CM_INLINE static int cm_screen_getchar() {
  return getch();
}

// Enable user input.
CM_INLINE static void cm_screen_enable_echo() {
  echo();
}

// Disable user input.
CM_INLINE static void cm_screen_disable_echo() {
  noecho();
}

// Read echo string to dest.
CM_INLINE static void cm_screen_get_str(cm_string dest) {
  getstr(dest);
}

// Just finishes endcurses.
CM_INLINE static void cm_screen_end_ncurses() {
  endwin();
}

// Clear an whole line.
void cm_screen_clear_line(struct cm_screen* screen, int y);

// Finishes the screen and desallocates memory.
void cm_screen_end(struct cm_screen*);

#endif