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

int *** getBlocksInView(char world[MAP_WIDTH][MAP_LENGTH][MAP_HEIGHT]){
  static int result[9][9][256] = {0};
  int dist = CAMERA_VIEW_DISTANCE;
  int x_start = playerX-5, x_end = playerX+5;
  int y_start = playerY-5, y_end = playerY+5;
  
  int resultX = 0;
  int resultY = 0;
  int resultZ = 0;
  for (int x = x_start ; x != x_end; x++){
    resultY = 0;
    for (int y = y_start ; y != y_end ; y++){
      resultZ = 0;
      for (int z = 0 ; z < 256 ; z++){	
	result[resultX][resultY][resultZ] = world[x+1][y+1][z];
	resultZ++;
      }
      resultY++;
    }
    resultX++;
  }

  return result;
}

void drawView(int blocks_in_view[9][9][256], SDL_Renderer * renderer){
  int distance = CAMERA_VIEW_DISTANCE;
  
  atlas_surface = SDL_LoadBMP(ATLAS_PATH);
  atlas_texture = SDL_CreateTextureFromSurface(renderer, atlas_surface);
  atlas_clip.w = TILE_WIDTH;
  atlas_clip.h = TILE_HEIGHT;
  atlas_clip.y = 0;

  int brightness[3] = {255, 175, 100};
  
  for (int z = -20 ; z < 1 ; z++){
    for (int x = 0 ; x < 9 ; x++){
      for (int y = 0 ; y < 9 ; y++){
	int block = blocks_in_view[x][y][playerZ+z];
	
	SDL_SetTextureColorMod(atlas_texture,
			       (z * 10)+BASE_DEPTH_BRIGHTNESS,
			       (z * 10)+BASE_DEPTH_BRIGHTNESS,
			       (z * 10)+BASE_DEPTH_BRIGHTNESS);
	if (block > 0){
	  atlas_clip.x = (block % (ATLAS_WIDTH / TILE_WIDTH)) * TILE_WIDTH;
	} else {
	  atlas_clip.x = 0;
	}
	
	atlas_rect.x = (x * (SCREEN_WIDTH/9));
	atlas_rect.y = (y * (SCREEN_HEIGHT/9));
	atlas_rect.w = (SCREEN_WIDTH/9);
	atlas_rect.h = (SCREEN_HEIGHT/9);
	SDL_RenderCopy(renderer, atlas_texture, &atlas_clip, &atlas_rect);
      }
    }
  }
  SDL_FreeSurface(atlas_surface);
  SDL_DestroyTexture(atlas_texture);
  drawPlayer(LEVEE_PATH, renderer);
}
  
/* Draw player */
void drawPlayer(char * playerPath, SDL_Renderer * renderer){
  SDL_Surface * sprite_surface = SDL_LoadBMP(playerPath);
  SDL_Texture * sprite_texture = SDL_CreateTextureFromSurface(renderer, sprite_surface);
  SDL_Rect sprite_area;
  sprite_area.x = (SCREEN_WIDTH/2)-((SCREEN_WIDTH/9)/2);
  sprite_area.y = (SCREEN_HEIGHT/2)-((SCREEN_WIDTH/9)/2);
  sprite_area.w = SCREEN_WIDTH/9;
  sprite_area.h = SCREEN_HEIGHT/9;
  /* Use this to change the lighting of the player */
  SDL_SetTextureColorMod(sprite_texture,
			 (playerZ * 10)+BASE_DEPTH_BRIGHTNESS,
			 (playerZ * 10)+BASE_DEPTH_BRIGHTNESS,
			 (playerZ * 10)+BASE_DEPTH_BRIGHTNESS
			 );
  SDL_RenderCopy(renderer, sprite_texture, NULL, &sprite_area);
  SDL_DestroyTexture(sprite_texture);
  SDL_FreeSurface(sprite_surface);
}
