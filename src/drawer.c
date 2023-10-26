#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <stdlib.h>
#include <drawer.h>
#include <map.h>
#include <camera.h>
#include <player.h>
#include <map_defs.h>
#include <math.h>

int SCREEN_WIDTH = DEFAULT_SCREEN_WIDTH;
int SCREEN_HEIGHT = DEFAULT_SCREEN_HEIGHT;
int view_x = 0;
int view_y = 0;

void draw_slope_overlay(render_obj_t * object, render_obj_t * overlay){
  /* FIXME */

  get_camera_view(&view_x, &view_y);
  overlay->surface = SDL_LoadBMP(WALL_PATH);
  overlay->texture = SDL_CreateTextureFromSurface(overlay->renderer, overlay->surface);
  overlay->clip.w = TILE_WIDTH;
  overlay->clip.h = TILE_HEIGHT;
  overlay->target.w = SCREEN_WIDTH/view_x;
  overlay->target.h = SCREEN_HEIGHT/view_y;
  overlay->clip.x = 0;
  overlay->clip.y = 0;
  int screen_x = 0;
  int screen_y = 0;
  
  for (int z = -10 ; z < 1 ; z++){
    screen_x = 0;
    for (int x = playerX-(view_x/2) ; x <= playerX+(view_x/2) ; x++){
      screen_y = 0;
      for (int y = playerY-(view_y/2) ; y <= playerY+(view_y/2) ; y++){
	
	if (world_map[x][y][playerZ+z] == 0){
	  continue;
	}

	overlay->target.x = (screen_x * (SCREEN_WIDTH/view_x));
	overlay->target.y = (screen_y * (SCREEN_HEIGHT/view_y));

	if (world_map[x-1][y][playerZ+z] == 0){
	  overlay->clip.x = 0;
	  SDL_RenderCopy(overlay->renderer, overlay->texture, &overlay->clip, &overlay->target);
	}

	if (world_map[x][y-1][playerZ+z] == 0){
	  overlay->clip.x = 16;
	  SDL_RenderCopy(overlay->renderer, overlay->texture, &overlay->clip, &overlay->target);
	}
	if (world_map[x+1][y][playerZ+z] == 0){
	  overlay->clip.x = 32;
	  SDL_RenderCopy(overlay->renderer, overlay->texture, &overlay->clip, &overlay->target);
	}
	if (world_map[x][y+1][playerZ+z] == 0){
	  overlay->clip.x = 48;
	  SDL_RenderCopy(overlay->renderer, overlay->texture, &overlay->clip, &overlay->target);
	}
	screen_y++;
      }
      screen_x++;
    }
  }
  SDL_BlitSurface(overlay->surface, &overlay->target, object->surface, &object->target);
  SDL_FreeSurface(overlay->surface);
  SDL_DestroyTexture(overlay->texture); 
}

void draw_view(render_obj_t * object){
  get_camera_view(&view_x, &view_y);
  object->surface = SDL_LoadBMP(ATLAS_PATH);
  object->texture = SDL_CreateTextureFromSurface(object->renderer, object->surface);
  object->clip.w = TILE_WIDTH;
  object->clip.h = TILE_HEIGHT;
  int screen_x = 0;
  int screen_y = 0;
  
  for (int z = -10 ; z < 1 ; z++){
    screen_x = 0;
    for (int x = playerX-(view_x/2) ; x <= playerX+(view_x/2) ; x++){
      screen_y = 0;
      for (int y = playerY-(view_y/2) ; y <= playerY+(view_y/2) ; y++){
	int block_state = states_map[x][y][playerZ+z];
	
	int block = getBlockProperties(world_map[x][y][playerZ+z]).block[block_state];
	int is_transparent = getBlockProperties(world_map[x][y][playerZ+z]).transparent;

	if (!is_transparent){
	  object->clip.x = (block % (ATLAS_WIDTH / TILE_WIDTH)) * TILE_WIDTH;
	  object->clip.y = (block / (ATLAS_HEIGHT / TILE_HEIGHT)) * TILE_HEIGHT;
	} else {
	  object->clip.x = 0;
	  object->clip.y = 0;
	}

	int brightness = 64+((playerZ+z) * 25);
	if (brightness > 255){
	  brightness = 255;
	}

	if (is_block_underground(x, y, playerZ+z)){
	  SDL_SetTextureColorMod(object->texture, 64, 64, 64);
	} else {
	  SDL_SetTextureColorMod(object->texture, brightness, brightness, brightness);
	}
	
        object->target.x = (screen_x * (SCREEN_WIDTH/view_x));
        object->target.y = (screen_y * (SCREEN_HEIGHT/view_y));
        object->target.w = SCREEN_WIDTH/view_x;
        object->target.h = SCREEN_HEIGHT/view_y;
	SDL_RenderCopy(object->renderer, object->texture, &object->clip, &object->target);
	screen_y++;
      }
      screen_x++;
    }
  }
  SDL_FreeSurface(object->surface);
  SDL_DestroyTexture(object->texture);
}
  
/* Draw player */
void draw_player(render_obj_t * object){
  get_camera_view(&view_x, &view_y);
  object->surface = SDL_LoadBMP(LEVEE_PATH);
  object->texture = SDL_CreateTextureFromSurface(object->renderer, object->surface);

  object->target.x = (SCREEN_WIDTH/2)-((SCREEN_WIDTH/view_x)/2);
  object->target.y = (SCREEN_HEIGHT/2)-((SCREEN_WIDTH/view_y)/2);
  object->target.w = SCREEN_WIDTH/view_x;
  object->target.h = SCREEN_HEIGHT/view_y;

  SDL_RenderCopy(object->renderer, object->texture, NULL, &object->target);
  SDL_DestroyTexture(object->texture);
  SDL_FreeSurface(object->surface);
}
