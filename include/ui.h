#ifndef UI_H_
#define UI_H_

#include <SDL2/SDL.h>
#include <drawer.h>

/* Directional arrows textures */
#define ARROW_UI_PATH "assets/ui/arrow.bmp"

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
extern char blockNames[256][64];
extern char messageBar[256];

void drawLetter(int xPos, int yPos, unsigned char offset, SDL_Renderer * renderer);
void drawString(int xPos, int yPos, char * string, SDL_Renderer * renderer);
void drawUI(SDL_Renderer * renderer); 
void drawCurrentDirection(SDL_Renderer * renderer);
void handleUISwitch(SDL_Event event);
void displayHealth(SDL_Renderer * renderer);
void displayMessageBar(SDL_Renderer * renderer);
void handleCraftingSelect(SDL_Event event);
void displayCraftableItems(SDL_Renderer * renderer);

#endif
