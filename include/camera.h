#ifndef CAMERA_H_
#define CAMERA_H_

#include <SDL2/SDL.h>
#include <map.h>
#include <pthread.h>
#include <time.h>

#define CAMERA_VIEW 9
#define CAMERA_SPEED CLOCKS_PER_SEC / 100

extern pthread_mutex_t camera_lock;

void update_camera_map(char world[MAP_WIDTH][MAP_LENGTH][MAP_HEIGHT]);
void setup_camera(SDL_Renderer * renderer, SDL_Window * window);
void update_camera();
void * update_camera_on_tick(void * vargp);

#endif
