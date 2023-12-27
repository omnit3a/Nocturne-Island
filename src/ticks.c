#include <SDL2/SDL.h>
#include <ticks.h>
#include <physics.h>
#include <camera.h>
#include <player.h>
#include <map.h>

#define SPEED 1 / (1000 / TICKS_PER_SECOND)

int current_tick = 0;
int starting_tick = 0;
int day_tick = 0;
int is_day = 1;
int days_survived = 0;
int switch_day = 0;
unsigned long hunger_tick = 0;
unsigned long thirst_tick = 0;

#define SPEED 1 / (1000 / TICKS_PER_SECOND)

int get_current_tick(){
  current_tick = (SDL_GetTicks() * SPEED & (TICKS_PER_SECOND-1)) + 1;
  return current_tick;
}

void tick_update(){
  get_current_tick();
  if ((current_tick & (20 - 1)) == 0){
    handle_physics();
    water_flow_update();
    int x_off;
    int y_off;
    get_player_offset(&x_off, &y_off);
    fire_update(x_off, y_off);
  } else {
    reset_physics();
  }
  
  if ((current_tick & (4 - 1)) == 0){
    update_camera();
  }

  day_night_update();
  hunger_update();
  thirst_update();
}

void day_night_update(){
  if (SDL_GetTicks() - day_tick > SDL_TICKS_PER_DAY){
    is_day = !is_day;
    day_tick = SDL_GetTicks();
    if (is_day){
      days_survived++;
    }
  }
}

int is_daytime(){
  return is_day;
}

void hunger_update(){
  if (SDL_GetTicks() - hunger_tick >= HUNGER_TICKS){
    hunger_tick += HUNGER_TICKS;
    int hunger = get_player_hunger();
    if (hunger == 0){
      set_player_health(get_player_health() - 1);
    }
    set_player_hunger(hunger - 1);
  }
}

void thirst_update(){
  if (SDL_GetTicks() - thirst_tick >= THIRST_TICKS){
    thirst_tick += THIRST_TICKS;
    int thirst = get_player_thirst();
    if (thirst == 0){
      set_player_health(get_player_health() - 1);
    }
    set_player_thirst(thirst-1);
  }
}

void water_flow_update(){
  /* TODO */
}

void fire_update(int x_off, int y_off){
}

int get_days_survived(){
  return days_survived;
}
