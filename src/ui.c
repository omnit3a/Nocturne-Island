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

ui_mode_t currentUIMode = IDLE;
blocks_t currentBlock = WORK_BENCH;
char * blockNames[64] = {
  "Empty",
  "Stone",
  "Grass",
  "Wood",
  "Water",
  "Magma",
  "Sand",
  "Log",
  "Oak Tree Leaves",
  "Pine Tree Leaves",
  "Block Highlight",
  "Nokium",
  "Stairs",
  "Stairs",
  "Stairs",
  "Stairs",
  "Workbench",
  "Table",
  "Iron Ore",
  "Coal Ore",
  "Iron Chunks",
  "Coal Chunks",
  "Rope",
  "Box of Nails",
};
char messageBar[256] = "";
int selected_block = 0;

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
  switch(currentUIMode){
    case IDLE:
      drawString(0,0,CURRENT_VERSION_MSG,renderer);
      drawCurrentBlock(12, 12, renderer);
      displayHealth(renderer);
      displayMessageBar(renderer);
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

void drawCurrentBlock(int xPos, int yPos, SDL_Renderer * renderer){
  drawString(0,1,CURRENT_BLOCK_MSG,renderer);
  drawString(strlen(CURRENT_BLOCK_MSG),1, blockNames[inventory[selected_block].block], renderer);
}

void drawCurrentDirection(int xPos, int yPos, SDL_Renderer * renderer){
  SDL_Surface * sprite_surface;
  int xOff = 0;
  int yOff = 0;
  /* Determine offset for the player direction arrow
     depending on the direction the player is facing
  */
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
void handleUISwitch(SDL_Event event){
  if (currentUIMode == CRAFTING){
    return;
  }
  switch(event.key.keysym.sym){
    case SDLK_e:
      if (currentUIMode == IDLE){
	currentUIMode = INVENTORY;
      } else if (currentUIMode == INVENTORY){
	currentUIMode = IDLE;
      }
      break;
    case SDLK_c:
      if (currentUIMode == IDLE){
	listCraftableItems();
	currentUIMode = CRAFTING;
      }
      break;
  }
}

/* Display list of items in inventory */
void displayInventory(SDL_Renderer * renderer){
  drawString(0,0,"Inventory",renderer);
  char amount[16];
  for (int i = 0 ; i < INVENTORY_SIZE ; i++){
    char buffer[32];
    if (i < 9){
      sprintf(buffer, " %d", i + 1);
    } else {
      sprintf(buffer, " %d", 0);
    }
    drawString(0, i + 1, buffer, renderer);
    strcpy(amount, "");
    sprintf(amount, ": %u", inventory[i].count);
    drawString(3,i+1,blockNames[inventory[i].block],renderer);
    drawString(3+strlen(blockNames[inventory[i].block]),i+1, amount, renderer);
  }
}

void displayHealth(SDL_Renderer * renderer){
  char health[64];
  sprintf(health, "Player Health: %d", playerHealth);
  drawString(0,2, health, renderer);
}

void displayMessageBar(SDL_Renderer * renderer){
  int yPos = ((SCREEN_HEIGHT)-FONT_HEIGHT)/FONT_HEIGHT;
  drawString(0,yPos, messageBar, renderer);
}

void handleCraftingSelect(SDL_Event event){
  char code = event.key.keysym.sym-97;
  if (currentUIMode == CRAFTING){
    if (code >= 0 && code <= 26){
      current_recipe = craftable_recipes[(int)code];
    } else {
      strcpy(messageBar, "Cannot select this item to craft");
    }
    currentUIMode = IDLE;
  }
}

void displayCraftableItems(SDL_Renderer * renderer){
  char line_text[64];
  drawString(0,0,"Craftable Items", renderer);
  for (int i = 0 ; i < CRAFTABLE_RECIPES_COUNT && craftable_recipes[i].output_block != 0 ; i++){
    sprintf(line_text, " %c %s", i+97, blockNames[craftable_recipes[i].output_block]);
    drawString(0, i+1, line_text, renderer);
  }
}
