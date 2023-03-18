
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
#include <lighting.h>
#include <inventory.h>

int main(int argc, char ** argv){
  if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0){
    fprintf(stderr, "Failed to initialized SDL2\n");
    return -1;
  }

  /* TODO
     Setup text rendering
  */

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
  setPhysicsMap(world_copy);
  generateSolidity(world_copy);
  initInventory();
  
  setTeam(DEFAULT_TEAM, 1); //set the players team to the default team

  /* START PHYSICS FOR PLAYER */
  if (pthread_mutex_init(&physics_lock,NULL) != 0){
    fprintf(stderr, "Failed to create mutex for physics\n");
    return -1;
  }
  pthread_t physics_id;
  pthread_create(&physics_id, NULL, handlePlayerGravity, NULL);

  /* MAIN GAME LOOP */
  bool running_game = true;
  while (running_game){
    SDL_Event e;
    while (SDL_PollEvent(&e) > 0){
      updateCamera(cameraX, cameraY, cameraZoom, renderer, world_copy, window);
      drawUI(renderer);
      SDL_RenderPresent(renderer);
      switch (e.type){
        case SDL_KEYDOWN:
	  handlePlayerMovement(world_copy, e);
	  handleBlockSelect(e);
	  handleCameraMovement(e);
	  handleUISwitch(e);
	  switch(e.key.keysym.sym){
	    /* Mine a block */
	    case SDLK_m:
	      playerMineBlock(world);
	      cullHiddenBlocks(world_copy, world);
	      setPhysicsMap(world_copy);
	      generateSolidity(world_copy);
	      break;
	    /* Place a block */
	    case SDLK_p:
	      playerPlaceBlock(world, currentBlock);
	      cullHiddenBlocks(world_copy, world);
	      setPhysicsMap(world_copy);
	      generateSolidity(world_copy);
	      break;
	    /* Swap team */
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
