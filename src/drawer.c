#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <stdlib.h>
#include <drawer.h>
#include <map.h>
#include <camera.h>
#include <player.h>
#include <ui.h>
#include <teams.h>

#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480

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

void drawBlock(int xPos, int yPos, int zPos, int height, blocks_t block, SDL_Renderer * renderer){
  atlas_rect.x = xPos;
  atlas_rect.y = yPos;
  atlas_rect.w = (cameraZoom*2);
  atlas_rect.h = (cameraZoom*3);
  if (block > 0){
    atlas_clip.x = (block * 24) % 384;
    atlas_clip.y = (block / 34);
  } else {
    atlas_clip.x = 0;
    atlas_clip.y = 0;
  }
  if (xPos >= SCREEN_WIDTH+(height*2) || xPos < 0-(height*2) || yPos >= SCREEN_HEIGHT+(height*2) || yPos < 0-(height*2)){
    return;
  }

  if (zPos > 0){
    SDL_SetTextureColorMod(atlas_texture, (zPos*10)+90, (zPos*10)+90,(zPos*10)+90);
  }
  SDL_RenderCopy(renderer, atlas_texture, &atlas_clip, &atlas_rect);
}

void drawWorld(char world[MAP_WIDTH][MAP_LENGTH][MAP_HEIGHT], int height, SDL_Renderer * renderer){
  int iterator = 0;
  int xPos;
  int yPos;

  atlas_surface = SDL_LoadBMP(ATLAS_PATH);
  atlas_texture = SDL_CreateTextureFromSurface(renderer, atlas_surface);
  atlas_clip.w = TILE_WIDTH;
  atlas_clip.h = TILE_HEIGHT;

  for (int n = MAP_HEIGHT-1 ; n >= 0 ; n--){
    for (int i = 0 ; i < MAP_WIDTH ; i++){
      for (int j = 0 ; j < MAP_LENGTH ; j++){
	xPos = ((j*height)-((i*height)-((SCREEN_WIDTH/2)+height+10)))+cameraX;
	yPos = (((j*height)+(i*height)+(n*height))+cameraY);
	if (world[i][j][iterator] == 0){
	  if (i == playerX && j == playerY && iterator == playerZ){
	    if (playerTeam == 0){
	      drawPlayer(PLAYER_SPRITE_RED,xPos,yPos, renderer);
	    } else if (playerTeam == 1){
	      drawPlayer(PLAYER_SPRITE_BLUE,xPos,yPos, renderer);
	    }
	    drawCurrentDirection(xPos, yPos, renderer);
	  }
	  continue;
	} else {

	  if (playerX == i && playerY == j && playerZ == iterator){
	    if (playerTeam == 0){
	      drawPlayer(PLAYER_SPRITE_RED, xPos, yPos, renderer);
	    } else if (playerTeam == 1){
	      drawPlayer(PLAYER_SPRITE_BLUE, xPos, yPos, renderer);
	    }
	  }
	  drawBlock(xPos, yPos, iterator, height, world[i][j][iterator], renderer);
        }
      }
    }
    iterator++;
  }
}

void drawPlayer(char * playerPath, int xPos, int yPos, SDL_Renderer * renderer){
  SDL_Surface * sprite_surface = SDL_LoadBMP(playerPath);
  SDL_Texture * sprite_texture = SDL_CreateTextureFromSurface(renderer, sprite_surface);
  SDL_Rect sprite_area;
  sprite_area.x = xPos;
  sprite_area.y = yPos;
  sprite_area.w = cameraZoom*2;
  sprite_area.h = cameraZoom*2;
  SDL_RenderCopy(renderer, sprite_texture, NULL, &sprite_area);
  SDL_DestroyTexture(sprite_texture);
  SDL_FreeSurface(sprite_surface);
}
