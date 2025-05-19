#ifndef CM_SOUND_H
#define CM_SOUND_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>

#include "cm_bool.h"
#include "cm_inline.h"
#include "cm_string.h"

struct cm_sound {
  // the name/path of sound
  cm_string name;
  // the sdl sound
  Mix_Music* music;
};

#define cm_sound(name) cm_sound_make(name);

struct cm_sound cm_sound_make(cm_string name);

// Inits the sound.
cm_bool cm_sound_init();

// Finish Sound stuffs, desallocate etc.
void cm_sound_end(struct cm_sound* cms);

// Play the sound.
cm_bool cm_sound_play(struct cm_sound* cms);

// Returns if sound's playing.
CM_INLINE static cm_bool cm_sound_isplaying() {
  return Mix_PlayingMusic();
}

// Delay to keep sound playing.
CM_INLINE static void cm_sound_delay(int ms) {
  SDL_Delay(ms);
}

// Stop the reproduction.
CM_INLINE static void cm_sound_stop() {
  Mix_HaltMusic();
}

#endif