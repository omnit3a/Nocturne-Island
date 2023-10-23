#ifndef CAMERA_H_
#define CAMERA_H_

#include <SDL2/SDL.h>
#include <map.h>
#include <pthread.h>
#include <time.h>

#define CAMERA_SPEED CLOCKS_PER_SEC / 100
#define CAMERA_VIEW 9

extern pthread_mutex_t camera_lock;

extern int cameraX;
extern int cameraY;
extern int cameraZoom;

void setupCameraMap(char world[MAP_WIDTH][MAP_LENGTH][MAP_HEIGHT]);
void setupCamera(SDL_Renderer * renderer, SDL_Window * window);
void updateCamera(unsigned int xPos, unsigned int yPos, unsigned int zoom, SDL_Renderer * renderer, char world[MAP_WIDTH][MAP_LENGTH][MAP_HEIGHT], SDL_Window * window, int free_texture);
void * updateCameraOnTick(void * vargp);

#endif
