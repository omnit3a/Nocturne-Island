#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <physics.h>
#include <player.h>
#include <map.h>
#include <time.h>
#include <ticks.h>

pthread_mutex_t physics_lock;
char physics_map[MAP_WIDTH][MAP_LENGTH][MAP_HEIGHT];
int playerIsJumping = 0;
clock_t currentTick = 0;
pthread_t wait_id;

int ticked = 0;

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
  if (playerIsJumping == 0){
    if(!getBlockProperties(physics_map[playerX][playerY][playerZ-1]).solid){
      playerZ--;
    }
  }
}

void reset_physics(){
  ticked = 0;
}

void handle_physics(){
  if (ticked == 0){
    handlePlayerGravity();
    ticked = 1;
  }
}
