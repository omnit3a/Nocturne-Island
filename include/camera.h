#ifndef CAMERA_H_
#define CAMERA_H_

#include <SDL2/SDL.h>

void get_camera_view(int * x, int * y);
void setup_camera(SDL_Renderer * renderer, SDL_Window * window);
void update_camera();

#endif
