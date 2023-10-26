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
#include <inventory.h>
#include <crafting.h>
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
  ui_x_scale = (SCREEN_WIDTH / COLS);
  ui_y_scale = (SCREEN_HEIGHT / ROWS); 
  switch(ui_mode){
    case IDLE:
      draw_string(CURRENT_VERSION_MSG, object);
      break;
    case INVENTORY:
      displayInventory(renderer);
      displayMessageBar(renderer);
      break;
    case CRAFTING:
      displayCraftableItems(renderer);
      break;
  }
  
}

void draw_direction(render_obj_t * object){
  int xOff = 0;
  int yOff = 0;
  /* Determine offset for the player direction arrow
     depending on the direction the player is facing
  */
  int x_width = (SCREEN_WIDTH/CAMERA_VIEW);
  int y_height = (SCREEN_HEIGHT/CAMERA_VIEW);
  object->surface = SDL_LoadBMP(ARROW_UI_PATH);
  switch (playerRotation){
    case NORTH:
      sprite_surface = SDL_LoadBMP(LEFT_UP_ARROW_UI);
      xOff = 0;
      yOff = 0;
      break;
    case EAST:
      sprite_surface = SDL_LoadBMP(RIGHT_UP_ARROW_UI);
      xOff = (cameraZoom*2)-cameraZoom;
      yOff = 0;
      break;
    case SOUTH:
      sprite_surface = SDL_LoadBMP(RIGHT_DOWN_ARROW_UI);
      xOff = (cameraZoom*2)-cameraZoom;
      yOff = (cameraZoom*2)-(cameraZoom/5);
      break;
    case WEST:
      sprite_surface = SDL_LoadBMP(LEFT_DOWN_ARROW_UI);
      xOff = 0;
      yOff = (cameraZoom*2)-(cameraZoom/5);
      break;
  }
  switch(playerZRotation){
    case UP:
      yOff = yOff-((cameraZoom*2)-(cameraZoom/5))/2;
      break;
    case DOWN:
      yOff = yOff+((cameraZoom*2)-(cameraZoom/5))/2;
      break;
    default:
      break;
  }
  object->texture = SDL_CreateTextureFromSurface(object->renderer, object->surface);
  object->target.x = xOff;
  object->target.y = yOff;
  object->target.w = (SCREEN_WIDTH/CAMERA_VIEW);
  object->target.h = (SCREEN_HEIGHT/CAMERA_VIEW);
  SDL_RenderCopyEx(object->renderer, object->texture, NULL, &object->target, render_angle, NULL, 0);
  SDL_DestroyTexture(object->texture);
  SDL_FreeSurface(object->surface);
}

void handleBlockSelect(SDL_Event event){
  /* Handle selecting items from the inventory */
  if (currentUIMode == IDLE){
    char code = event.key.keysym.sym-48;
    if (code >= 0 && code <= 9){
      if (code == 0){
	currentBlock = inventory[9].block;
        selected_block = 9;
      } else {
	currentBlock = inventory[code-1].block;
	selected_block = code - 1;
      }
    }
  }
}

/* Switch between UI Modes */
void handle_ui(SDL_Event event){
}

void handleCraftingSelect(SDL_Event event){
  char code = event.key.keysym.sym-97;
  if (currentUIMode == CRAFTING){
    if (code >= 0 && code <= 26){
      current_recipe = craftable_recipes[(int)code];
    } else {
      strcpy(messageBar, UNCRAFTABLE_SELECTION_MSG);
    }
    currentUIMode = IDLE;
  }
}

void displayCraftableItems(SDL_Renderer * renderer){
  char line_text[64];
  drawString(0,0,CRAFTABLE_ITEMS_MSG, renderer);
  for (int i = 0 ; i < CRAFTABLE_RECIPES_COUNT && craftable_recipes[i].output_block != 0 ; i++){
    sprintf(line_text, " %c %s", i+97, blockNames[craftable_recipes[i].output_block]);
    drawString(0, i+1, line_text, renderer);
  }
}
