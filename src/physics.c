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

pthread_mutex_t physics_lock;
char physics_map[MAP_WIDTH][MAP_LENGTH][MAP_HEIGHT];
int playerIsJumping = 0;
SDL_Renderer * physics_renderer;

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

void setPhysicsRenderer(SDL_Renderer * renderer){
  physics_renderer = renderer;
}

/* Make the player jump */
void * handlePlayerJumping(void * vargp){
  pthread_mutex_lock(&physics_lock);
  playerIsJumping = 1;
  playerZ++;
  playWav(HITGROUND_SFX_PATH);
  sleep(1);
  playerIsJumping = 0;
  pthread_mutex_unlock(&physics_lock);
  return NULL;
}

/* Constantly run a seperate thread for player gravity */
void * handlePlayerGravity(void * vargp){
  int temp_playerZ;

  while(1){
    pthread_mutex_lock(&physics_lock);
    temp_playerZ = playerZ;
    if (playerIsJumping == 0){
      if(!(getBlockProperties(physics_map, playerX, playerY, temp_playerZ-1).solid)){
        playerZ--;
      }
    }
    pthread_mutex_unlock(&physics_lock);
  }

  return NULL;
}

/* Make water flow into tile if any of its neighbours contain water */
void waterFlow(char map[MAP_WIDTH][MAP_LENGTH][MAP_HEIGHT]){
  /* TODO */
}
