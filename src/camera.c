#include <SDL2/SDL.h>
#include <camera.h>
#include <stdio.h>
#include <stdlib.h>
#include <drawer.h>
#include <player.h>
#include <map.h>
#include <unistd.h>

int cameraX = 0;
int cameraY = 0;
int cameraZoom = 32;

void updateCamera(unsigned int xPos, unsigned int yPos, unsigned int zoom, SDL_Renderer * renderer, char world[MAP_WIDTH][MAP_LENGTH][MAP_HEIGHT], SDL_Window * window, int free_texture){
  /* Make the camera follow the player */
  cameraX = -((xPosBackup-xPos)-((SCREEN_WIDTH/2)-TILE_WIDTH));
  cameraY = -((yPosBackup-yPos)-((SCREEN_HEIGHT/2)-TILE_HEIGHT));
  /* Dynamically resize window*/
  SDL_GetWindowSize(window, &SCREEN_WIDTH, &SCREEN_HEIGHT);
  SDL_SetRenderDrawColor(renderer, 0, 0, 50, 255);
  SDL_RenderClear(renderer);
  drawWorld(world, cameraZoom, renderer, free_texture);
}

