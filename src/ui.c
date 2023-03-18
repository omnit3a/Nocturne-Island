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

ui_mode_t currentUIMode = IDLE;
blocks_t currentBlock = 0;
int uiScale = 32;
char * blockNames[64] = {
  "Empty",
  "Stone",
  "Grass",
  "Wood",
  "Water",
  "Magma",
  "Sand",
  "Tree Bottom",
  "Tree Trunk",
  "Tree Leaves",
  "Nokium"
};

SDL_Surface * font_surface;
SDL_Texture * font_texture;
SDL_Rect font_rect = {
  0,
  0,
  FONT_WIDTH,
  FONT_HEIGHT
};
SDL_Rect font_clip = {
  0,
  0,
  FONT_WIDTH,
  FONT_HEIGHT
};

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
  switch(currentUIMode){
    case IDLE:
      drawString(0,0,CURRENT_VERSION_MSG,renderer);
      drawCurrentBlock(12, 12, renderer);
      break;
    case INVENTORY:
      displayInventory(renderer);
      break;
  }
  
}

void drawCurrentBlock(int xPos, int yPos, SDL_Renderer * renderer){
  drawString(0,1,CURRENT_BLOCK_MSG,renderer);
  drawString(strlen(CURRENT_BLOCK_MSG),1, blockNames[currentBlock], renderer);
}

void drawCurrentDirection(int xPos, int yPos, SDL_Renderer * renderer){
  SDL_Surface * sprite_surface;
  int xOff = 0;
  int yOff = 0;
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
  SDL_Texture * sprite_texture = SDL_CreateTextureFromSurface(renderer, sprite_surface);
  SDL_Rect sprite_area;
  sprite_area.x = xPos+xOff;
  sprite_area.y = yPos+yOff;
  sprite_area.w = cameraZoom;
  sprite_area.h = cameraZoom;
  SDL_RenderCopy(renderer, sprite_texture, NULL, &sprite_area);
  SDL_DestroyTexture(sprite_texture);
  SDL_FreeSurface(sprite_surface);
}

void handleBlockSelect(SDL_Event event){
  switch(event.key.keysym.sym){
    case SDLK_1:
      currentBlock = inventory[0].block;
      break;
    case SDLK_2:
      currentBlock = inventory[1].block;
      break;
    case SDLK_3:
      currentBlock = inventory[2].block;
      break;
    case SDLK_4:
      currentBlock = inventory[3].block;
      break;
    case SDLK_5:
      currentBlock = inventory[4].block;
      break;
    case SDLK_6:
      currentBlock = inventory[5].block;
      break;
    case SDLK_7:
      currentBlock = inventory[6].block;
      break;
    case SDLK_8:
      currentBlock = inventory[7].block;
      break;
    case SDLK_9:
      currentBlock = inventory[8].block;
      break;
    case SDLK_0:
      currentBlock = inventory[9].block;
      break;
  }
}

void handleUISwitch(SDL_Event event){
  switch(event.key.keysym.sym){
    case SDLK_e:
      if (currentUIMode == IDLE){
	currentUIMode = INVENTORY;
      } else {
	currentUIMode = IDLE;
      }
      break;
  }
}

void displayInventory(SDL_Renderer * renderer){
  drawString(0,0,"Inventory",renderer);
  for (int i = 0 ; i < INVENTORY_SIZE ; i++){
    drawString(0,i+1," - ",renderer);
    drawString(3,i+1,blockNames[inventory[i].block],renderer);
  }
}
