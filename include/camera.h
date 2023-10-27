#ifndef CAMERA_H_
#define CAMERA_H_

#include <SDL2/SDL.h>
#include <map.h>
#include <pthread.h>
#include <time.h>

#define CAMERA_SPEED CLOCKS_PER_SEC / 100

void get_camera_view(int * x, int * y);
void setup_camera(SDL_Renderer * renderer, SDL_Window * window);
void update_camera();

#endif
