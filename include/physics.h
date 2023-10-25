#ifndef PHYSICS_H_
#define PHYSICS_H_
#include <map.h>
#include <unistd.h>
#include <pthread.h>
#include <SDL2/SDL.h>
#include <time.h>

#define PHYSICS_SPEED 100000

extern pthread_mutex_t physics_lock;
extern char physics_map[MAP_WIDTH][MAP_LENGTH][MAP_HEIGHT];

void setPhysicsMap(char map[MAP_WIDTH][MAP_LENGTH][MAP_HEIGHT]);
void * handlePlayerJumping(void * vargp);
void handlePlayerGravity();
void reset_physics();
void handle_physics();

#endif
