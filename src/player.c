#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <player.h>
#include <map.h>
#include <physics.h>
#include <map_defs.h>
#include <entity.h>
#include <inventory.h>

tag_t player_tag = {
  0,
  0
};
sprite_t player_sprite;
entity_t player_entity;
transform_t current_rotation;

void spawn_player(){
  player_entity.position.x = SPAWN_X;
  player_entity.position.y = SPAWN_Y;
  player_entity.position.z = SPAWN_Z;
  player_entity.rotation.x = SPAWN_X;
  player_entity.rotation.y = SPAWN_Y;
  player_entity.rotation.z = SPAWN_Z;
  current_rotation.x = 0;
  current_rotation.y = -1;
  current_rotation.z = 0;
}

int get_mining_speed(){
  return 1;
}

// add direction offsets for mining and placing blocks
void player_rotate(){
  transform_t pos = player_entity.position;
  transform_t offset = pos;

  offset.x = pos.x+current_rotation.x;
  offset.y = pos.y+current_rotation.y;
  offset.z = pos.z+current_rotation.z;

  entity_rotate(&player_entity, &offset);
}

/* Mine a block in the direction of the player */
void player_mine_block(){
  player_rotate();
  transform_t rot = player_entity.rotation;
  if (!is_block_mineable(get_block(rot.x, rot.y, rot.z).block)){
    return;
  }

  if (get_block(rot.x, rot.y, rot.z).hp > 1){
    int state = get_block(rot.x, rot.y, rot.z).current_state;
    block_data_t block = get_block(rot.x, rot.y, rot.z).block;
    block.hp--;
    set_block(block, rot.x, rot.y, rot.z);
    // prevent reseting of block_state to 0
    set_block_state(state, rot.x, rot.y, rot.z);
    return;
  }
  
  if (get_block(rot.x, rot.y, rot.z).block.solid){
    block_data_t block = get_block_properties(get_block(rot.x, rot.y, rot.z).block.dropped_item);
    int count = get_block(rot.x, rot.y, rot.z).block.count;
    add_inventory_item(block, count);
    set_block(get_block_properties(EMPTY), rot.x, rot.y, rot.z);
  }
}

/* Allow player to place a block from the inventory */
void player_place_block(){
  transform_t rot = player_entity.rotation;
  block_data_t block = get_current_item()->item;
  int result = remove_inventory_item(get_current_item()->item, 1);
  if (result && !get_block(rot.x, rot.y, rot.z).block.solid){
    set_block(block, rot.x, rot.y, rot.z);
  }
}

/* Get user input for the player, then do stuff with it */
void handle_player_movement(SDL_Event event){
  transform_t pos = player_entity.position;
  transform_t prev_rot = current_rotation;
  transform_t rot;
  
  int move_player = 0;
  
  switch (event.key.keysym.sym){
    case SDLK_w:
      current_rotation.x = 0;
      current_rotation.y = -1;
      move_player = 1;
      break;
    case SDLK_a:
      current_rotation.x = -1;
      current_rotation.y = 0;
      move_player = 1;
      break;
    case SDLK_s:
      current_rotation.x = 0;
      current_rotation.y = 1;
      move_player = 1;
      break;
    case SDLK_d:
      current_rotation.x = 1;
      current_rotation.y = 0;
      move_player = 1;
      break;
    case SDLK_SPACE:
      /* Check for empty space above player and solid space below player */
      if (!get_block(pos.x, pos.y, pos.z+1).block.solid &&
	  get_block(pos.x, pos.y, pos.z-1).block.solid){
	player_jump();
      }
      break;
  }
  player_rotate();
  rot = player_entity.rotation;
  if(!get_block(rot.x, rot.y, rot.z).block.solid && move_player){
    entity_move(&player_entity, &rot);
  }
  current_rotation = prev_rot;
  player_rotate();
}

void handle_player_rotation(SDL_Event event){
  switch (event.key.keysym.sym){
    case SDLK_i:
      current_rotation.x = 0;
      current_rotation.y = -1;
      break;
    case SDLK_j:
      current_rotation.x = -1;
      current_rotation.y = 0;
      break;
    case SDLK_k:
      current_rotation.x = 0;
      current_rotation.y = 1;
      break;
    case SDLK_l:
      current_rotation.x = 1;
      current_rotation.y = 0;
      break;
  }
  player_rotate();
}

entity_t * get_player_entity(){
  return &player_entity;
}

void init_player_entity(){
  spawn_player();
  player_rotate();
  entity_set_tag(&player_entity, &player_tag);
}

transform_t get_player_direction(){
  return current_rotation;
}
