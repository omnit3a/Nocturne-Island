#ifndef PHYSICS_H_
#define PHYSICS_H_
#include <map.h>
#include <unistd.h>
#include <pthread.h>
#include <SDL2/SDL.h>

extern pthread_mutex_t physics_lock;
extern char physics_map[MAP_WIDTH][MAP_LENGTH][MAP_HEIGHT];

void setPhysicsMap(char map[MAP_WIDTH][MAP_LENGTH][MAP_HEIGHT]);
void setPhysicsRenderer(SDL_Renderer * renderer);
void * handlePlayerJumping(void * vargp);
void * handlePlayerGravity(void * vargp);
void waterFlow(int xPos, int yPos, int zPos, char map[MAP_WIDTH][MAP_LENGTH][MAP_HEIGHT]);

#endif
