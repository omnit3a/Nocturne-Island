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

const float height_offset = 0.525;
const float block_offset = 1.525;
	
int get_drawing_height(){
	transform_t pos = get_player_entity()->position;
	int x = pos.x + 1;
	int y = pos.y + 1;
	int z = pos.z + 1; 
	
	while (get_block(x, y, z).id == 0 || get_block(x, y, z).block.transparent){
		x++;
		y++;
		z++;
		
		if (x > pos.x + 2 || y > pos.y + 2 || z > pos.z + 2){
			return CHUNK_HEIGHT;
		}
		if (x == CHUNK_WIDTH || y == CHUNK_LENGTH || z == CHUNK_HEIGHT){
			return z;
		}
	}
	return pos.z + 1;
}

void draw_view(render_obj_t * object){
	get_camera_view(&view_x, &view_y);
	object->surface = SDL_LoadBMP(ATLAS_PATH);
	object->texture = SDL_CreateTextureFromSurface(object->renderer, object->surface);
	object->clip.w = TILE_WIDTH;
	object->clip.h = TILE_HEIGHT;
	transform_t pos = get_player_entity()->position;
	int drawing_height = CHUNK_HEIGHT;
	int draw_indoors = 0;
	
	for (int index = 0 ; index < CHUNK_WIDTH * CHUNK_LENGTH; index++){
		int x = index % CHUNK_WIDTH;
		int y = index / CHUNK_LENGTH;

		if (is_block_shaded(pos.x, pos.y, pos.z-1)){
			drawing_height = pos.z+1;
			draw_indoors = 1;
		} else {
			drawing_height = get_drawing_height();
			draw_indoors = 0;
		}
		
		for (int z = 0 ; z < drawing_height ; z++){

			int shade_z = z;
			if (shade_z == drawing_height - 1){
				shade_z--;
			}			
			if (is_block_shaded(x, y, shade_z)){
				draw_indoors = 1;
			} else {
				draw_indoors = 0;
			}
			
			int block = get_block(x, y, z).block.texture;

			if (x == CHUNK_WIDTH / 2 && y == CHUNK_LENGTH / 2 && z == pos.z){
				SDL_FreeSurface(object->surface);
				SDL_DestroyTexture(object->texture);
				draw_player(object);
				object->surface = SDL_LoadBMP(ATLAS_PATH);
				object->texture = SDL_CreateTextureFromSurface(object->renderer, object->surface);
				object->clip.w = TILE_WIDTH;
				object->clip.h = TILE_HEIGHT;
			}
				
			if (get_block(x, y, z).id > 0){
				object->clip.x = (block % (ATLAS_WIDTH / TILE_WIDTH)) * TILE_WIDTH;
				object->clip.y = (block / (ATLAS_HEIGHT / TILE_HEIGHT)) * TILE_HEIGHT;						
			} else {
				object->clip.x = 0;
				object->clip.y = 0;
			}
			
			float z_pos = z * height_offset;
			float x_pos = x - z_pos;
			float y_pos = y - z_pos;

			int brightness = (32 * is_daytime())+((z) * 20);
			if (brightness > 255){
				brightness = 255;
			}

			if (draw_indoors){
				SDL_SetTextureColorMod(object->texture, 16+(z * 15), 16+(z * 15), 16+(z * 15));
			} else {
				SDL_SetTextureColorMod(object->texture, brightness, brightness, brightness);
			}
			
			object->target.x = (x_pos * (DEFAULT_SCREEN_WIDTH/view_x)) / block_offset;
			object->target.y = (y_pos * (DEFAULT_SCREEN_HEIGHT/view_y)) / block_offset;
			object->target.w = DEFAULT_SCREEN_WIDTH/view_x;
			object->target.h = DEFAULT_SCREEN_HEIGHT/view_y;
			SDL_RenderCopy(object->renderer, object->texture, &object->clip, &object->target);
		}
	}
	SDL_FreeSurface(object->surface);
	SDL_DestroyTexture(object->texture);
}
  
/* Draw player */
void draw_player(render_obj_t * object){
	transform_t pos = get_player_entity()->position;
	
	get_camera_view(&view_x, &view_y);    
	object->surface = SDL_LoadBMP(LEVEE_BODY_PATH);
	object->texture = SDL_CreateTextureFromSurface(object->renderer, object->surface);

	float z_pos = pos.z * height_offset;
	float x_pos = CHUNK_WIDTH/2 - z_pos;
	float y_pos = CHUNK_LENGTH/2 - z_pos;
    
	object->target.x = (x_pos * (DEFAULT_SCREEN_WIDTH/view_x)) / block_offset;
	object->target.y = (y_pos * (DEFAULT_SCREEN_HEIGHT/view_y)) / block_offset;
	object->target.x += 8;
	object->target.y -= 8;
	object->target.w = DEFAULT_SCREEN_WIDTH/view_x + 12;
	object->target.h = DEFAULT_SCREEN_HEIGHT/view_y + 12;

	object->clip.w = LEVEE_WIDTH;
	object->clip.h = LEVEE_HEIGHT;
	object->clip.x = get_player_entity()->sprite.frame_offset * LEVEE_WIDTH;
	object->clip.y = 0;

	int brightness = (32 * is_daytime())+((pos.z) * 25);
	if (brightness > 255){
		brightness = 255;
	}

	if (is_block_shaded(pos.x, pos.y, pos.z-1)){
		SDL_SetTextureColorMod(object->texture, 16+(pos.z * 15), 16+(pos.z * 15), 16+(pos.z * 15));
	} else {
		SDL_SetTextureColorMod(object->texture, brightness, brightness, brightness);
	}
	
	SDL_RenderCopy(object->renderer, object->texture, &object->clip, &object->target);
	SDL_FreeSurface(object->surface);
	SDL_DestroyTexture(object->texture);

}
