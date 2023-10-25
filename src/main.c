#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <drawer.h>
#include <map.h>
#include <camera.h>
#include <time.h>
#include <player.h>
#include <pthread.h>
#include <unistd.h>
#include <physics.h>
#include <teams.h>
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
  char world[MAP_WIDTH][MAP_LENGTH][MAP_HEIGHT];
  char world_copy[MAP_WIDTH][MAP_LENGTH][MAP_HEIGHT];

  loadBlockProperties(BLOCK_DATA_PATH, data_map);
  setTeam(DEFAULT_TEAM, 1);

  generateHills(world, time(0));  // generate a hilly world
  cullHiddenBlocks(world_copy, world); // remove blocks that are surrounded

  setPhysicsMap(world_copy); // save the world map to the physics collision map  
  setup_camera(renderer, window);
  
  /* INIT PHYSICS MUTEX */
  if (pthread_mutex_init(&physics_lock,NULL) != 0){
    fprintf(stderr, "Failed to create mutex for physics\n");
    return -1;
  }

  /* MAIN GAME LOOP */
  bool running_game = true;
  while (running_game){
    SDL_Event e;

    if (get_current_tick() % (TICKS_PER_SECOND / 8) == 0){
      handle_physics();
    } else if (get_current_tick() % 2 == 0){
      update_camera();
    } else {
      reset_physics();
    }
    
    while (SDL_PollEvent(&e) > 0){
      switch (e.type){
        case SDL_KEYDOWN:
	  handlePlayerMovement(world_copy, e);
	  handlePlayerRotation(e);
	  switch(e.key.keysym.sym){
	    /* Mine a block */
	    case SDLK_m:
	      playerMineBlock(world);
	      /* Regenerate the world_copy map, physics map, and solidity map */
	      cullHiddenBlocks(world_copy, world);
	      break;
	      
	    /* Place a block */
	    case SDLK_n:
	      //playerPlaceBlock(world, currentBlock);
	      /* Regenerate the world_copy map, physics map, and solidity map */
	      cullHiddenBlocks(world_copy, world);
	      break;
	  }
	  break;
        case SDL_QUIT:
	  running_game = false;
	  break;
      }
    }
  }

  pthread_mutex_destroy(&physics_lock);
  SDL_Quit();
  
  return 0;
}
