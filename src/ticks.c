#include <SDL2/SDL.h>
#include <ticks.h>
#include <physics.h>
#include <camera.h>

int current_tick = 0;

int get_current_tick(){
  current_tick = (SDL_GetTicks() / (1000 / TICKS_PER_SECOND) % TICKS_PER_SECOND) + 1;
  return current_tick;
}

void tick_update(){
  if (get_current_tick() % (TICKS_PER_SECOND / 8) == 0){
    handle_physics();
  } else {
    reset_physics();
  }

  if (get_current_tick() % 2 == 0){
    update_camera();
  }
}

