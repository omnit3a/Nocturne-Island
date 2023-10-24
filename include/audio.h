#ifndef AUDIO_H_
#define AUDIO_H_

#include <SDL2/SDL.h>

#define BLOCK_BREAK_SFX_PATH "assets/sfx/dig-dirt.wav"
#define BLOCK_PLACE_SFX_PATH "assets/sfx/build.wav"
#define HITGROUND_SFX_PATH "assets/sfx/hitground.wav"
#define JUMP_SFX_PATH "assets/sfx/jumpblock.wav"

int playWav(char * path_name);

#endif
