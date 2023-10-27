#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <physics.h>
#include <player.h>
#include <map.h>
#include <ticks.h>

int playerIsJumping = 0;

int ticked = 0;
int jump_timeout = 0;

/* Make the player jump */
void player_jump(){
  jump_timeout = SDL_GetTicks() + JUMP_LENGTH;
  playerIsJumping = 1;
  playerZ++;
}

void handle_gravity(){
  if (playerIsJumping == 0){
    if(!get_block(playerX, playerY, playerZ-1).block.solid){
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
