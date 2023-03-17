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

int playerX = MAP_WIDTH/2;
int playerY = MAP_LENGTH/2;
int playerZ = MAP_HEIGHT-1;
int playerDirectionX = 1;
int playerDirectionY = 1;
rotation_t playerRotation;
z_rotation_t playerZRotation;
pthread_t jump_thread;

int playerXOff;
int playerYOff;
int playerZOff;

void movePlayer(int xOff, int yOff, int zOff, SDL_Renderer * renderer){
  playerX += xOff;
  playerY += yOff;
  playerZ += zOff;
}

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

void rotatePlayerClockwise(){
  playerRotation = (playerRotation + 1) % 4;
}

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

blocks_t playerMineBlock(char map[MAP_WIDTH][MAP_LENGTH][MAP_HEIGHT]){
  playerOffsetDirection();
  if (playerX > 0 && playerX < MAP_WIDTH-1 && playerY > 0 && playerY < MAP_LENGTH-1 && playerZ > 0 && playerZ < MAP_HEIGHT-1){
    static blocks_t temp_block = 0;
    if (solid_map[playerXOff][playerYOff][playerZOff] == true){
      temp_block = map[playerXOff][playerYOff][playerZOff];
      map[playerXOff][playerYOff][playerZOff] = 0;
      solid_map[playerXOff][playerYOff][playerZOff] = false;
      waterFlow(playerXOff, playerYOff, playerZOff, map);
    }
    return temp_block;
  }
  return 0;
}

void playerPlaceBlock(char map[MAP_WIDTH][MAP_LENGTH][MAP_HEIGHT], blocks_t block){
  playerOffsetDirection();
  if (playerX > 0 && playerX < MAP_WIDTH-1 && playerY > 0 && playerY < MAP_LENGTH-1 && playerZ > 0 && playerZ < MAP_HEIGHT-1){
    if (solid_map[playerXOff][playerYOff][playerZOff] == false){
      map[playerXOff][playerYOff][playerZOff] = block;
    }
  }
}

void handlePlayerMovement(char map[MAP_WIDTH][MAP_LENGTH][MAP_HEIGHT], SDL_Event event){
  switch (event.key.keysym.sym){
    case SDLK_a:
      if (solid_map[playerX][playerY-1][playerZ] == false){
        playerY--;
      }
      break;
    case SDLK_s:
      if (solid_map[playerX+1][playerY][playerZ] == false){
        playerX++;
      }
      break;
    case SDLK_w:
      if (solid_map[playerX-1][playerY][playerZ] == false){
        playerX--;
      }
      break;
    case SDLK_d:
      if (solid_map[playerX][playerY+1][playerZ] == false){
        playerY++;
      }
      break;
    case SDLK_r:
      rotatePlayerClockwise();
      break;
    case SDLK_f:
      rotatePlayerUp();
      break;
    case SDLK_SPACE:
      pthread_create(&jump_thread, NULL, handlePlayerJumping, NULL);
      break;
  }

}
