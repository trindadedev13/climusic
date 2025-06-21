#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <pthread.h>

#include "cm_bool.h"
#include "cm_colors.h"
#include "cm_env.h"
#include "cm_file.h"
#include "cm_gui.h"
#include "cm_inline.h"
#include "cm_online.h"
#include "cm_screen.h"
#include "cm_sound.h"
#include "cm_vec.h"

#define CM_MODE_INPUT 1
#define CM_MODE_SELECT 0

#define CM_MUSIC_INITIAL_CAPACITY 1

struct cm_music {
  cm_string name;
  struct cm_vec2 position;
};

struct cm_state {
  cm_bool isRunning;
  int mode;
  struct cm_music* musics;
  struct cm_music selected_music;
  int musics_amount;
  int musics_capacity;
};

static void cm_state_init(struct cm_state* cmstate) {
  cmstate->isRunning = true;
  cmstate->mode = CM_MODE_SELECT;
  cmstate->musics_amount = 0;
  cmstate->musics_capacity = CM_MUSIC_INITIAL_CAPACITY;
  cmstate->musics = malloc(sizeof(struct cm_music) * CM_MUSIC_INITIAL_CAPACITY);
  if (!cmstate->musics) {
    cm_gui_draw_error_d(defcms,
                        "Failed to allocate memory for cmstate->musics");
  }
}

static void cm_state_close(struct cm_state* cmstate) {
  for (int i = 0; i < cmstate->musics_amount; i++) {
    free(cmstate->musics[i].name);
  }
  free(cmstate->musics);
}

#define cm_music(name, position) cm_music_make(name, position);

CM_INLINE static struct cm_music cm_music_make(cm_string name,
                                               struct cm_vec2 position) {
  struct cm_music cmm;
  cmm.name = name;
  cmm.position = position;
  return cmm;
}

// Music player thread
// Play the music from name (arg)
void* cm_music_player_thread(void* arg) {
  const cm_string music_name = (const cm_string)arg;

  cm_string music_path = strdup(CM_ENV_PATH);
  strcat(music_path, music_name);

  // Draw "Playing *muusicName Now"
  cm_gui_draw_text(CM_COLOR_PRIMARY_PAIR, cm_vec2(0, 0), "Playing %s now",
                   music_name);

  // struct to store sound data
  struct cm_sound cms;
  cms = cm_sound(music_path);

  // Initializes SDL Sound
  if (!cm_sound_init())
    cm_gui_draw_error_d(defcms, "Failed to initialize sound");

  // Play the music
  if (!cm_sound_play(&cms))
    cm_gui_draw_error_d(defcms, "Failed to play sound");

  // The loop when music is playing
  while (cm_sound_isplaying()) {
    cm_sound_delay(100);
  }
  cm_sound_end(&cms);
  free(music_path);
  return NULL;
}

// Load musics to cmstate->musics array
static void cm_music_load_musics(struct cm_state* cmstate,
                                 struct cm_file_list_dir_data* tld) {
  if (tld && tld->len > 0) {
    int max_items = defcms->yMax - 2;
    for (int i = 0; i < tld->len && i < max_items; i++) {
      cm_string name = strdup(tld->dirs[i].name);
      if (cm_string_ends_with(name, ".mp3") ||
          cm_string_ends_with(name, ".wav")) {
        if (cmstate->musics_amount >= cmstate->musics_capacity) {
          cmstate->musics_capacity = cmstate->musics_amount + 1;
          cmstate->musics =
              realloc(cmstate->musics,
                      sizeof(struct cm_music) * cmstate->musics_capacity);
        }
        struct cm_vec2 pos;
        pos = cm_vec2(defcms->yMax / 2 + i, defcms->xMax / 2 - 8);
        cmstate->musics[cmstate->musics_amount++] = cm_music(name, pos);
      }
    }
    cmstate->selected_music = cmstate->musics[0];
    cm_screen_move(cmstate->musics[0].position);
  }
}

// Draw Musics List form cmstate->musics array
static void cm_music_draw_musics_list(struct cm_state* cmstate) {
  if (cmstate->musics && cmstate->musics_amount > 0) {
    for (int i = 0; i < cmstate->musics_amount; ++i) {
      struct cm_music cmm;
      cmm = cmstate->musics[i];
      cm_gui_draw_text(CM_COLOR_PRIMARY_PAIR,
                       cm_vec2(cmm.position.y, cmm.position.x), "%s", cmm.name);
    }
  } else {
    cm_gui_draw_text(CM_COLOR_PRIMARY_PAIR,
                     cm_vec2(defcms->yMax / 2, defcms->xMax / 2 - 5),
                     "No files found");
    cm_screen_move(cm_vec2(0, 0));  // move cursor to y:0 x:0
  }
}

