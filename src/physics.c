#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <physics.h>
#include <player.h>
#include <map.h>
#include <ticks.h>

char physics_map[MAP_WIDTH][MAP_LENGTH][MAP_HEIGHT];
int playerIsJumping = 0;

int ticked = 0;
int jump_timeout = 0;

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
void player_jump(){
  jump_timeout = SDL_GetTicks() + JUMP_LENGTH;
  playerIsJumping = 1;
  playerZ++;
}

void handle_gravity(){
  if (playerIsJumping == 0){
    if(!getBlockProperties(physics_map[playerX][playerY][playerZ-1]).solid){
      playerZ--;
    }
  }
}

void reset_physics(){
  ticked = 0;
  if (SDL_GetTicks() >= jump_timeout){
    playerIsJumping = 0;
  }
}

void handle_physics(){
  if (ticked == 0){
    handle_gravity();
    ticked = 1;
  }
}
