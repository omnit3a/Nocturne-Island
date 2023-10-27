#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <player.h>
#include <map.h>
#include <drawer.h>
#include <camera.h>
#include <physics.h>
#include <ui.h>
#include <map_defs.h>

int playerX = MAP_WIDTH/2;
int playerY = MAP_LENGTH/2;
int playerZ = MAP_HEIGHT-1;
int playerDirectionX = 1;
int playerDirectionY = 1;
rotation_t playerRotation = NORTH;
z_rotation_t playerZRotation = STRAIGHT;
int playerXOff;
int playerYOff;
int playerZOff;
int playerHealth = 50;

void spawn_player(){
  playerX = SPAWN_X;
  playerY = SPAWN_Y;
  playerZ = SPAWN_Z;
}

int get_mining_speed(){
  return 1;
}

/* Make the player face a specific direction */
void set_player_rotation(rotation_t rotation){
  switch (rotation){
    case NORTH:
      playerDirectionX = 0;
      playerDirectionY = 0;
      break;
    case EAST:
      playerDirectionX = 0;
      playerDirectionY = 1;
      break;
    case SOUTH:
      playerDirectionX = 1;
      playerDirectionY = 1;
      break;
    case WEST:
      playerDirectionX = 1;
      playerDirectionY = 0;
      break;
  }
  playerRotation = rotation;
}

// add direction offsets for mining and placing blocks
void player_offset_direction(){
  playerZOff = playerZ;
  switch (playerRotation){
    case NORTH:
      if (playerY == 0){
	playerYOff = playerY;
	break;
      }
      playerXOff = playerX;
      playerYOff = playerY-1;
      break;
    case EAST:
      if (playerX == 0){
	playerXOff = playerX;
	break;
      }
      playerXOff = playerX-1;
      playerYOff = playerY;
      break;
    case SOUTH:
      if (playerY == MAP_LENGTH-1){
	playerYOff = playerY;
	break;
      }
      playerXOff = playerX;
      playerYOff = playerY+1;
      break;
    case WEST:
      if (playerX == MAP_WIDTH-1){
	playerXOff = playerX;
	break;
      }
      playerXOff = playerX+1;
      playerYOff = playerY;
      break;
  }
}

/* Mine a block in the direction of the player */
void player_mine_block(){
  player_offset_direction();
  if (get_block(playerXOff, playerYOff, playerZOff).hp < -100){
    return;
  }

  if (get_block(playerXOff, playerYOff, playerZOff).hp > 1){
    block_data_t block = get_block(playerXOff, playerYOff, playerZOff).block;
    block.hp--;
    set_block(block, playerXOff, playerYOff, playerZOff);
    return;
  }
  
  if (get_block(playerXOff, playerYOff, playerZOff).block.solid){
    set_block(get_block_properties(EMPTY), playerXOff, playerYOff, playerZOff);    
  }
}

/* Allow player to place a block from the inventory */
void player_place_block(int block){
  /* TODO */
}

/* Get user input for the player, then do stuff with it */
void handle_player_movement(SDL_Event event){
  rotation_t prevRotation = playerRotation;
  player_offset_direction();
  int move_player = 0;
  switch (event.key.keysym.sym){
    case SDLK_w:
      set_player_rotation(NORTH);
      move_player = 1;
      break;
    case SDLK_a:
      set_player_rotation(EAST);
      move_player = 1;
      break;
    case SDLK_s:
      set_player_rotation(SOUTH);
      move_player = 1;
      break;
    case SDLK_d:
      set_player_rotation(WEST);
      move_player = 1;
      break;
    case SDLK_SPACE:
      /* Check for empty space above player and solid space below player */
      if (!get_block(playerX,playerY,playerZ+1).block.solid && get_block(playerX,playerY,playerZ-1).block.solid){
	player_jump();
      }
      break;
  }
  player_offset_direction();
  if(!get_block(playerXOff, playerYOff, playerZ).block.solid && move_player){
    playerX = playerXOff;
    playerY = playerYOff;
    playerRotation = prevRotation;
  } else {
    playerRotation = prevRotation;
  }
}

void handle_player_rotation(SDL_Event event){
  switch (event.key.keysym.sym){
    case SDLK_i:
      playerRotation = NORTH;
      break;
    case SDLK_j:
      playerRotation = EAST;
      break;
    case SDLK_k:
      playerRotation = SOUTH;
      break;
    case SDLK_l:
      playerRotation = WEST;
      break;
  }
}
