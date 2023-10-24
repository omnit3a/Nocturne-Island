#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <drawer.h>
#include <ui.h>
#include <player.h>
#include <camera.h>
#include <messages.h>
#include <map_defs.h>

ui_mode_t ui_mode = IDLE;
int x_cursor = 0;
int y_cursor = 0;
int ui_x_scale = 1;
int ui_y_scale = 1;

SDL_Surface * font_surface;
SDL_Texture * font_texture;
SDL_Rect font_rect = {
  0,
  0,
  FONT_WIDTH,
  FONT_HEIGHT
};
/* Used for the selecting which character from the font map to draw */
SDL_Rect font_clip = {
  0,
  0,
  FONT_WIDTH,
  FONT_HEIGHT
};

void zero_ui(){
  x_cursor = 0;
  y_cursor = 0;
}

void newline_ui(){
  x_cursor = 0;
  y_cursor++;
  y_cursor = y_cursor % ROWS;
}

/* Draw and individual character from the font map */
void draw_letter(char letter, SDL_Renderer * renderer){
  font_rect.x = x_cursor * ui_x_scale;
  font_rect.y = y_cursor * ui_y_scale;
  if (letter > 0){
    font_clip.x = (letter * FONT_WIDTH) % (FONT_WIDTH*FONT_ATLAS_SIZE);
    font_clip.y = ((letter / FONT_ATLAS_SIZE) * FONT_HEIGHT);
  } else {
    font_clip.x = 0;
    font_clip.y = 0;
  }

  x_cursor++;
  x_cursor = x_cursor % COLS;
  if (x_cursor == COLS-1){
    newline_ui();
  }
    
  SDL_RenderCopy(renderer, font_texture, &font_clip, &font_rect);
}

void draw_string(char * string, SDL_Renderer * renderer){
  font_surface = SDL_LoadBMP(FONT_PATH);
  font_texture = SDL_CreateTextureFromSurface(renderer, font_surface);
  /* Scale the font */
  font_rect.w = ui_x_scale;
  font_rect.h = ui_y_scale;
  font_clip.w = FONT_WIDTH;
  font_clip.h = FONT_HEIGHT;
  for (int i = 0 ; string[i] != 0 ; i++){
    draw_letter(string[i], renderer);
  }
  SDL_FreeSurface(font_surface);
  SDL_DestroyTexture(font_texture);
}

void draw_ui(SDL_Renderer * renderer){
  zero_ui();
  draw_direction(renderer);
  ui_x_scale = (SCREEN_WIDTH / COLS);
  ui_y_scale = (SCREEN_HEIGHT / ROWS); 
  switch(ui_mode){
    case IDLE:
      draw_string(CURRENT_VERSION_MSG,renderer);
      break;
    default:
      break;
  }
  
}

void draw_direction(SDL_Renderer * renderer){
  SDL_Surface * sprite_surface = NULL;
  int xOff = 0;
  int yOff = 0;
  int render_angle = 0;
  /* Determine offset for the player direction arrow
     depending on the direction the player is facing
  */
  int x_width = (SCREEN_WIDTH/CAMERA_VIEW);
  int y_height = (SCREEN_HEIGHT/CAMERA_VIEW);
  sprite_surface = SDL_LoadBMP(ARROW_UI_PATH);
  switch (playerRotation){
    case NORTH:
      xOff = x_width * 4;
      yOff = y_height * 3 - (y_height/4);
      render_angle = 0;
      break;
    case WEST:
      xOff = x_width * 5 - (x_width/4);
      yOff = y_height * 4;
      render_angle = 90;
      break;
    case SOUTH:
      xOff = x_width * 4;
      yOff = y_height * 5 + (y_height/4);
      render_angle = 180;
      break;
    case EAST:
      xOff = x_width * 3 + (x_width/4);
      yOff = y_height * 4;
      render_angle = 270;
      break;
  }
  SDL_Texture * sprite_texture = SDL_CreateTextureFromSurface(renderer, sprite_surface);
  SDL_Rect sprite_area;
  sprite_area.x = xOff;
  sprite_area.y = yOff;
  sprite_area.w = (SCREEN_WIDTH/CAMERA_VIEW);
  sprite_area.h = (SCREEN_HEIGHT/CAMERA_VIEW);
  SDL_RenderCopyEx(renderer, sprite_texture, NULL, &sprite_area, render_angle, NULL, 0);
  SDL_DestroyTexture(sprite_texture);
  SDL_FreeSurface(sprite_surface);
}

/* Switch between UI Modes */
void handle_ui(SDL_Event event){
}
