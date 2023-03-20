#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <player.h>
#include <map.h>
#include <drawer.h>
#include <camera.h>
#include <physics.h>
#include <pthread.h>
#include <unistd.h>
#include <inventory.h>
#include <crafting.h>

int playerX = MAP_WIDTH/2;
int playerY = MAP_LENGTH/2;
int playerZ = MAP_HEIGHT-1;
int playerDirectionX = 1;
int playerDirectionY = 1;
rotation_t playerRotation = NORTH;
z_rotation_t playerZRotation = STRAIGHT;
pthread_t jump_thread;

int playerXOff;
int playerYOff;
int playerZOff;

int playerHealth = 50;

void movePlayer(int xOff, int yOff, int zOff, SDL_Renderer * renderer){
  playerX += xOff;
  playerY += yOff;
  playerZ += zOff;
}

/* Make the player face a specific direction */
void setPlayerRotation(rotation_t rotation){
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

/* Rotate player on the x and y axis */
void rotatePlayerClockwise(){
  playerRotation = (playerRotation + 1) % 4;
}

/* Rotate player on the z axis */
void rotatePlayerUp(){
  playerZRotation = (playerZRotation +1) % 3;
}

// add direction offsets for mining and placing blocks
void playerOffsetDirection(){
  switch (playerRotation){
    case NORTH:
      playerXOff = playerX;
      playerYOff = playerY-1;
      break;
    case EAST:
      playerXOff = playerX-1;
      playerYOff = playerY;
      break;
    case SOUTH:
      playerXOff = playerX;
      playerYOff = playerY+1;
      break;
    case WEST:
      playerXOff = playerX+1;
      playerYOff = playerY;
      break;
  }
  switch (playerZRotation){
    case UP:
      playerZOff = playerZ+1;
      break;
    case STRAIGHT:
      playerZOff = playerZ;
      break;
    case DOWN:
      playerZOff = playerZ-1;
      break;
  }
}

/* Mine a block in the direction of the player */
blocks_t playerMineBlock(char map[MAP_WIDTH][MAP_LENGTH][MAP_HEIGHT]){
  playerOffsetDirection();
  if (playerX > 0 && playerX < MAP_WIDTH-1 && playerY > 0 && playerY < MAP_LENGTH-1 && playerZ > 0 && playerZ < MAP_HEIGHT-1){
    static blocks_t temp_block = 0;
    /* If trying to mine NOKIUM, return from function */
    /* I probably want to replace this with a hardness value for each block */
    if (getBlockProperties(map,playerXOff,playerYOff,playerZOff).hp < 0){
      return map[playerXOff][playerYOff][playerZOff];
    }
    if (getBlockProperties(map,playerXOff,playerYOff,playerZOff).solid){
      /* Add the mined item to the players inventory */
      addItemToInventory(getBlockProperties(map, playerXOff, playerYOff, playerZOff).dropped_item, getBlockProperties(map, playerXOff, playerYOff, playerZOff).count);
      map[playerXOff][playerYOff][playerZOff] = 0;
      waterFlow(playerXOff, playerYOff, playerZOff, map);
      blockingPlayerCheck(map);
    }
    return temp_block;
  }
  return 0;
}

/* Allow player to place a block from the inventory */
void playerPlaceBlock(char map[MAP_WIDTH][MAP_LENGTH][MAP_HEIGHT], blocks_t block){
  playerOffsetDirection();
  if (playerX > 0 && playerX < MAP_WIDTH-1 && playerY > 0 && playerY < MAP_LENGTH-1 && playerZ > 0 && playerZ < MAP_HEIGHT-1){
    if (!(getBlockProperties(map,playerXOff,playerYOff,playerZOff).solid) && checkInventoryForItem(block)){

      /* Handle placing of stairs so that they face the correct direction*/
      switch(block){
        case STAIRS:
	  switch(playerRotation){
	    case NORTH:
	      map[playerXOff][playerYOff][playerZOff] = SOUTH_STAIRS;
	      break;
	    case EAST:
	      map[playerXOff][playerYOff][playerZOff] = WEST_STAIRS;
	      break;
	    case SOUTH:
	      map[playerXOff][playerYOff][playerZOff] = NORTH_STAIRS;
	      break;
	    case WEST:
	      map[playerXOff][playerYOff][playerZOff] = EAST_STAIRS;
	      break;
	  }
	  break;
        default:
	  map[playerXOff][playerYOff][playerZOff] = block;
	  break;
      }
      checkAndRemoveItem(block, 1);
      blockingPlayerCheck(map);
    }
  }
}

/* Get user input for the player, then do stuff with it */
void handlePlayerMovement(char map[MAP_WIDTH][MAP_LENGTH][MAP_HEIGHT], SDL_Event event){
  rotation_t prevRotation = playerRotation;
  playerOffsetDirection();
  switch (event.key.keysym.sym){
    case SDLK_a:
      setPlayerRotation(NORTH);
      break;
    case SDLK_s:
      setPlayerRotation(WEST);
      break;
    case SDLK_w:
      setPlayerRotation(EAST);
      break;
    case SDLK_d:
      setPlayerRotation(SOUTH);
      break;
    case SDLK_r:
      rotatePlayerClockwise();
      break;
    case SDLK_z:
      interactWithWorkbench(map);
      break;
    case SDLK_f:
      rotatePlayerUp();
      break;
    case SDLK_SPACE:
      /* Check for empty space above player and solid space below player */
      if (!(getBlockProperties(map,playerX,playerY,playerZ+1).solid) && getBlockProperties(map,playerX,playerY,playerZ-1).solid){
        pthread_create(&jump_thread, NULL, handlePlayerJumping, NULL);
        blockingPlayerCheck(map);
      }
      break;
  }
  playerOffsetDirection();
  switch (event.key.keysym.sym){
    case SDLK_w:
    case SDLK_a:
    case SDLK_s:
    case SDLK_d:
      switch (getBlockProperties(map, playerXOff, playerYOff, playerZ).block){
	/* Move player up if going up stairs */
        case STAIRS:
	  if (!getBlockProperties(map, playerXOff,playerYOff, playerZ+1).solid){
	    playerZ++;
	  }
	  break;
      }
      if(!getBlockProperties(map, playerXOff, playerYOff, playerZ).solid){
	playerX = playerXOff;
	playerY = playerYOff;
	blockingPlayerCheck(map);
	playerRotation = prevRotation;
      } else {
	playerRotation = prevRotation;
      }
      break;
  }
}

