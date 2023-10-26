#ifndef PHYSICS_H_
#define PHYSICS_H_
#include <map.h>
#include <SDL2/SDL.h>

#define JUMP_LENGTH 500

extern char physics_map[MAP_WIDTH][MAP_LENGTH][MAP_HEIGHT];

void setPhysicsMap(char map[MAP_WIDTH][MAP_LENGTH][MAP_HEIGHT]);
void player_jump();
void handle_gravity();
void reset_physics();
void handle_physics();

#endif
