#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <drawer.h>
#include <map.h>
#include <camera.h>
#include <time.h>
#include <player.h>
#include <physics.h>
#include <map_defs.h>
#include <ticks.h>

int main(int argc, char ** argv){
  if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0){
    fprintf(stderr, "Failed to initialized SDL2\n");
    return -1;
  }

  SDL_Window * window;
  SDL_Renderer * renderer;
  SDL_CreateWindowAndRenderer(SCREEN_WIDTH,
			      SCREEN_HEIGHT,
			      SDL_RENDERER_PRESENTVSYNC,
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

  SDL_UpdateWindowSurface(window);

  load_block_properties(BLOCK_DATA_PATH);
  generate_hills(time(0));  // generate a hilly world
  setup_camera(renderer, window);
  spawn_player();
  
  /* MAIN GAME LOOP */
  bool running_game = true;
  while (running_game){
    SDL_Event e;

    if (get_current_tick() % (TICKS_PER_SECOND / 8) == 0){
      handle_physics();
    } else {
      reset_physics();
    }

    if (get_current_tick() % 2 == 0){
      update_camera();
    }
    
    while (SDL_PollEvent(&e) > 0){
      switch (e.type){
        case SDL_KEYDOWN:
	  handle_player_movement(e);
	  handle_player_rotation(e);
	  switch(e.key.keysym.sym){
	    /* Mine a block */
	    case SDLK_m:
	      player_mine_block();
	      break;
	      
	    /* Place a block */
	    case SDLK_n:
	      player_place_block(0);
	      break;
	  }
	  break;
        case SDL_QUIT:
	  running_game = false;
	  break;
      }
    }
  }

  SDL_Quit();
  
  return 0;
}