int main() {
  // Initializes Default CM Screen (ncurses and more)
  defcms = cm_screen_create();
  if (!defcms) {
    cm_gui_draw_error_d(defcms, "Failed to create screen");
    cm_screen_end_ncurses();  // just end ncurses to avoid bugs in terminal
    return EXIT_FAILURE;
  }

  // Initialize states
  struct cm_state cmstate;
  cm_state_init(&cmstate);

  // List all musics from dir
  struct cm_file_list_dir_data* tld;
  tld = cm_file_list_dir(CM_ENV_PATH);

  cm_gui_draw_text(CM_COLOR_PRIMARY_PAIR, cm_vec2(defcms->yMax - 2, 0),
                   "Press 'q' to quit.");
  cm_screen_clear_line(defcms, 0);

  // Online
  struct cm_online cmom;
  cmom = cm_online_init();
  if (!cm_online_get_musics(&cmom, "Nuts")) {
    cm_gui_draw_error_d(defcms, "Failed to get musics");
    cm_screen_end_ncurses();  // just end ncurses to avoid bugs in terminal
    return EXIT_FAILURE;
  }
  cm_online_display_musics(&cmom);
  cm_online_cleanup(&cmom);

  pthread_t music_thread;
  cm_string input = malloc(1024);

  cm_music_load_musics(&cmstate, tld);
  cm_music_draw_musics_list(&cmstate);

  // handle chars
  // when 'q' finish program
  int ch;
  while (cmstate.isRunning) {
    ch = cm_screen_getchar();

    cm_screen_update(defcms);
    if (!cm_sound_isplaying()) {
      cm_screen_clear_line(defcms, 0);
    }

    cm_music_draw_musics_list(&cmstate);

    if (ch == 'w' || ch == CM_KEY_UP) {
      if (cmstate.mode != CM_MODE_INPUT) {
        if (defcms->cursor->y != 0 &&
            !(defcms->cursor->y <= cmstate.musics[0].position.y)) {
          cm_screen_move(cm_vec2(--defcms->cursor->y, defcms->cursor->x));
          for (int i = 0; i < cmstate.musics_amount; i++) {
            struct cm_music cmm;
            cmm = cmstate.musics[i];
            if (cmm.position.y == defcms->cursor->y) {
              cmstate.selected_music = cmm;
            }
          }
        }
      }
    } else if (ch == 's' || ch == CM_KEY_DOWN) {
      if (cmstate.mode != CM_MODE_INPUT) {
        if (defcms->cursor->y != defcms->yMax &&
            !(defcms->cursor->y >=
              cmstate.musics[cmstate.musics_amount - 1].position.y)) {
          cm_screen_move(cm_vec2(++defcms->cursor->y, defcms->cursor->x));
          for (int i = 0; i < cmstate.musics_amount; i++) {
            struct cm_music cmm;
            cmm = cmstate.musics[i];
            if (cmm.position.y == defcms->cursor->y) {
              cmstate.selected_music = cmm;
            }
          }
        }
      }
    } else if (ch == '\n') {
      if (cmstate.mode != CM_MODE_INPUT) {
        if (!cm_sound_isplaying()) {
          cm_gui_draw_text(CM_COLOR_PRIMARY_PAIR, cm_vec2(defcms->yMax - 2, 0),
                           "Press 'q' to pause.");
          pthread_create(&music_thread, NULL, cm_music_player_thread,
                         (void*)cmstate.selected_music.name);
          pthread_detach(music_thread);
        }
      }
    } else if (ch == CM_KEY_LEFT) {
    } else if (ch == CM_KEY_RIGHT) {
    } else if (ch == 'q') {
      if (cmstate.mode != CM_MODE_INPUT) {
        cm_screen_clear_line(defcms, 0);
        if (cm_sound_isplaying()) {
          cm_gui_draw_text(CM_COLOR_PRIMARY_PAIR, cm_vec2(defcms->yMax - 2, 0),
                           "Press 'q' to quit.");
          cm_sound_stop();
          cmstate.selected_music = cmstate.musics[0];
          cm_screen_move(cmstate.musics[0].position);
        } else {
          cmstate.isRunning = CM_MODE_SELECT;
        }
      }
    } else if (ch == ':') {
      // input mode
      struct cm_vec2 input_pos;
      input_pos = cm_vec2(defcms->yMax - 1, 0);
      cmstate.mode = CM_MODE_INPUT;
      cm_gui_draw_text(CM_COLOR_PRIMARY_PAIR, input_pos, ":");
      cm_screen_move(cm_vec2(input_pos.y, input_pos.x + 2));
      cm_screen_enable_echo();
      cm_screen_get_str(input);
      cm_screen_disable_echo();
      cm_gui_draw_text(CM_COLOR_PRIMARY_PAIR, cm_vec2(0, 0), "You write: %s",
                       input);
      // exit input mode
      cm_screen_clear_line(defcms, defcms->yMax - 1);
      cmstate.selected_music = cmstate.musics[0];
      cm_screen_move(cmstate.musics[0].position);
      cmstate.mode = CM_MODE_SELECT;
      continue;
    }
    cm_screen_move(cmstate.selected_music.position);
  }

  free(input);
  cm_file_list_dir_close(tld);
  cm_screen_end(defcms);
  cm_state_close(&cmstate);
  return EXIT_SUCCESS;
}