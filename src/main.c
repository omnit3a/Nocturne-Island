#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <stdio.h>
#include <stdlib.h>
#include <drawer.h>
#include <map.h>
#include <camera.h>
#include <time.h>
#include <player.h>
#include <physics.h>
#include <map_defs.h>
#include <ticks.h>
#include <ui.h>
#include <inventory.h>
#include <menu_defs.h>
#include <crafting.h>
#include <controls.h>
#include <menu.h>

int main(int argc, char ** argv){	
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0){
		fprintf(stderr, "Failed to initialized SDL2\n");
		return -1;
	}

	SDL_Window * window;
	SDL_Renderer * renderer;
	SDL_CreateWindowAndRenderer(DEFAULT_SCREEN_WIDTH,
				    DEFAULT_SCREEN_HEIGHT,
				    SDL_RENDERER_PRESENTVSYNC | SDL_WINDOW_RESIZABLE,
				    &window,
				    &renderer);
  
	if (!window){
		fprintf(stderr, "Failed to create window\n");
		return -1;
	}

	SDL_Surface * window_surface = SDL_GetWindowSurface(window);
	if (!window_surface){
		fprintf(stderr, "Failed to create window surface\n");
		return -1;
	}

	TTF_Init();
  
	SDL_RenderSetLogicalSize(renderer, DEFAULT_SCREEN_WIDTH, DEFAULT_SCREEN_HEIGHT);
	SDL_UpdateWindowSurface(window);

	load_block_properties(BLOCK_DATA_PATH);
	load_crafting_recipes(CRAFTING_DATA_PATH);
	load_control_scheme(CONTROL_SCHEME_PATH);
	
	int player_dead = 1;
	allocate_changed_blocks();
	while (player_dead){
		player_dead = 0;
		init_map(time(0));
		printf("Seed: %d\n", get_map_seed());
		generate_hills(SPAWN_X, SPAWN_Y);  // generate a hilly world

		init_texture_lookup();
		setup_camera(renderer, window);
		init_player_entity();
		init_inventory();
    
		ui_menu_t start_menu;
		get_menu_info(&start_menu, GAME_UI_ID);
		open_menu(&start_menu);
    
		/* MAIN GAME LOOP */
		int running_game = 1;
		while (running_game){
			SDL_Event event;
      
			tick_update();

			if (SDL_PollEvent(&event) <= 0){
				continue;
			}
			
			if (event.type == SDL_QUIT){
				running_game = 0;
				break;
			}

			if (event.type != SDL_KEYDOWN){
				continue;
			}
			
			handle_menu(&running_game, &player_dead, event);

			if (player_dead){
				running_game = 0;
				break;
			}
	    
			handle_player_movement(event);
			handle_player_rotation(event);

			int keycode = translate_keypress(event, get_active_menu());
	    
			switch(keycode){
				/* Mine a block */
			case SDLK_m:
				player_mine_block();
				break;
	      
				/* Place a block */
			case SDLK_n:
				player_place_block();
				break;
	    
				/* Eat food */
			case SDLK_f:
				player_eat_food();
				break;

			case SDLK_q:
				player_drink_water();
				break;
			}
		}
	}

	free_changed_blocks();
	unload_textures();
	TTF_Quit();
	SDL_Quit();
  
	return 0;
}
