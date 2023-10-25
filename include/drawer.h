#ifndef DRAWER_H_
#define DRAWER_H_

#include <SDL2/SDL.h>
#include <map.h>
#include <stdbool.h>
#include <map_defs.h>

#define ATLAS_PATH "assets/3d-atlas.bmp"
#define LEVEE_PATH "assets/levee.bmp"

#define TILE_WIDTH 16
#define TILE_HEIGHT 16
#define ATLAS_WIDTH TILE_WIDTH * 16
#define ATLAS_HEIGHT TILE_HEIGHT * 16
#define DEFAULT_SCREEN_WIDTH 640
#define DEFAULT_SCREEN_HEIGHT 480

#define BASE_DEPTH_BRIGHTNESS 75

extern int SCREEN_WIDTH;
extern int SCREEN_HEIGHT;

typedef struct render_obj_t {
  SDL_Renderer * renderer;
  SDL_Window * window;
  SDL_Surface * surface;
  SDL_Texture * texture;
  SDL_Rect clip;
  SDL_Rect target;
} render_obj_t;

#include <camera.h>

extern char blocks_in_view[CAMERA_VIEW][CAMERA_VIEW][MAP_HEIGHT];

void get_blocks_in_view(char world[MAP_WIDTH][MAP_LENGTH][MAP_HEIGHT]);
void draw_slopes(render_obj_t * object);
void draw_view(render_obj_t * object);
void draw_player(render_obj_t * object);
#endif
