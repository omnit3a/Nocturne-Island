#ifndef UI_H_
#define UI_H_

#include <SDL2/SDL.h>
#include <drawer.h>

#define STONE_BLOCK_UI "../assets/ui/blocks/stone.bmp"
#define GRASS_BLOCK_UI "../assets/ui/blocks/grass.bmp"
#define WOOD_BLOCK_UI "../assets/ui/blocks/wood.bmp"

#define LEFT_UP_ARROW_UI "../assets/ui/up-left-arrow.bmp"
#define RIGHT_UP_ARROW_UI "../assets/ui/up-right-arrow.bmp"
#define LEFT_DOWN_ARROW_UI "../assets/ui/down-left-arrow.bmp"
#define RIGHT_DOWN_ARROW_UI "../assets/ui/down-right-arrow.bmp"

#define FONT_PATH "../assets/ui/font.bmp"
#define FONT_WIDTH 8
#define FONT_HEIGHT 8
#define FONT_ROW_LENGTH 16
#define FONT_SCALE 2

typedef enum ui_mode_e {
  IDLE = 0,
  INVENTORY = 1
} ui_mode_t;

extern ui_mode_t currentUIMode;
extern blocks_t currentBlock;
extern int uiScale;
extern char * blockName[64];

void drawLetter(int xPos, int yPos, unsigned char offset, SDL_Renderer * renderer);
void drawString(int xPos, int yPos, char * string, SDL_Renderer * renderer);
void drawUI(SDL_Renderer * renderer); 
void drawCurrentBlock(int xPos, int yPos, SDL_Renderer * renderer);
void drawCurrentDirection(int xPos, int yPos, SDL_Renderer * renderer);
void handleBlockSelect(SDL_Event event);
void handleUISwitch(SDL_Event event);
void displayInventory(SDL_Renderer * renderer);

#endif
