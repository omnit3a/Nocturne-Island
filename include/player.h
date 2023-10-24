#ifndef PLAYER_H_
#define PLAYER_H_

#include <SDL2/SDL.h>
#include <drawer.h>
#include <stdbool.h>

typedef enum rotation_e {
  NORTH = 0,
  EAST = 1,
  SOUTH = 2,
  WEST = 3,
} rotation_t;  

typedef enum z_rotation_e {
  UP = 0,
  STRAIGHT = 1,
  DOWN = 2,
} z_rotation_t;

extern int playerX;
extern int playerY;
extern int playerZ;
extern int playerDirectionX;
extern int playerDirectionY;
extern rotation_t playerRotation;
extern z_rotation_t playerZRotation;
extern int playerXOff;
extern int playerYOff;
extern int playerZOff;
extern int playerHealth;

int getMiningSpeed(char map[MAP_WIDTH][MAP_LENGTH][MAP_HEIGHT]);
void movePlayer(int xOff, int yOff, int zOff, SDL_Renderer * renderer);
void setPlayerRotation(rotation_t rotation);
void rotatePlayerClockwise();
void rotatePlayerUp();
void playerOffsetDirection();
int playerMineBlock(char map[MAP_WIDTH][MAP_LENGTH][MAP_HEIGHT]);
void playerPlaceBlock(char map[MAP_WIDTH][MAP_LENGTH][MAP_HEIGHT], int block);
void handlePlayerMovement(char map[MAP_WIDTH][MAP_LENGTH][MAP_HEIGHT], SDL_Event event);
void handlePlayerRotation(SDL_Event event);

#endif
