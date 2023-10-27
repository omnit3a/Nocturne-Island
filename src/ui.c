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
void draw_letter(char letter, render_obj_t * object){
  object->target.x = x_cursor * ui_x_scale;
  object->target.y = y_cursor * ui_y_scale;

  if (letter > 0){
    object->clip.x = (letter * FONT_WIDTH) % (FONT_WIDTH*FONT_ATLAS_SIZE);
    object->clip.y = ((letter / FONT_ATLAS_SIZE) * FONT_HEIGHT);
  } else {
    object->clip.x = 0;
    object->clip.y = 0;
  }

  x_cursor++;
  x_cursor = x_cursor % COLS;
  if (x_cursor == COLS-1){
    newline_ui();
  }
    
  SDL_RenderCopy(object->renderer, object->texture, &object->clip, &object->target);
}

void draw_string(char * string, render_obj_t * object){
  object->surface = SDL_LoadBMP(FONT_PATH);
  object->texture = SDL_CreateTextureFromSurface(object->renderer, object->surface);
  /* Scale the font */
  object->target.w = ui_x_scale;
  object->target.h = ui_y_scale;
  object->clip.w = FONT_WIDTH;
  object->clip.h = FONT_HEIGHT;
  for (int i = 0 ; string[i] != 0 ; i++){
    draw_letter(string[i], object);
  }
  SDL_FreeSurface(object->surface);
  SDL_DestroyTexture(object->texture);
}

void draw_ui(render_obj_t * object){
  zero_ui();
  draw_direction(object);
  ui_x_scale = (DEFAULT_SCREEN_WIDTH / COLS);
  ui_y_scale = (DEFAULT_SCREEN_HEIGHT / ROWS); 
  switch(ui_mode){
    case IDLE:
      draw_string(CURRENT_VERSION_MSG, object);
      break;
    default:
      break;
  }
  
}

void draw_direction(render_obj_t * object){
  int view_x;
  int view_y;
  int xOff = 0;
  int yOff = 0;
  int render_angle = 0;
  /* Determine offset for the player direction arrow
     depending on the direction the player is facing
  */
  get_camera_view(&view_x, &view_y);
  int x_width = (DEFAULT_SCREEN_WIDTH/view_x);
  int y_height = (DEFAULT_SCREEN_HEIGHT/view_y);
  object->surface = SDL_LoadBMP(ARROW_UI_PATH);
  switch (playerRotation){
    case NORTH:
      xOff = x_width * (view_x/2);
      yOff = y_height * (view_y/2-1) - (y_height/4);
      render_angle = 0;
      break;
    case WEST:
      xOff = x_width * (view_x/2+1) - (x_width/4);
      yOff = y_height * (view_y/2);
      render_angle = 90;
      break;
    case SOUTH:
      xOff = x_width * (view_x/2);
      yOff = y_height * (view_y/2+1) + (y_height/4);
      render_angle = 180;
      break;
    case EAST:
      xOff = x_width * (view_x/2-1) + (x_width/4);
      yOff = y_height * (view_y/2);
      render_angle = 270;
      break;
  }
  object->texture = SDL_CreateTextureFromSurface(object->renderer, object->surface);
  object->target.x = xOff;
  object->target.y = yOff;
  object->target.w = (DEFAULT_SCREEN_WIDTH/view_x);
  object->target.h = (DEFAULT_SCREEN_HEIGHT/view_y);
  SDL_RenderCopyEx(object->renderer, object->texture, NULL, &object->target, render_angle, NULL, 0);
  SDL_DestroyTexture(object->texture);
  SDL_FreeSurface(object->surface);
}

/* Switch between UI Modes */
void handle_ui(SDL_Event event){
}
