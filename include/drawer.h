#ifndef DRAWER_H_
#define DRAWER_H_

#include <SDL2/SDL.h>
#include <map.h>
#include <stdbool.h>

#define ATLAS_PATH "../assets/atlas.bmp"

#define LEVEE_PATH "../assets/levee.bmp"
#define LEVEE_OUTLINE_PATH "../assets/levee-outline.bmp"

#define TILE_WIDTH 24
#define TILE_HEIGHT 34
#define ATLAS_WIDTH TILE_WIDTH * 16
#define ATLAS_HEIGHT TILE_HEIGHT * 16
#define DEFAULT_SCREEN_WIDTH 640
#define DEFAULT_SCREEN_HEIGHT 480

#define BASE_DEPTH_BRIGHTNESS 50

extern char * PLAYER_SPRITE;
extern bool renderPlayerLast;

extern int SCREEN_WIDTH;
extern int SCREEN_HEIGHT;
extern int xPosBackup, yPosBackup, belowPosBackup;

typedef enum blocks_e {
  STONE = 1,
  GRASS = 2,
  WOOD = 3,
  WATER = 4,
  MAGMA = 5,
  SAND = 6,
  TREE_BOTTOM = 7,
  TREE_TRUNK = 8,
  TREE_LEAVES = 9,
  BLOCK_OUTLINE = 10,
  NOKIUM = 11,
  NORTH_STAIRS = 12,
  EAST_STAIRS = 13,
  SOUTH_STAIRS = 14,
  WEST_STAIRS = 15,
  STAIRS = 12,
} blocks_t;

bool blockingPlayerCheck(char map[MAP_WIDTH][MAP_LENGTH][MAP_HEIGHT]);
void drawBlock(int xPos, int yPos, int zPos, int height, blocks_t block, SDL_Renderer * renderer);
void drawWorld(char world[MAP_WIDTH][MAP_LENGTH][MAP_HEIGHT], int height, SDL_Renderer * renderer);
void drawPlayer(char * playerPath, int xPos, int yPos, SDL_Renderer * renderer);
#endif

