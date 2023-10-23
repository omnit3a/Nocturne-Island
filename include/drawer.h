#ifndef DRAWER_H_
#define DRAWER_H_

#include <SDL2/SDL.h>
#include <map.h>
#include <stdbool.h>
#include <map_defs.h>
#include <camera.h>

#define ATLAS_PATH "assets/3d-atlas.bmp"

#define LEVEE_PATH "assets/levee.bmp"
#define LEVEE_OUTLINE_PATH "assets/levee-outline.bmp"

#define TILE_WIDTH 16
#define TILE_HEIGHT 16
#define ATLAS_WIDTH TILE_WIDTH * 16
#define ATLAS_HEIGHT TILE_HEIGHT * 16
#define DEFAULT_SCREEN_WIDTH 640
#define DEFAULT_SCREEN_HEIGHT 480

#define BASE_DEPTH_BRIGHTNESS 75

extern char * PLAYER_SPRITE;
extern bool renderPlayerLast;

extern int SCREEN_WIDTH;
extern int SCREEN_HEIGHT;
extern int xPosBackup, yPosBackup, belowPosBackup;

/*
  IDK why i made these as global variables.
  im honestly too scared to change them, cause idk where
  they are required.
 */
extern int iBackup, jBackup;

int *** getBlocksInView(char world[MAP_WIDTH][MAP_LENGTH][MAP_HEIGHT]);
void drawSlopes(int blocks_in_view[CAMERA_VIEW][CAMERA_VIEW][256], SDL_Renderer * renderer);
void drawView(int blocks_in_view[CAMERA_VIEW][CAMERA_VIEW][256], SDL_Renderer * renderer);
void drawPlayer(char * playerPath, SDL_Renderer * renderer);
#endif
