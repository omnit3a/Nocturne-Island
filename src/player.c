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
#include <map.h>
#include <controls.h>
#include <ticks.h>
#include <tools/waterskin.h>

int block_progress = 0;

tag_t player_tag = {
  0,
  0
};
sprite_t player_sprite;
entity_t player_entity;
transform_t current_rotation;
int player_hunger = 5;
int player_health = 10;
int player_thirst = 5;

int x_pos_offset = SPAWN_X;
int y_pos_offset = SPAWN_Y;

void spawn_player(){
  player_entity.position.x = SPAWN_X;
  player_entity.position.y = SPAWN_Y;
  get_height(&player_entity.position.z, SPAWN_X, SPAWN_Y);
  player_entity.position.z++;
  player_entity.rotation.x = SPAWN_X;
  player_entity.rotation.y = SPAWN_Y;
  get_height(&player_entity.rotation.z, SPAWN_X, SPAWN_Y);
  player_entity.position.z++;
  player_health = 10;
  player_hunger = 5;
  player_thirst = 5;
  current_rotation.x = 0;
  current_rotation.y = 1;
  current_rotation.z = 0;
}

int get_mining_speed(){
  return 1;
}

int get_block_progress(){
  return block_progress;
}

int get_player_thirst(){
  return player_thirst;
}

void set_player_thirst(int value){
  if (value + player_hunger > STOMACH_CAPACITY){
    return;
  }

  player_thirst = value;
  if (player_thirst < 0){
    player_thirst = 0;
    return;
  }
  if (player_thirst > 10){
    player_thirst = 10;
    return;
  }
}

int get_player_hunger(){
  return player_hunger;
}

void set_player_hunger(int value){
  if (player_thirst + value > STOMACH_CAPACITY){
    return;
  }

  player_hunger = value;
  if (player_hunger < 0){
    player_hunger = 0;
    return;
  }
  if (player_hunger > 10){
    player_hunger = 10;
    return;
  }
}

int get_player_health(){
  return player_health;
}

void set_player_health(int value){
  player_health = value;
}

void player_eat_food(){
  if (get_active_menu() != GAME_UI_ID){
    return;
  }

  if (get_current_item()->item.block_type != FOOD_TYPE){
    return;
  }

  if (player_hunger + player_thirst == STOMACH_CAPACITY){
    return;
  }

  if ((player_hunger + 1) + player_thirst > STOMACH_CAPACITY){
    return;
  }
  
  int result = remove_inventory_item(get_current_item()->item, 1);
  
  if (result){
    set_player_hunger(player_hunger + 1);
  }
}

void player_drink_water(){
  transform_t pos = player_entity.position;
  transform_t rot = player_entity.rotation;
  rot.x += current_rotation.x;
  rot.y += current_rotation.y;
  rot.z = pos.z-1;
  if (get_active_menu() != GAME_UI_ID){
    return;
  }

  if (get_block(rot.x, rot.y, rot.z).block.id != WATER){
    return;
  }

  if (player_thirst + player_hunger == STOMACH_CAPACITY){
    return;
  }

  set_player_thirst(player_thirst + 1);

}

/* Mine a block in the direction of the player */
void player_mine_block(){
  int strength = 0;
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

  if (get_current_item()->item.hardness <= 0){
    strength = 0;
  } else {
    strength = get_current_item()->item.hardness;
  }

  if (get_block(rot.x, rot.y, rot.z).block.hardness > strength && get_block(rot.x, rot.y, rot.z).block.solid){
    return;
  }
  
  if (get_block(rot.x, rot.y, rot.z).block.hp > 0){
    world_data_t block = get_block(rot.x, rot.y, rot.z);
    block.block.hp -= get_mining_speed();
    world_data_t prev_data = get_block(rot.x, rot.y, rot.z);
    set_block(block.block, rot.x, rot.y, rot.z);

    set_changed_blocks(1,
		       prev_data,
		       get_block(rot.x, rot.y, rot.z),
		       SPAWN_X+x_pos_offset+current_rotation.x,
		       SPAWN_Y+y_pos_offset+current_rotation.y,
		       rot.z);
    block_progress = block.block.hp;

    if (block.block.hp > 0){
      return;
    }
    
  }
  
  if (get_block(rot.x, rot.y, rot.z).block.hp == 0){
    block_data_t block = get_block_properties(get_block(rot.x, rot.y, rot.z).block.dropped_item);
    int count = get_block(rot.x, rot.y, rot.z).block.count;
    add_inventory_item(block, count);
    world_data_t prev_data = get_block(rot.x, rot.y, rot.z);
    set_block(get_block_properties(prev_data.block.output_id), rot.x, rot.y, rot.z);

    set_changed_blocks(1,
		       prev_data,
		       get_block(rot.x, rot.y, rot.z),
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
    world_data_t prev_data = get_block(rot.x, rot.y, rot.z);
    set_block(block, rot.x, rot.y, rot.z);
    set_changed_blocks(1,
		       prev_data,
		       get_block(rot.x, rot.y, rot.z),
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

  int keycode = translate_keypress(event, GAME_UI_ID);
  
  switch (keycode){
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

  int keycode = translate_keypress(event, GAME_UI_ID);
  
  switch (keycode){
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
	    fill_waterskin();
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

void get_player_offset(int * x_off, int * y_off){
  *x_off = x_pos_offset;
  *y_off = y_pos_offset;
}
