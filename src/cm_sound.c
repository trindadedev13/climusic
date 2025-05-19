#include "cm_sound.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>

#include "cm_bool.h"
#include "cm_colors.h"
#include "cm_gui.h"
#include "cm_screen.h"
#include "cm_string.h"

struct cm_sound cm_sound_make(cm_string name) {
  struct cm_sound s;
  s.name = name;
  return s;
}

cm_bool cm_sound_init() {
  if (SDL_Init(SDL_INIT_AUDIO) < 0) {
    cm_gui_draw_text_d(defcms, CM_COLOR_RED_PAIR,
                       "Failed to initialize sdl audio: %s\n", SDL_GetError());
    return false;
  }

  if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
    cm_gui_draw_text_d(defcms, CM_COLOR_RED_PAIR,
                       "Failed to Open audio 44100: %s\n", SDL_GetError());
    return false;
  }

  return true;
}

void cm_sound_end(struct cm_sound* cms) {
  if (cms->music) {
    Mix_FreeMusic(cms->music);
  }
  Mix_CloseAudio();
  SDL_Quit();
}

cm_bool cm_sound_play(struct cm_sound* cms) {
  cms->music = Mix_LoadMUS(cms->name);
  if (!cms->music) {
    cm_gui_draw_text_d(defcms, CM_COLOR_RED_PAIR, "Failed to Load Music: %s\n",
                       SDL_GetError());
    return false;
  }

  if (Mix_PlayMusic(cms->music, 1) < 0) {
    cm_gui_draw_text_d(defcms, CM_COLOR_RED_PAIR, "Failed to Play Music: %s\n",
                       SDL_GetError());
    return false;
  }

  return true;
}