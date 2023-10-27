#include <SDL2/SDL.h>
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
#include <inventory.h>

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

  SDL_RenderSetLogicalSize(renderer, DEFAULT_SCREEN_WIDTH, DEFAULT_SCREEN_HEIGHT);
  SDL_UpdateWindowSurface(window);

  load_block_properties(BLOCK_DATA_PATH);
  generate_hills(time(0));  // generate a hilly world
  setup_camera(renderer, window);
  init_player_entity();
  init_inventory();
  
  /* MAIN GAME LOOP */
  int running_game = 1;
  while (running_game){
    SDL_Event event;

    tick_update();
    
    while (SDL_PollEvent(&event) > 0){
      switch (event.type){
        case SDL_KEYDOWN:
	  handle_player_movement(event);
	  handle_player_rotation(event);
	  switch(event.key.keysym.sym){
	    /* Mine a block */
	    case SDLK_m:
	      player_mine_block();
	      break;
	      
	    /* Place a block */
	    case SDLK_n:
	      player_place_block();
	      break;
	  }
	  break;
        case SDL_QUIT:
	  running_game = 0;
	  break;
      }
    }
  }

  SDL_Quit();
  
  return 0;
}
