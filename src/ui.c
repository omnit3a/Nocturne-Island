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

ui_mode_t currentUIMode = IDLE;
int currentBlock = EMPTY;
int selected_block = 0;
char blockNames[256][64] = {
  "Empty",
  "Stone",
  "Grass",
  "Log",
  "Tree Leaves",
};
char messageBar[256] = "";

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

/* Draw and individual character from the font map */
void drawLetter(int xPos, int yPos, unsigned char offset, SDL_Renderer * renderer){
  font_rect.x = xPos;
  font_rect.y = yPos;
  if (offset > 0){
    font_clip.x = (offset * FONT_WIDTH) % (FONT_WIDTH*FONT_ROW_LENGTH);
    font_clip.y = ((offset / FONT_ROW_LENGTH) * FONT_HEIGHT);
  } else {
    font_clip.x = 0;
    font_clip.y = 0;
  }
  SDL_RenderCopy(renderer, font_texture, &font_clip, &font_rect);
}

void drawString(int xPos, int yPos, char * string, SDL_Renderer * renderer){
  font_surface = SDL_LoadBMP(FONT_PATH);
  font_texture = SDL_CreateTextureFromSurface(renderer, font_surface);
  /* Scale the font */
  font_rect.w = FONT_WIDTH * FONT_SCALE;
  font_rect.h = FONT_HEIGHT * FONT_SCALE;
  font_clip.w = FONT_WIDTH;
  font_clip.h = FONT_HEIGHT;
  for (int i = 0 ; string[i] != 0 ; i++){
    drawLetter((xPos * FONT_WIDTH * FONT_SCALE) + (i*(FONT_WIDTH*FONT_SCALE)), yPos * (FONT_HEIGHT * FONT_SCALE), (unsigned char) string[i], renderer);
  }
  SDL_FreeSurface(font_surface);
  SDL_DestroyTexture(font_texture);
}

void drawUI(SDL_Renderer * renderer){
  drawCurrentDirection(renderer);
  switch(currentUIMode){
    case IDLE:
      drawString(0,0,CURRENT_VERSION_MSG,renderer);
      break;
    case INVENTORY:
      displayMessageBar(renderer);
      break;
    default:
      break;
  }
  
}

void drawCurrentDirection(SDL_Renderer * renderer){
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
void handleUISwitch(SDL_Event event){
}

void displayHealth(SDL_Renderer * renderer){
  char health[64];
  sprintf(health, PLAYER_HEALTH_MSG, playerHealth);
  drawString(0,2, health, renderer);
}

void displayMessageBar(SDL_Renderer * renderer){
  int yPos = ((SCREEN_HEIGHT)-FONT_HEIGHT)/FONT_HEIGHT;
  drawString(0,yPos, messageBar, renderer);
}
