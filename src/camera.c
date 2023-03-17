#include <SDL2/SDL.h>
#include <camera.h>
#include <stdio.h>
#include <stdlib.h>
#include <drawer.h>
#include <player.h>
#include <map.h>

unsigned int cameraX = 0;
unsigned int cameraY = 0;
unsigned int cameraZoom = 8;

void updateCamera(unsigned int xPos, unsigned int yPos, unsigned int zoom, SDL_Renderer * renderer, char world[MAP_WIDTH][MAP_LENGTH][MAP_HEIGHT]){
  cameraX = xPos, cameraY = yPos, cameraZoom = zoom;
  SDL_SetRenderDrawColor(renderer, 0, 32, 255, 255);
  SDL_RenderClear(renderer);
  drawWorld(world, zoom, renderer);
}

void handleCameraMovement(SDL_Event event){
  switch(event.key.keysym.sym){
    case SDLK_z:
      cameraZoom++;
      break;
    case SDLK_x:
      cameraZoom--;
      break;
    case SDLK_UP:
      cameraY+=cameraZoom;
      break;
    case SDLK_DOWN:
      cameraY-=cameraZoom;
      break;
    case SDLK_LEFT:
      cameraX+=(cameraZoom*2);
      break;
    case SDLK_RIGHT:
      cameraX-=(cameraZoom*2);
      break;
  }
}
