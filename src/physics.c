#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <physics.h>
#include <player.h>
#include <map.h>
#include <ticks.h>
#include <entity.h>

int player_is_jumping = 0;

int ticked = 0;
int jump_timeout = 0;

/* Make the player jump */
void player_jump(){
  transform_t pos = get_player_entity()->position;
  jump_timeout = SDL_GetTicks() + JUMP_LENGTH;
  player_is_jumping = 1;
  pos.z++;
  entity_move(get_player_entity(), &pos);
}

void handle_gravity(){
  transform_t pos = get_player_entity()->position;
  if (player_is_jumping == 0){
    if(!get_block(pos.x, pos.y, pos.z-1).block.solid){
      pos.z--;
      entity_move(get_player_entity(), &pos);
    }
  }
}

void reset_physics(){
  ticked = 0;
  if (SDL_GetTicks() >= jump_timeout){
    player_is_jumping = 0;
  }
}

void handle_physics(){
  if (ticked == 0){
    handle_gravity();
    ticked = 1;
  }
}
