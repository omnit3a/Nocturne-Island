#ifndef DRAWER_H_
#define DRAWER_H_

#include <SDL2/SDL.h>
#include <map.h>

#define PLAYER_SPRITE_RED "../assets/player-red.bmp"
#define PLAYER_SPRITE_BLUE "../assets/player-blue.bmp"
#define ATLAS_PATH "../assets/atlas.bmp"

#define ATLAS_WIDTH 384
#define ATLAS_HEIGHT 544
#define TILE_WIDTH 24
#define TILE_HEIGHT 34

typedef enum blocks_e {
  STONE = 1,
  GRASS = 2,
  WOOD = 3,
  WATER = 4,
  MAGMA = 5,
  SAND = 6,
} blocks_t;

void drawBlock(int xPos, int yPos, int zPos, int height, blocks_t block, SDL_Renderer * renderer);
void drawWorld(char world[MAP_WIDTH][MAP_LENGTH][MAP_HEIGHT], int height, SDL_Renderer * renderer);
void drawPlayer(char * playerPath, int xPos, int yPos, SDL_Renderer * renderer);
#endif

