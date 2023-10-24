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
#include <map_defs.h>

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

  loadBlockProperties(BLOCK_DATA_PATH, data_map);
  
  setTeam(DEFAULT_TEAM, 1);
  generateHills(world, time(0));  // generate a hilly world
  cullHiddenBlocks(world_copy, world); // remove blocks that are surrounded
  setPhysicsRenderer(renderer, window);
  setPhysicsMap(world_copy); // save the world map to the physics collision map
  setupCamera(renderer, window);
  setupCameraMap(world_copy);
  
  /* INIT PHYSICS MUTEX */
  if (pthread_mutex_init(&physics_lock,NULL) != 0){
    fprintf(stderr, "Failed to create mutex for physics\n");
    return -1;
  }

  /* INIT CAMERA DRAW LOOP MUTEX */
  if (pthread_mutex_init(&camera_lock,NULL)!=0){
    fprintf(stderr, "Failed to create mutex for camera\n");
    return -1;
  }
  
  pthread_t physics_id;
  pthread_t camera_id;
  /* Player physics run on a seperate thread to allow for real-time gameplay 
     rather than turn-based 
  */
  pthread_create(&physics_id, NULL, handlePhysics, NULL);

  /* Setup camera so that renderer can function properly */
  pthread_create(&camera_id, NULL, updateCameraOnTick, NULL);

  /* MAIN GAME LOOP */
  bool running_game = true;
  while (running_game){
    SDL_Event e;
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
  pthread_mutex_destroy(&camera_lock);
  
  return 0;
}
