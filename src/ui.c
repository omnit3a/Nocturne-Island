#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <stdlib.h>
#include <drawer.h>
#include <ui.h>
#include <player.h>
#include <camera.h>

blocks_t currentBlock = STONE;
int uiScale = 32;

void drawUI(SDL_Renderer * renderer){
  drawCurrentBlock(12, 12, renderer);
}

void drawCurrentBlock(int xPos, int yPos, SDL_Renderer * renderer){
  SDL_Surface * sprite_surface;
  switch (currentBlock){
    case STONE:
      sprite_surface = SDL_LoadBMP(STONE_BLOCK_UI);
      break;
    case GRASS:
      sprite_surface = SDL_LoadBMP(GRASS_BLOCK_UI);
      break;
    case WOOD:
      sprite_surface = SDL_LoadBMP(WOOD_BLOCK_UI);
      break;
    default:
      sprite_surface = SDL_LoadBMP(STONE_BLOCK_UI);
      break;
  }
  SDL_Texture * sprite_texture = SDL_CreateTextureFromSurface(renderer, sprite_surface);
  SDL_Rect sprite_area;
  sprite_area.x = xPos;
  sprite_area.y = yPos;
  sprite_area.w = 24;
  sprite_area.h = 34;
  SDL_RenderCopy(renderer, sprite_texture, NULL, &sprite_area);
  SDL_DestroyTexture(sprite_texture);
  SDL_FreeSurface(sprite_surface);
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
      currentBlock = GRASS;
      break;
    case SDLK_2:
      currentBlock = STONE;
      break;
    case SDLK_3:
      currentBlock = WOOD;
      break;
  }
}
