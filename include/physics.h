#ifndef PHYSICS_H_
#define PHYSICS_H_
#include <map.h>
#include <SDL2/SDL.h>

#define JUMP_LENGTH 500

void player_jump();
void handle_gravity();
void reset_physics();
void handle_physics();

#endif
