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
#define FONT_ATLAS_SIZE 16
#define COLS 80
#define ROWS 24

typedef enum ui_mode_e {
  IDLE = 0,
  INVENTORY = 1,
  CRAFTING = 2
} ui_mode_t;

void zero_ui();
void newline_ui();
void draw_letter(char letter, render_obj_t * object);
void draw_string(char * string, render_obj_t * object);
void draw_ui(render_obj_t * object); 
void draw_direction(render_obj_t * object);
void handle_ui(SDL_Event event);

#endif
