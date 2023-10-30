#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <stdlib.h>
#include <drawer.h>
#include <map.h>
#include <camera.h>
#include <player.h>
#include <map_defs.h>
#include <entity.h>
#include <ticks.h>
#include <physics.h>

int view_x = 0;
int view_y = 0;

void draw_view(render_obj_t * object){
  get_camera_view(&view_x, &view_y);
  object->surface = SDL_LoadBMP(ATLAS_PATH);
  object->texture = SDL_CreateTextureFromSurface(object->renderer, object->surface);
  object->clip.w = TILE_WIDTH;
  object->clip.h = TILE_HEIGHT;
  int screen_x = -1;
  int screen_y = -1;
  transform_t pos = get_player_entity()->position;
  int drawing_height = MAP_HEIGHT;
  int draw_indoors = 0;
  
  for (int x = pos.x-(view_x/2) ; x <= pos.x+(view_x/2) ; x++){
    screen_x = x - (pos.x-view_x/2);
    for (int y = pos.y-(view_y/2) ; y <= pos.y+(view_y/2) ; y++){
      screen_y = y - (pos.y-view_y/2);
      int z;
      get_height(&z, x, y);

      if (is_block_shaded(pos.x, pos.y, pos.z-1)){
        z = 0;
	draw_indoors = 1;
      } else {
	draw_indoors = 0;
      }

      do {
	int state = get_block(x, y, z).current_state;
	int block = get_block(x, y, z).block.block[state];
	
	if (get_block(x, y, z).id > 0){
	  object->clip.x = (block % (ATLAS_WIDTH / TILE_WIDTH)) * TILE_WIDTH;
	  object->clip.y = (block / (ATLAS_HEIGHT / TILE_HEIGHT)) * TILE_HEIGHT;
	} else {
	  object->clip.x = 0;
	  object->clip.y = 0;
	}
	
	int brightness = (32 * is_daytime())+((z) * 25);
	if (brightness > 255){
	  brightness = 255;
	}
	
	if (is_block_shaded(x, y, z)){
	  SDL_SetTextureColorMod(object->texture, 64, 64, 64);
	} else {
	  SDL_SetTextureColorMod(object->texture, brightness, brightness, brightness);
	}
	
	object->target.x = (screen_x * DEFAULT_SCREEN_WIDTH/view_x);
	object->target.y = (screen_y * DEFAULT_SCREEN_HEIGHT/view_y);
	object->target.w = DEFAULT_SCREEN_WIDTH/view_x;
	object->target.h = DEFAULT_SCREEN_HEIGHT/view_y;
	SDL_RenderCopy(object->renderer, object->texture, &object->clip, &object->target);
      } while (draw_indoors && z++ != pos.z);
    }
  }
  SDL_FreeSurface(object->surface);
  SDL_DestroyTexture(object->texture);
}
  
/* Draw player */
void draw_player(render_obj_t * object){
  transform_t pos = get_player_entity()->position;

  get_camera_view(&view_x, &view_y);
  object->surface = SDL_LoadBMP(LEVEE_PATH);
  object->texture = SDL_CreateTextureFromSurface(object->renderer, object->surface);

  object->target.x = (DEFAULT_SCREEN_WIDTH/2)-((DEFAULT_SCREEN_WIDTH/view_x)/2);
  object->target.y = (DEFAULT_SCREEN_HEIGHT/2)-((DEFAULT_SCREEN_HEIGHT/view_y)/2);
  object->target.w = DEFAULT_SCREEN_WIDTH/view_x;
  object->target.h = DEFAULT_SCREEN_HEIGHT/view_y;
  object->clip.w = LEVEE_WIDTH;
  object->clip.h = LEVEE_HEIGHT;
  object->clip.x = LEVEE_WIDTH * is_player_jumping();
  object->clip.y = 0;

  int brightness = (32 * is_daytime())+((pos.z) * 25);
  if (brightness > 255){
    brightness = 255;
  }
  if (is_block_shaded(pos.x, pos.y, pos.z-1)){
    SDL_SetTextureColorMod(object->texture, 16, 16, 16);
  } else {
    SDL_SetTextureColorMod(object->texture, brightness, brightness, brightness);                   
  }
  
  SDL_RenderCopy(object->renderer, object->texture, &object->clip, &object->target);
  SDL_DestroyTexture(object->texture);
  SDL_FreeSurface(object->surface);
}
