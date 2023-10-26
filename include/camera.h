#ifndef CAMERA_H_
#define CAMERA_H_

#include <SDL2/SDL.h>
#include <drawer.h>
#include <pthread.h>
#include <time.h>

#define CAMERA_VIEW 9
#define CAMERA_SPEED CLOCKS_PER_SEC / 100

void update_camera_map(char world[MAP_WIDTH][MAP_LENGTH][MAP_HEIGHT]);
void setup_camera(SDL_Renderer * renderer, SDL_Window * window);
void update_camera();

#endif
