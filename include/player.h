#ifndef PLAYER_H_
#define PLAYER_H_

#include <SDL2/SDL.h>
#include <drawer.h>
#include <stdbool.h>
#include <map.h>

#define SPAWN_X MAP_WIDTH/2
#define SPAWN_Y MAP_LENGTH/2
#define SPAWN_Z 11

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

void spawn_player();
int player_border_check();
int get_mining_speed();
void set_player_rotation(rotation_t rotation);
void player_offset_direction();
void player_mine_block();
void player_place_block(int block);
void handle_player_movement(SDL_Event event);
void handle_player_rotation(SDL_Event event);

#endif
