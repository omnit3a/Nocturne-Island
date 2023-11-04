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
int switch_day = 0;
unsigned long hunger_tick = 0;

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
    fire_update();
  } else {
    reset_physics();
  }

  if ((current_tick & (4 - 1)) == 0){
    update_camera();
  }

  day_night_update();
  hunger_update();
}

void day_night_update(){
  if (SDL_GetTicks() - day_tick > SDL_TICKS_PER_DAY){
    is_day = !is_day;
    day_tick = SDL_GetTicks();
  }
}

int is_daytime(){
  return is_day;
}

void hunger_update(){
  if (SDL_GetTicks() - hunger_tick >= HUNGER_TICKS){
    hunger_tick += HUNGER_TICKS;
    int hunger = get_player_hunger();
    set_player_hunger(hunger - 1);
  }
}

void water_flow_update(){

}

void fire_update(){
  int start_index = (1 * CHUNK_WIDTH) + 1;
  int end_index = ((CHUNK_LENGTH-1) * CHUNK_WIDTH) + (CHUNK_WIDTH - 1);
  int average = 0;
  for (int reps = 0 ; reps < 2 ; reps++){
  for (int index = start_index ; index < end_index ; index++){
    int x = index % CHUNK_WIDTH;
    int y = index / CHUNK_LENGTH;
    for (int z = 1 ; z < CHUNK_HEIGHT-1 ; z++){
      average = 0;
      average += get_block(x, y, z-1).temperature;
      average += get_block(x, y, z+1).temperature;
      average += get_block(x, y-1, z).temperature;
      average += get_block(x, y+1, z).temperature;
      average += get_block(x-1, y, z).temperature;
      average += get_block(x+1, y, z).temperature;
      set_temperature(average / 6, x, y, z);
      printf("%d\n", average / 6);

      if (get_block(x, y, z).block.ignition == -1){
	continue;
      }

      if (get_block(x, y, z).block.id == FIRE){
	continue;
      }
      
      if (get_block(x, y, z).temperature > get_block(x, y, z).block.ignition){
	world_data_t prev_data = get_block(x, y, z);
	set_block(get_block_properties(FIRE), x, y, z);
	set_changed_blocks(prev_data,
			   get_block(x, y, z),
			   x,
			   y,
			   z);
      }
    }
  }
  }
}
