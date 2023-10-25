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

char camera_map[MAP_WIDTH][MAP_LENGTH][MAP_HEIGHT];
SDL_Renderer * camera_renderer;
render_obj_t camera_object;

void update_camera_map(char world[MAP_WIDTH][MAP_LENGTH][MAP_HEIGHT]){
  for (int x = 0 ; x < MAP_WIDTH ; x++){
    for (int y = 0 ; y < MAP_LENGTH ; y++){
      for (int z = 0 ; z < MAP_HEIGHT ; z++){
	camera_map[x][y][z] = world[x][y][z];
      }
    }
  }
}

void setup_camera(SDL_Renderer * renderer, SDL_Window * window){
  camera_object.window = window;
  camera_object.renderer = renderer;
}

void update_camera(){
  /* Dynamically resize window*/
  SDL_GetWindowSize(camera_object.window, &SCREEN_WIDTH, &SCREEN_HEIGHT);
  SDL_SetRenderDrawColor(camera_object.renderer, 0, 0, 50, 255);
  SDL_RenderClear(camera_object.renderer);

  get_blocks_in_view(camera_map);
  
  draw_view(&camera_object);
  draw_player(&camera_object);
  draw_ui(&camera_object);
}

void * update_camera_on_tick(void * vargp){
  clock_t currentClock;
  clock_t currentTick;

  while(1){
    currentClock = clock() % CLOCKS_PER_SEC;
    currentTick = currentClock % CAMERA_SPEED;
    if (currentTick % 10 == 0){
      pthread_mutex_lock(&camera_lock);
      
      update_camera();      
      SDL_RenderPresent(camera_object.renderer);
      
      pthread_mutex_unlock(&camera_lock);
    }
  }

  return NULL;
}
