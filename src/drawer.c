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

char * PLAYER_SPRITE = LEVEE_PATH;
bool renderPlayerLast;

int SCREEN_WIDTH = DEFAULT_SCREEN_WIDTH;
int SCREEN_HEIGHT = DEFAULT_SCREEN_HEIGHT;

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
bool blockingPlayerCheck(){
  int iteratorX = 0;
  int iteratorY = 0;
  int iteratorXY = 0;
  for (int reach = 0 ; reach < 3 ; reach++){
    for (int i = playerZ ; i < playerZ+5 ; i++){
      if (solid_map[playerX+reach][playerY][i]){
	iteratorX++;
      }
      if (solid_map[playerX][playerY+reach][i]){
	iteratorY++;
      }
      if (solid_map[playerX+reach][playerY+reach][i]){
	iteratorXY++;
      }
    }
  }
  if (solid_map[playerX][playerY][playerZ+1]){
    iteratorX+=2;
    iteratorY+=2;
    iteratorXY+=2;
  }
  if ((iteratorX > 3 && iteratorY > 3) || iteratorXY > 3){
    PLAYER_SPRITE = LEVEE_OUTLINE_PATH;
    renderPlayerLast = true;
    return true;
  } else {
    PLAYER_SPRITE = LEVEE_PATH;
    renderPlayerLast = false;
    return false;
  }
}

void drawBlock(int xPos, int yPos, int zPos, int height, blocks_t block, SDL_Renderer * renderer){
  atlas_rect.x = xPos;
  atlas_rect.y = yPos;
  atlas_rect.w = (cameraZoom*2);
  atlas_rect.h = (cameraZoom*3);
  if (block > 0){
    atlas_clip.x = (block * TILE_WIDTH) % 384;
    atlas_clip.y = (block / TILE_HEIGHT);
  } else {
    atlas_clip.x = 0;
    atlas_clip.y = 0;
  }
  if (xPos >= SCREEN_WIDTH+(height*3) || xPos < 0-(height*3) || yPos >= SCREEN_HEIGHT+(height*3) || yPos < 0-(height*3)){
    return;
  }
  
  if (zPos > -1){
    SDL_SetTextureColorMod(atlas_texture
			   ,((zPos * 10)+BASE_DEPTH_BRIGHTNESS)
			   ,((zPos * 10)+BASE_DEPTH_BRIGHTNESS)
			   ,((zPos * 10)+BASE_DEPTH_BRIGHTNESS));
  }
  SDL_RenderCopy(renderer, atlas_texture, &atlas_clip, &atlas_rect);
}

void drawWorld(char world[MAP_WIDTH][MAP_LENGTH][MAP_HEIGHT], int height, SDL_Renderer * renderer){
  int iterator = 0;
  int xPos;
  int belowPos;
  int yPos;
  int xPosBackup, yPosBackup, belowPosBackup;

  atlas_surface = SDL_LoadBMP(ATLAS_PATH);
  atlas_texture = SDL_CreateTextureFromSurface(renderer, atlas_surface);
  atlas_clip.w = TILE_WIDTH;
  atlas_clip.h = TILE_HEIGHT;

  for (int n = MAP_HEIGHT-1 ; n >= 0 ; n--){
    for (int i = 0 ; i < MAP_WIDTH ; i++){
      for (int j = 0 ; j < MAP_LENGTH ; j++){
	xPos = ((j*height)-((i*height)-((SCREEN_WIDTH/2)+height+10)))+cameraX;
	belowPos = ((j*height)+(i*height)+((n+1)*height))+cameraY;
	yPos = ((j*height)+(i*height)+(n*height))+cameraY;
	if (world[i][j][iterator] == 0){
	  if (i == playerX && j == playerY && iterator == playerZ){
	    drawBlock(xPos, belowPos, iterator, height, BLOCK_OUTLINE, renderer);
	    drawPlayer(PLAYER_SPRITE,xPos,yPos, renderer);
	    drawCurrentDirection(xPos, yPos, renderer);

	    /* Save player render position for renderPlayerLast */
	    xPosBackup = xPos;
	    yPosBackup = yPos;
	    belowPosBackup = belowPos;
	  }
	  continue;
	} else {

	  if (playerX == i && playerY == j && playerZ == iterator){
	    drawPlayer(PLAYER_SPRITE, xPos, yPos, renderer);
	    drawBlock(xPos, belowPos, iterator, height, BLOCK_OUTLINE, renderer);
	    drawCurrentDirection(xPos, yPos, renderer);

	    /* Save player render position for renderPlayerLast */ 
	    xPosBackup = xPos;
	    yPosBackup = yPos;
	    belowPosBackup = belowPos;
	  }
	  drawBlock(xPos, yPos, iterator, height, world[i][j][iterator], renderer);
        }
      }
    }
    iterator++;
  }
  if (renderPlayerLast){
    drawBlock(xPosBackup, belowPosBackup, iterator, height, BLOCK_OUTLINE, renderer);
    drawPlayer(PLAYER_SPRITE, xPosBackup, yPosBackup, renderer);
    drawCurrentDirection(xPosBackup, yPosBackup, renderer);
  }
  SDL_FreeSurface(atlas_surface);
  SDL_DestroyTexture(atlas_texture);
}

void drawPlayer(char * playerPath, int xPos, int yPos, SDL_Renderer * renderer){
  SDL_Surface * sprite_surface = SDL_LoadBMP(playerPath);
  SDL_Texture * sprite_texture = SDL_CreateTextureFromSurface(renderer, sprite_surface);
  SDL_Rect sprite_area;
  sprite_area.x = xPos;
  sprite_area.y = yPos;
  sprite_area.w = cameraZoom*2;
  sprite_area.h = cameraZoom*2;
  SDL_SetTextureColorMod(sprite_texture,
			 (playerZ * 10)+BASE_DEPTH_BRIGHTNESS,
			 (playerZ * 10)+BASE_DEPTH_BRIGHTNESS,
			 (playerZ * 10)+BASE_DEPTH_BRIGHTNESS
			 );
  SDL_RenderCopy(renderer, sprite_texture, NULL, &sprite_area);
  SDL_DestroyTexture(sprite_texture);
  SDL_FreeSurface(sprite_surface);
}
