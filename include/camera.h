#ifndef CAMERA_H_
#define CAMERA_H_

#include <SDL2/SDL.h>
#include <drawer.h>

extern unsigned int cameraX;
extern unsigned int cameraY;
extern unsigned int cameraZoom;

void updateCamera(unsigned int xPos, unsigned int yPos, unsigned int zoom, SDL_Renderer * renderer, char world[MAP_WIDTH][MAP_LENGTH][MAP_HEIGHT], SDL_Window * window);
void handleCameraMovement(SDL_Event event);
#endif
