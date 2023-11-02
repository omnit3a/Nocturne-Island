#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <player.h>
#include <map.h>
#include <physics.h>
#include <map_defs.h>
#include <entity.h>
#include <inventory.h>
#include <ui.h>
#include <menu_defs.h>
#include <menu.h>

int block_progress = 0;

tag_t player_tag = {
  0,
  0
};
sprite_t player_sprite;
entity_t player_entity;
transform_t current_rotation;

int x_pos_offset = SPAWN_X;
int y_pos_offset = SPAWN_Y;

void spawn_player(){
  player_entity.position.x = SPAWN_X;
  player_entity.position.y = SPAWN_Y;
  player_entity.position.z = SPAWN_Z;
  player_entity.rotation.x = SPAWN_X;
  player_entity.rotation.y = SPAWN_Y;
  player_entity.rotation.z = SPAWN_Z;
  current_rotation.x = 0;
  current_rotation.y = 1;
  current_rotation.z = 0;
}

int get_mining_speed(){
  transform_t pos = player_entity.position;
  transform_t rot = player_entity.rotation;
  rot.x += current_rotation.x;
  rot.y += current_rotation.y;
  rot.z = pos.z+current_rotation.z;
  
  switch (get_block(rot.x, rot.y, rot.z).block.block_type){
    case TERRAIN_TYPE:
      if(compare_blocks(get_current_item()->item, get_block_properties(SHARP_STICK))){
	return 2;
      }
      break;
  }
  return 1;
}

int get_block_progress(){
  return block_progress;
}

/* Mine a block in the direction of the player */
void player_mine_block(){
  transform_t pos = player_entity.position;
  transform_t rot = player_entity.rotation;
  rot.x += current_rotation.x;
  rot.y += current_rotation.y;
  rot.z = pos.z+current_rotation.z;
  
  if (get_active_menu() != GAME_UI_ID){
    return;
  }
  
  if (!is_block_mineable(get_block(rot.x, rot.y, rot.z).block)){
    return;
  }

  if (get_block(rot.x, rot.y, rot.z).block.hp > 1){
    int state = get_block(rot.x, rot.y, rot.z).current_state;
    world_data_t block = get_block(rot.x, rot.y, rot.z);
    block.block.hp -= get_mining_speed();
    set_block(block.block, rot.x, rot.y, rot.z);
    // prevent reseting of block_state to 0
    set_block_state(state, rot.x, rot.y, rot.z);
    set_changed_blocks(get_block(rot.x, rot.y, rot.z),
		       SPAWN_X+x_pos_offset+current_rotation.x,
		       SPAWN_Y+y_pos_offset+current_rotation.y,
		       rot.z);
    block_progress = block.block.hp;
    if (block.block.hp > 0){
      return;
    }
    
  }
  
  if (get_block(rot.x, rot.y, rot.z).block.solid){
    block_data_t block = get_block_properties(get_block(rot.x, rot.y, rot.z).block.dropped_item);
    int count = get_block(rot.x, rot.y, rot.z).block.count;
    add_inventory_item(block, count);
    set_block(get_block_properties(EMPTY), rot.x, rot.y, rot.z);
    set_changed_blocks(get_block(rot.x, rot.y, rot.z),
		       SPAWN_X+x_pos_offset+current_rotation.x,
		       SPAWN_Y+y_pos_offset+current_rotation.y,
		       rot.z);
    block_progress = 0;
  }
}

/* Allow player to place a block from the inventory */
void player_place_block(){
  transform_t pos = player_entity.position;
  transform_t rot = player_entity.rotation;
  block_data_t block = get_current_item()->item;
  rot.x += current_rotation.x;
  rot.y += current_rotation.y;
  rot.z = pos.z+current_rotation.z;
  
  if (get_active_menu() != GAME_UI_ID){
    return;
  }

  if (get_block(rot.x, rot.y, rot.z).block.solid){
    return;
  }
  
  int result = remove_inventory_item(get_current_item()->item, 1);
  
  if (result){
    set_block(block, rot.x, rot.y, rot.z);
    set_changed_blocks(get_block(rot.x, rot.y, rot.z),
		       SPAWN_X+x_pos_offset+current_rotation.x,
		       SPAWN_Y+y_pos_offset+current_rotation.y,
		       rot.z);
  }
}

/* Get user input for the player, then do stuff with it */
void handle_player_movement(SDL_Event event){
  transform_t pos = player_entity.position;
  transform_t rot = player_entity.rotation;
  transform_t prev_rot = current_rotation;
  
  int move_player = 0;

  if (get_active_menu() != GAME_UI_ID){
    return;
  }
  
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
  rot.x += current_rotation.x;
  rot.y += current_rotation.y;
  rot.z = pos.z;
  
  if(!get_block(rot.x, rot.y, rot.z).block.solid && move_player){
    x_pos_offset += current_rotation.x;
    y_pos_offset += current_rotation.y;
    generate_hills(x_pos_offset, y_pos_offset);
  }
  current_rotation.x = prev_rot.x;
  current_rotation.y = prev_rot.y;
  current_rotation.z = prev_rot.z;
}

void handle_player_rotation(SDL_Event event){

  if (get_active_menu() != GAME_UI_ID){
    return;
  }
  
  switch (event.key.keysym.sym){
    case SDLK_i:
      current_rotation.x = 0;
      current_rotation.y = -1;
      player_entity.sprite.frame_offset = 2;
      break;
    case SDLK_j:
      current_rotation.x = -1;
      current_rotation.y = 0;
      player_entity.sprite.frame_offset = 3;
      break;
    case SDLK_k:
      current_rotation.x = 0;
      current_rotation.y = 1;
      player_entity.sprite.frame_offset = 0;
      break;
    case SDLK_l:
      current_rotation.x = 1;
      current_rotation.y = 0;
      player_entity.sprite.frame_offset = 1;
      break;
    case SDLK_u:
      current_rotation.z = current_rotation.z - 1;
      if (current_rotation.z == -2){
	current_rotation.z = 1;
      }
      break;
  }
}

entity_t * get_player_entity(){
  return &player_entity;
}

void init_player_entity(){
  spawn_player();
  entity_set_tag(&player_entity, &player_tag);
}

transform_t get_player_direction(){
  return current_rotation;
}

