#include <SDL2/SDL.h>
#include <ticks.h>

int current_tick = 0;

int get_current_tick(){
  current_tick = (SDL_GetTicks() / (1000 / TICKS_PER_SECOND) % TICKS_PER_SECOND) + 1;
  return current_tick;
}
