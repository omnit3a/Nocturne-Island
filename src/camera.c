#include <SDL2/SDL.h>
#include <camera.h>
#include <stdio.h>
#include <stdlib.h>
#include <drawer.h>
#include <player.h>
#include <map.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>
#include <ui.h>

pthread_mutex_t camera_lock;

int cameraX = 0;
int cameraY = 0;
int cameraZoom = 32;
SDL_Renderer * camera_renderer;
char camera_map[MAP_WIDTH][MAP_LENGTH][MAP_HEIGHT];
SDL_Window * camera_window;

void setupCameraMap(char world[MAP_WIDTH][MAP_LENGTH][MAP_HEIGHT]){
  for (int x = 0 ; x < MAP_WIDTH ; x++){
    for (int y = 0 ; y < MAP_LENGTH ; y++){
      for (int z = 0 ; z < MAP_HEIGHT ; z++){
	camera_map[x][y][z] = world[x][y][z];
      }
    }
  }
}
void setupCamera(SDL_Renderer * renderer, SDL_Window * window){
  camera_renderer = renderer;
  camera_window = window;
}

void updateCamera(unsigned int xPos, unsigned int yPos, unsigned int zoom, SDL_Renderer * renderer, char world[MAP_WIDTH][MAP_LENGTH][MAP_HEIGHT], SDL_Window * window, int free_texture){
  /* Make the camera follow the player */
  cameraX = -((xPosBackup-xPos)-((SCREEN_WIDTH/2)-TILE_WIDTH));
  cameraY = -((yPosBackup-yPos)-((SCREEN_HEIGHT/2)-TILE_HEIGHT));
  /* Dynamically resize window*/
  SDL_GetWindowSize(window, &SCREEN_WIDTH, &SCREEN_HEIGHT);
  SDL_SetRenderDrawColor(renderer, 0, 0, 50, 255);
  SDL_RenderClear(renderer);
  setupCameraMap(world);
  getBlocksInView(world);
  drawView(renderer);
}

void * updateCameraOnTick(void * vargp){
  clock_t currentClock;
  clock_t currentTick;

  while(1){
    currentClock = clock() % CLOCKS_PER_SEC;
    currentTick = currentClock % CAMERA_SPEED;
    if (currentTick % 50 == 0){
      pthread_mutex_lock(&camera_lock);
      updateCamera(cameraX, cameraY, cameraZoom, camera_renderer, camera_map, camera_window, 1);
      draw_ui(camera_renderer);
      SDL_RenderPresent(camera_renderer);
      pthread_mutex_unlock(&camera_lock);
    }
  }

  return NULL;
}
