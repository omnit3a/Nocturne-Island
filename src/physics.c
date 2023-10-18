#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <physics.h>
#include <player.h>
#include <camera.h>
#include <map.h>
#include <audio.h>
#include <time.h>
#include <ui.h>

pthread_mutex_t physics_lock;
char physics_map[MAP_WIDTH][MAP_LENGTH][MAP_HEIGHT];
int playerIsJumping = 0;
SDL_Renderer * physics_renderer;
SDL_Window * physics_window;
clock_t currentTick = 0;
pthread_t wait_id;

/* Save world map for physics usage */
void setPhysicsMap(char map[MAP_WIDTH][MAP_LENGTH][MAP_HEIGHT]){
  for (int i = 0 ; i < MAP_WIDTH ; i++){
    for (int j = 0 ; j < MAP_LENGTH ; j++){
      for (int n = 0 ; n < MAP_HEIGHT ; n++){
        physics_map[i][j][n] = map[i][j][n];
      }
    }
  }
}

void setPhysicsRenderer(SDL_Renderer * renderer, SDL_Window * window){
  physics_renderer = renderer;
  physics_window = window;
}

/* Make the player jump */
void * handlePlayerJumping(void * vargp){
  pthread_mutex_lock(&physics_lock);
  playerIsJumping = 1;
  playerZ++;
  pthread_mutex_unlock(&physics_lock);

  sleep(1);
  
  pthread_mutex_lock(&physics_lock);
  playerIsJumping = 0;
  pthread_mutex_unlock(&physics_lock);
  return NULL;
}

/* Constantly run a seperate thread for player gravity */
void handlePlayerGravity(){
  pthread_mutex_lock(&physics_lock);
  if (playerIsJumping == 0){
    if(!(getBlockProperties(physics_map, playerX, playerY, playerZ-1).solid)){
      playerZ--;
    }
  }
  pthread_mutex_unlock(&physics_lock);
}

/* Make water flow into tile if any of its neighbours contain water */
void waterFlow(char map[MAP_WIDTH][MAP_LENGTH][MAP_HEIGHT]){
  /* TODO */
}


void physicsWait(){
  // TODO
}

void * handlePhysics(void * vargp){
  clock_t currentClock;
  clock_t physicsTick;
  
  while(1){
    currentClock = clock() % CLOCKS_PER_SEC;
    physicsTick = currentClock % PHYSICS_SPEED;
    if (physicsTick == 0){
      pthread_mutex_lock(&physics_lock);
      currentTick++;
      pthread_mutex_unlock(&physics_lock);
      handlePlayerGravity();
    }
  }
}
