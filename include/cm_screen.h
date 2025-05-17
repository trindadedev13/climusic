#ifndef CM_SCREEN_H
#define CM_SCREEN_H

#include <ncurses.h>

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

// Clear Screen.
void cm_screen_clear();

// Refresh Screen.
void cm_screen_refresh();

// Move cursor to cm_vec2(y, x).
void cm_screen_move(struct cm_vec2);

// Returns the char input of terminal (ncurses).
int cm_screen_getchar();

// Update. Should be called in loop.
void cm_screen_update(struct cm_screen*);

// Enable user input.
void cm_screen_enable_echo();

// Disable user input.
void cm_screen_disable_echo();

// Read echo string to dest.
void cm_screen_get_str(cm_string dest);

// Clear an whole line.
void cm_screen_clear_line(struct cm_screen* screen, int y);

// Just finishes endcurses.
void cm_screen_end_ncurses();

// Finishes the screen and desallocates memory.
void cm_screen_end(struct cm_screen*);

#endif