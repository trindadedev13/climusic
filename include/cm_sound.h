#ifndef CM_SOUND_H
#define CM_SOUND_H

#include <SDL2/SDL_mixer.h>

#include "cm_bool.h"
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

// Returns if sound's playing.
cm_bool cm_sound_isplaying();

// Delay to keep sound playing.
void cm_sound_delay(int ms);

// Finish Sound stuffs, desallocate etc.
void cm_sound_end(struct cm_sound* cms);

// Play the sound.
cm_bool cm_sound_play(struct cm_sound* cms);

// Stop the reproduction.
void cm_sound_stop();

#endif