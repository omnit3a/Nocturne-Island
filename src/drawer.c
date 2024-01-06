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
	transform_t pos = get_player_entity()->position;
	int draw_indoors = 0;
	int drawing_height = 0;

	for (int x = 0 ; x < CHUNK_WIDTH ; x++){
		for (int y = 0 ; y < CHUNK_LENGTH ; y++){
			for (int z = 0 ; z < CHUNK_HEIGHT ; z++){
    
				int block = get_block(x, y, z).block.texture;
				
				if (get_block(x, y, z).id > 0){
					object->clip.x = (block % (ATLAS_WIDTH / TILE_WIDTH)) * TILE_WIDTH;
					object->clip.y = (block / (ATLAS_HEIGHT / TILE_HEIGHT)) * TILE_HEIGHT;						
				} else {
					object->clip.x = 0;
					object->clip.y = 0;
				}

				if (x == CHUNK_WIDTH / 2 && y == CHUNK_LENGTH / 2 && z == pos.z){
				}
				
				int brightness = (32 * is_daytime())+((z) * 20);
				if (brightness > 255){
					brightness = 255;
				}

				float z_pos = z * 0.4;
				float x_pos = x - z_pos;
			        float y_pos = y - z_pos;
				
				object->target.x = (x_pos * (DEFAULT_SCREEN_WIDTH/view_x)) / 1.5;
				object->target.y = (y_pos * (DEFAULT_SCREEN_HEIGHT/view_y)) / 1.5;
				object->target.w = DEFAULT_SCREEN_WIDTH/view_x;
				object->target.h = DEFAULT_SCREEN_HEIGHT/view_y;
				SDL_RenderCopy(object->renderer, object->texture, &object->clip, &object->target);
			}
		}
	}
	SDL_FreeSurface(object->surface);
	SDL_DestroyTexture(object->texture);
}
  
/* Draw player */
void draw_player(render_obj_t * object){
	transform_t pos = get_player_entity()->position;
	transform_t rot = get_player_direction();
	int stage = -1;
	
	get_camera_view(&view_x, &view_y);    
	object->surface = SDL_LoadBMP(LEVEE_BODY_PATH);
	object->texture = SDL_CreateTextureFromSurface(object->renderer, object->surface);

	float z_pos = pos.z * 0.4;
	float x_pos = CHUNK_WIDTH/2 - z_pos;
	float y_pos = CHUNK_LENGTH/2 - z_pos;
    
	object->target.x = (x_pos * (DEFAULT_SCREEN_WIDTH/view_x)) / 1.5;
	object->target.y = (y_pos * (DEFAULT_SCREEN_HEIGHT/view_y)) / 1.5;
	object->target.x += 8;
	object->target.y -= 8;
	object->target.w = DEFAULT_SCREEN_WIDTH/view_x;
	object->target.h = DEFAULT_SCREEN_HEIGHT/view_y;

	object->clip.w = LEVEE_WIDTH;
	object->clip.h = LEVEE_HEIGHT;
	object->clip.x = get_player_entity()->sprite.frame_offset * LEVEE_WIDTH;
	object->clip.y = 0;

	if (is_player_jumping()){
	}
    
	SDL_RenderCopy(object->renderer, object->texture, &object->clip, &object->target);
	SDL_FreeSurface(object->surface);
	SDL_DestroyTexture(object->texture);

}
