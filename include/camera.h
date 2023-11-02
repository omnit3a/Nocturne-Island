#ifndef CAMERA_H_
#define CAMERA_H_

#include <SDL2/SDL.h>

#define CAMERA_X_VIEW 13
#define CAMERA_Y_VIEW 9

void get_camera_view(int * x, int * y);
void setup_camera(SDL_Renderer * renderer, SDL_Window * window);
void update_camera();

#endif
