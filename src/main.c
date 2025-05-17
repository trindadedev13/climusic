#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <pthread.h>

#include "cm_bool.h"
#include "cm_colors.h"
#include "cm_file.h"
#include "cm_gui.h"
#include "cm_screen.h"
#include "cm_sound.h"
#include "cm_vec.h"

#define CM_MUSIC_INITIAL_CAPACITY 1
#define CM_MUSIC_PATH "/sdcard/climusics/"

struct cm_music {
  char* name;
  struct cm_vec2 position;
};

struct cm_state {
  cm_bool isRunning;
  struct cm_music* musics;
  struct cm_music selected_music;
  int musics_amount;
  int musics_capacity;
};

struct cm_state cmstate;

static void cm_state_close() {
  for (int i = 0; i < cmstate.musics_amount; i++) {
    free(cmstate.musics[i].name);
  }
  free(cmstate.musics);
}

#define cm_music(name, position) cm_music_make(name, position);

static struct cm_music cm_music_make(char* name, struct cm_vec2 position) {
  struct cm_music cmm;
  cmm.name = name;
  cmm.position = position;
  return cmm;
}

// Music player thread
void* cm_music_player_thread(void* arg) {
  const char* music_name = (const char*)arg;
  char* full = strdup(CM_MUSIC_PATH);
  strcat(full, music_name);
  // cm_gui_draw_text(CM_COLOR_PRIMARY_PAIR, cm_vec2(1, 0), "Path %s", full);
  cm_gui_draw_text(CM_COLOR_PRIMARY_PAIR, cm_vec2(0, 0), "Playing %s now",
                   music_name);
  struct cm_sound s = cm_sound(full);
  if (!cm_sound_init())
    cm_gui_draw_error_d(defcms, "Failed to initialize sound");
  if (!cm_sound_play(&s))
    cm_gui_draw_error_d(defcms, "Failed to play sound");
  while (cm_sound_isplaying()) {
    cm_sound_delay(100);
  }
  cm_sound_end(&s);
  return NULL;
}

static void cm_music_print(struct cm_music m) {
  cm_gui_draw_textln_d(defcms, CM_COLOR_PRIMARY_PAIR, "Name: %s, X: %d, Y: %d",
                       m.name, m.position.x, m.position.y);
}

int main() {
  cmstate.isRunning = true;
  cmstate.musics_amount = 0;
  cmstate.musics_capacity = CM_MUSIC_INITIAL_CAPACITY;
  cmstate.musics = malloc(sizeof(struct cm_music) * CM_MUSIC_INITIAL_CAPACITY);
  if (!cmstate.musics) {
    cm_gui_draw_error_d(defcms, "Failed to allocate memory for cmstate.musics");
  }

  // you can change it if you want
  struct cm_file_list_dir_data* tld = cm_file_list_dir(CM_MUSIC_PATH);

  defcms = cm_screen_create();

  if (!defcms) {
    cm_gui_draw_error_d(defcms, "Failed to create screen");
    cm_screen_end_ncurses();  // just end ncurses to avoid bugs in terminal
    return EXIT_FAILURE;
  }

  cm_gui_draw_text(CM_COLOR_PRIMARY_PAIR, cm_vec2(defcms->yMax - 1, 0),
                   "Press 'q' to quit.");
  cm_screen_clear_line(defcms, 0);

  if (tld && tld->len > 0) {
    int max_items = defcms->yMax - 2;
    for (int i = 0; i < tld->len && i < max_items; i++) {
      char* name = strdup(tld->dirs[i].name);
      int y = defcms->yMax / 2 + i;
      int x = defcms->xMax / 2 - 8;
      if (cmstate.musics_amount >= cmstate.musics_capacity) {
        cmstate.musics_capacity = cmstate.musics_amount + 1;
        cmstate.musics = realloc(
            cmstate.musics, sizeof(struct cm_music) * cmstate.musics_capacity);
      }
      cmstate.musics[cmstate.musics_amount++] = cm_music(name, cm_vec2(y, x));
      cm_gui_draw_text(CM_COLOR_PRIMARY_PAIR, cm_vec2(y, x), "%s", name);
    }
    cmstate.selected_music = cmstate.musics[0];
    cm_screen_move(cmstate.musics[0].position);
  } else {
    cm_gui_draw_text(CM_COLOR_PRIMARY_PAIR,
                     cm_vec2(defcms->yMax / 2, defcms->xMax / 2 - 5),
                     "No files found");
    cm_screen_move(cm_vec2(0, 0));  // move cursor to x:0 y:0
  }

  pthread_t music_thread;

  // handle chars
  // when 'q' finish problem
  int ch;
  while (cmstate.isRunning) {
    ch = cm_screen_getchar();

    cm_screen_update(defcms);
    if (!cm_sound_isplaying()) {
      cm_screen_clear_line(defcms, 0);
    }
    if (ch == 'w' || ch == CM_KEY_UP) {
      if (defcms->cursor->y != 0 &&
          !(defcms->cursor->y <= cmstate.musics[0].position.y)) {
        cm_screen_move(cm_vec2(--defcms->cursor->y, defcms->cursor->x));
        for (int i = 0; i < cmstate.musics_amount; i++) {
          struct cm_music cmm = cmstate.musics[i];
          if (cmm.position.y == defcms->cursor->y) {
            cmstate.selected_music = cmm;
          }
        }
      }
    } else if (ch == 's' || ch == CM_KEY_DOWN) {
      if (defcms->cursor->y != defcms->yMax &&
          !(defcms->cursor->y >=
            cmstate.musics[cmstate.musics_amount - 1].position.y)) {
        cm_screen_move(cm_vec2(++defcms->cursor->y, defcms->cursor->x));
        for (int i = 0; i < cmstate.musics_amount; i++) {
          struct cm_music cmm = cmstate.musics[i];
          if (cmm.position.y == defcms->cursor->y) {
            cmstate.selected_music = cmm;
          }
        }
      }
    } else if (ch == '\n') {
      if (!cm_sound_isplaying()) {
        cm_gui_draw_text(CM_COLOR_PRIMARY_PAIR, cm_vec2(defcms->yMax - 1, 0),
                         "Press 'q' to pause.");
        pthread_create(&music_thread, NULL, cm_music_player_thread,
                       (void*)cmstate.selected_music.name);
        pthread_detach(music_thread);
      }
    } else if (ch == 'q') {
      cm_screen_clear_line(defcms, 0);
      if (cm_sound_isplaying()) {
        cm_gui_draw_text(CM_COLOR_PRIMARY_PAIR, cm_vec2(defcms->yMax - 1, 0),
                         "Press 'q' to quit.");
        cm_sound_stop();
        cmstate.selected_music = cmstate.musics[0];
        cm_screen_move(cmstate.musics[0].position);
      } else {
        cmstate.isRunning = false;
      }
    }
  }

  cm_file_list_dir_close(tld);
  cm_screen_end(defcms);
  cm_state_close();
  return EXIT_SUCCESS;
}