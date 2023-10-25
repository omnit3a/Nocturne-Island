#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <stdlib.h>
#include <drawer.h>
#include <map.h>
#include <camera.h>
#include <player.h>
#include <map_defs.h>

int SCREEN_WIDTH = DEFAULT_SCREEN_WIDTH;
int SCREEN_HEIGHT = DEFAULT_SCREEN_HEIGHT;
char blocks_in_view[CAMERA_VIEW][CAMERA_VIEW][MAP_HEIGHT];

void get_blocks_in_view(char world[MAP_WIDTH][MAP_LENGTH][MAP_HEIGHT]){
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

void draw_slope_overlay(render_obj_t * object, render_obj_t * overlay_object){
  /* TODO */
}

void draw_view(render_obj_t * object){
  object->surface = SDL_LoadBMP(ATLAS_PATH);
  object->texture = SDL_CreateTextureFromSurface(object->renderer, object->surface);
  object->clip.w = TILE_WIDTH;
  object->clip.h = TILE_HEIGHT;
  object->target.w = ATLAS_WIDTH;
  object->target.h = ATLAS_HEIGHT;
  
  for (int z = -10 ; z < 1 ; z++){
    for (int x = 0 ; x < CAMERA_VIEW ; x++){
      for (int y = 0 ; y < CAMERA_VIEW ; y++){
	int block = getBlockProperties(blocks_in_view[x][y][playerZ+z]).block;

	if (block > 0){
	  object->clip.x = (block % (ATLAS_WIDTH / TILE_WIDTH)) * TILE_WIDTH;
	  object->clip.y = (block / (ATLAS_HEIGHT / TILE_HEIGHT)) * TILE_HEIGHT;
	} else {
	  object->clip.x = 0;
	  object->clip.y = 0;
	}

        object->target.x = (x * (SCREEN_WIDTH/CAMERA_VIEW));
        object->target.y = (y * (SCREEN_HEIGHT/CAMERA_VIEW));
        object->target.w = (SCREEN_WIDTH/CAMERA_VIEW);
        object->target.h = (SCREEN_HEIGHT/CAMERA_VIEW);
	SDL_RenderCopy(object->renderer, object->texture, &object->clip, &object->target);
      }
    }
  }
  SDL_FreeSurface(object->surface);
  SDL_DestroyTexture(object->texture);
}
  
/* Draw player */
void draw_player(render_obj_t * object){
  object->surface = SDL_LoadBMP(LEVEE_PATH);
  object->texture = SDL_CreateTextureFromSurface(object->renderer, object->surface);

  object->target.x = (SCREEN_WIDTH/2)-((SCREEN_WIDTH/CAMERA_VIEW)/2);
  object->target.y = (SCREEN_HEIGHT/2)-((SCREEN_WIDTH/CAMERA_VIEW)/2);
  object->target.w = SCREEN_WIDTH/CAMERA_VIEW;
  object->target.h = SCREEN_HEIGHT/CAMERA_VIEW;

  SDL_RenderCopy(object->renderer, object->texture, NULL, &object->target);
  SDL_DestroyTexture(object->texture);
  SDL_FreeSurface(object->surface);
}
