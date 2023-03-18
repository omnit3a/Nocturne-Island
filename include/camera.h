#ifndef CAMERA_H_
#define CAMERA_H_

#include <SDL2/SDL.h>
#include <drawer.h>

extern int cameraX;
extern int cameraY;
extern int cameraZoom;

void updateCamera(unsigned int xPos, unsigned int yPos, unsigned int zoom, SDL_Renderer * renderer, char world[MAP_WIDTH][MAP_LENGTH][MAP_HEIGHT], SDL_Window * window);
#endif
