
#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <drawer.h>
#include <map.h>
#include <camera.h>
#include <time.h>
#include <player.h>
#include <ui.h>
#include <pthread.h>
#include <unistd.h>
#include <physics.h>
#include <teams.h>
#include <inventory.h>

int main(int argc, char ** argv){
  if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0){
    fprintf(stderr, "Failed to initialized SDL2\n");
    return -1;
  }

  SDL_Window * window;
  SDL_Renderer * renderer;
  SDL_CreateWindowAndRenderer(SCREEN_WIDTH,
			      SCREEN_HEIGHT,
			      0,
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
  generateHills(world, time(0));  // generate a hilly world
  cullHiddenBlocks(world_copy, world); // remove blocks that are surrounded
  setPhysicsMap(world_copy); // save the world map to the physics collision map
  initInventory(); // fill inventory with empty slots
  addItemToInventory(WORK_BENCH, 1); // give player 1 workbench
  
  setTeam(DEFAULT_TEAM, 1); //set the players team to the default team and spawn the player

  /* START PHYSICS FOR PLAYER */
  if (pthread_mutex_init(&physics_lock,NULL) != 0){
    fprintf(stderr, "Failed to create mutex for physics\n");
    return -1;
  }
  pthread_t physics_id;
  /* Player physics run on a seperate thread to allow for real-time gameplay 
     rather than turn-based 
  */
  pthread_create(&physics_id, NULL, handlePlayerGravity, NULL);

  /* MAIN GAME LOOP */
  bool running_game = true;
  while (running_game){
    SDL_Event e;
    while (SDL_PollEvent(&e) > 0){
      /* Redraw screen and UI everytime an input is received.
	 Probably should make it refresh everytime the world/player/physics 
	 updates instead of when the player does something
       */
      updateCamera(cameraX, cameraY, cameraZoom, renderer, world_copy, window);
      drawUI(renderer);
      SDL_RenderPresent(renderer);
      switch (e.type){
        case SDL_KEYDOWN:
	  if (currentUIMode != CRAFTING){
	    handlePlayerMovement(world_copy, e);
	    handleBlockSelect(e);
	    handlePlayerRotation(e);
	    handleUISwitch(e); // switch between UI modes
	    switch(e.key.keysym.sym){
	      /* Mine a block */
	      case SDLK_m:
		if (currentUIMode == CRAFTING){
		  break;
		}
		playerMineBlock(world);
		/* Regenerate the world_copy map, physics map, and solidity map */
		cullHiddenBlocks(world_copy, world);
		setPhysicsMap(world_copy);
		break;
		/* Place a block */
	      case SDLK_n:
		if (currentUIMode == CRAFTING){
		  break;
		}
		playerPlaceBlock(world, currentBlock);
		/* Regenerate the world_copy map, physics map, and solidity map */
		cullHiddenBlocks(world_copy, world);
		setPhysicsMap(world_copy);
		break;
	    }
	    break;
	  } else {
	    handleCraftingSelect(e);
	  }
	  break;
        case SDL_QUIT:
	  running_game = false;
	  break;
      }
    }
  }
  
  pthread_mutex_destroy(&physics_lock);

  return 0;
}
