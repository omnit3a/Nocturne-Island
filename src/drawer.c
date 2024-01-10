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

const double height_offset = 1 / 0.5;
const double block_offset = 1.6175;

int texture_lookup[16 * 16][2];

void init_texture_lookup(){
	for (int index = 0 ; index < 256 ; index++){
		int x = index % 16;
		int y = index / 16;
		texture_lookup[index][0] = x * TILE_WIDTH;
		texture_lookup[index][1] = y * TILE_HEIGHT;
	}
}

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

double z_pos;

double fast_div(double x, double y){
	union {
		double dbl;
		unsigned long long ull;
	} data_union;
	data_union.dbl = y;
	data_union.ull = ( 0xbfcdd6a18f6a6f52ULL - data_union.ull ) >> (unsigned char) 1;
	data_union.dbl *= data_union.dbl;
	return data_union.dbl * x; 
}

SDL_Surface * atlas_surface;
SDL_Texture * atlas_texture;
SDL_Surface * player_surface;
SDL_Texture * player_texture;

void load_textures(render_obj_t * object){
	atlas_surface = SDL_LoadBMP(ATLAS_PATH);
	atlas_texture = SDL_CreateTextureFromSurface(object->renderer, atlas_surface);
	player_surface = SDL_LoadBMP(LEVEE_BODY_PATH);
	player_texture = SDL_CreateTextureFromSurface(object->renderer, player_surface);
}

void unload_textures(){
	SDL_FreeSurface(atlas_surface);
	SDL_DestroyTexture(atlas_texture);
	SDL_FreeSurface(player_surface);
	SDL_DestroyTexture(player_texture);	
}

void draw_view(render_obj_t * object){
	get_camera_view(&view_x, &view_y);
	object->clip.w = TILE_WIDTH;
	object->clip.h = TILE_HEIGHT;
	transform_t pos = get_player_entity()->position;
	int drawing_height = CHUNK_HEIGHT;
	const int block_width = DEFAULT_SCREEN_WIDTH / view_x;
	const int block_height = DEFAULT_SCREEN_HEIGHT / view_y;
	
	for (int index = 0 ; index < CHUNK_WIDTH * CHUNK_LENGTH; index++){
		int x = index % CHUNK_WIDTH;
		int y = index / CHUNK_LENGTH;
		int draw_indoors = 0;
		
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
			z_pos = (z - pos.z);
			z_pos = fast_div(z_pos, height_offset);
			
			if (x == CHUNK_WIDTH / 2 && y == CHUNK_LENGTH / 2 && z == pos.z){
				draw_player(object);
				object->clip.w = TILE_WIDTH;
				object->clip.h = TILE_HEIGHT;
			}
				
			if (get_block(x, y, z).id > 0){
				int tree_base = (get_block(x, y, z).block.id == TREE_TRUNK &&
						 (get_block(x,y,z-1).block.id != TREE_TRUNK && get_block(x,y,z-1).block.id != EMPTY));
				object->clip.x = texture_lookup[block+tree_base][0];
				object->clip.y = texture_lookup[block][1];
			} else {
				object->clip.x = 0;
				object->clip.y = 0;
			}

			if (draw_indoors){
				SDL_SetTextureColorMod(atlas_texture, 16+(z * 15), 16+(z * 15), 16+(z * 15));
			} else {
				int brightness = (32 * is_daytime())+((z) * 20);
				if (brightness > 255){
					brightness = 255;
				}
				SDL_SetTextureColorMod(atlas_texture, brightness, brightness, brightness);
			}

			double x_pos = x - z_pos;
		        double y_pos = y - z_pos;
		        x_pos *= block_width;
		        y_pos *= block_height;
			x_pos = fast_div(x_pos, block_offset);
			y_pos = fast_div(y_pos, block_offset);
			object->target.x = x_pos;
			object->target.y = y_pos;
			object->target.w = DEFAULT_SCREEN_WIDTH/view_x;
			object->target.h = DEFAULT_SCREEN_HEIGHT/view_y;
			SDL_RenderCopy(object->renderer, atlas_texture, &object->clip, &object->target);
		}
	}
}
  
/* Draw player */
void draw_player(render_obj_t * object){
	transform_t pos = get_player_entity()->position;	
	get_camera_view(&view_x, &view_y);    
	const int block_width = DEFAULT_SCREEN_WIDTH / view_x;
	const int block_height = DEFAULT_SCREEN_HEIGHT / view_y;

	double x_pos = CHUNK_WIDTH/2 - z_pos;
        double y_pos = CHUNK_LENGTH/2 - z_pos;
	x_pos *= block_width;
	y_pos *= block_height;
	x_pos = fast_div(x_pos, block_offset);
	y_pos = fast_div(y_pos, block_offset);
	
	object->target.x = x_pos;
	object->target.y = y_pos;
	//object->target.x += 8;
	//object->target.y -= 8;
	object->target.w = DEFAULT_SCREEN_WIDTH/view_x + 16;
	object->target.h = DEFAULT_SCREEN_HEIGHT/view_y + 16;

	object->clip.w = LEVEE_WIDTH;
	object->clip.h = LEVEE_HEIGHT;
	object->clip.x = get_player_entity()->sprite.frame_offset * LEVEE_WIDTH;
	object->clip.y = 0;

	if (is_block_shaded(pos.x, pos.y, pos.z-1)){
		SDL_SetTextureColorMod(player_texture, 16+(pos.z * 15), 16+(pos.z * 15), 16+(pos.z * 15));
	} else {
		int brightness = (32 * is_daytime())+((pos.z) * 25);
		if (brightness > 255){
			brightness = 255;
		}
		SDL_SetTextureColorMod(player_texture, brightness, brightness, brightness);
	}
	
	SDL_RenderCopy(object->renderer, player_texture, &object->clip, &object->target);
}
