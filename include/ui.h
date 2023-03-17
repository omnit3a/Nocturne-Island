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

extern blocks_t currentBlock;
extern int uiScale;

void drawUI(SDL_Renderer * renderer); 
void drawCurrentBlock(int xPos, int yPos, SDL_Renderer * renderer);
void drawCurrentDirection(int xPos, int yPos, SDL_Renderer * renderer);
void handleBlockSelect(SDL_Event event);

#endif
