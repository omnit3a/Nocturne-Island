#ifndef UI_H_
#define UI_H_

#include <SDL2/SDL.h>
#include <drawer.h>

/* Directional arrows textures */
#define LEFT_UP_ARROW_UI "assets/ui/up-left-arrow.bmp"
#define RIGHT_UP_ARROW_UI "assets/ui/up-right-arrow.bmp"
#define LEFT_DOWN_ARROW_UI "assets/ui/down-left-arrow.bmp"
#define RIGHT_DOWN_ARROW_UI "assets/ui/down-right-arrow.bmp"

/* Font related definitions */
#define FONT_PATH "assets/ui/font.bmp"
#define FONT_WIDTH 8
#define FONT_HEIGHT 16
#define FONT_ROW_LENGTH 16
#define FONT_SCALE 1

typedef enum ui_mode_e {
  IDLE = 0,
  INVENTORY = 1,
  CRAFTING = 2
} ui_mode_t;

extern ui_mode_t currentUIMode;
extern int currentBlock;
extern int selected_block;
/* List of display names of blocks */
extern char * blockNames[64];
extern char messageBar[256];

void drawLetter(int xPos, int yPos, unsigned char offset, SDL_Renderer * renderer);
void drawString(int xPos, int yPos, char * string, SDL_Renderer * renderer);
void drawUI(SDL_Renderer * renderer); 
void drawCurrentBlock(int xPos, int yPos, SDL_Renderer * renderer);
void drawCurrentDirection(int xPos, int yPos, SDL_Renderer * renderer);
void handleBlockSelect(SDL_Event event);
void handleUISwitch(SDL_Event event);
void displayInventory(SDL_Renderer * renderer);
void displayHealth(SDL_Renderer * renderer);
void displayMessageBar(SDL_Renderer * renderer);
void handleCraftingSelect(SDL_Event event);
void displayCraftableItems(SDL_Renderer * renderer);

#endif
