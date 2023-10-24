#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <drawer.h>
#include <map.h>
#include <camera.h>
#include <player.h>
#include <ui.h>
#include <lighting.h>
#include <map_defs.h>

char * PLAYER_SPRITE = LEVEE_PATH;
bool renderPlayerLast;

int SCREEN_WIDTH = DEFAULT_SCREEN_WIDTH;
int SCREEN_HEIGHT = DEFAULT_SCREEN_HEIGHT;
int xPosBackup = 0, yPosBackup = 0, belowPosBackup = 0;
int iBackup = 0, jBackup = 0;
int blocks_in_view[CAMERA_VIEW][CAMERA_VIEW][MAP_HEIGHT];

int iterator = 0;
SDL_Surface * atlas_surface;
SDL_Texture * atlas_texture;
SDL_Rect atlas_rect = {
  0,
  0,
  ATLAS_WIDTH,
  ATLAS_HEIGHT
};
SDL_Rect atlas_clip = {
  0,
  0,
  TILE_WIDTH,
  TILE_HEIGHT
};
SDL_Rect tile_rect;

void getBlocksInView(char world[MAP_WIDTH][MAP_LENGTH][MAP_HEIGHT]){
  int x_start = playerX-5, x_end = playerX+5;
  int y_start = playerY-5, y_end = playerY+5;
  
  int resultX = 0;
  int resultY = 0;
  int resultZ = 0;
  for (int x = x_start ; x != x_end; x++){
    resultY = 0;
    for (int y = y_start ; y != y_end ; y++){
      resultZ = 0;
      for (int z = 0 ; z < MAP_HEIGHT ; z++){	
        blocks_in_view[resultX][resultY][resultZ] = world[x+1][y+1][z];
	resultZ++;
      }
      resultY++;
    }
    resultX++;
  }
}

void drawSlopes(SDL_Renderer * renderer){
  /* TODO */
}

void drawView(SDL_Renderer * renderer){
  atlas_surface = SDL_LoadBMP(ATLAS_PATH);
  atlas_texture = SDL_CreateTextureFromSurface(renderer, atlas_surface);
  atlas_clip.w = TILE_WIDTH;
  atlas_clip.h = TILE_HEIGHT;
  
  for (int z = -10 ; z < 1 ; z++){
    for (int x = 0 ; x < CAMERA_VIEW ; x++){
      for (int y = 0 ; y < CAMERA_VIEW ; y++){
	int block = blocks_in_view[x][y][playerZ+z];
	
	if (block > 0){
	  atlas_clip.x = (block % (ATLAS_WIDTH / TILE_WIDTH)) * TILE_WIDTH;
	  atlas_clip.y = (block / (ATLAS_HEIGHT / TILE_HEIGHT)) * TILE_HEIGHT;
	} else {
	  atlas_clip.x = 0;
	  atlas_clip.y = 0;
	}

	atlas_rect.x = (x * (SCREEN_WIDTH/CAMERA_VIEW));
	atlas_rect.y = (y * (SCREEN_HEIGHT/CAMERA_VIEW));
	atlas_rect.w = (SCREEN_WIDTH/CAMERA_VIEW);
	atlas_rect.h = (SCREEN_HEIGHT/CAMERA_VIEW);
	SDL_RenderCopy(renderer, atlas_texture, &atlas_clip, &atlas_rect);
      }
    }
  }

  drawSlopes(renderer);

  SDL_FreeSurface(atlas_surface);
  SDL_DestroyTexture(atlas_texture);
  drawPlayer(LEVEE_PATH, renderer);
}
  
/* Draw player */
void drawPlayer(char * playerPath, SDL_Renderer * renderer){
  SDL_Surface * sprite_surface = SDL_LoadBMP(LEVEE_PATH);
  SDL_Texture * sprite_texture = SDL_CreateTextureFromSurface(renderer, sprite_surface);
  SDL_Rect sprite_area;
  sprite_area.x = (SCREEN_WIDTH/2)-((SCREEN_WIDTH/CAMERA_VIEW)/2);
  sprite_area.y = (SCREEN_HEIGHT/2)-((SCREEN_WIDTH/CAMERA_VIEW)/2);
  sprite_area.w = SCREEN_WIDTH/CAMERA_VIEW;
  sprite_area.h = SCREEN_HEIGHT/CAMERA_VIEW;
  SDL_RenderCopy(renderer, sprite_texture, NULL, &sprite_area);
  SDL_DestroyTexture(sprite_texture);
  SDL_FreeSurface(sprite_surface);
}
