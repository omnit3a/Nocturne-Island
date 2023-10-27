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
#define FONT_ATLAS_SIZE 16
#define COLS 80
#define ROWS 24

typedef enum ui_mode_e {
  IDLE = 0,
  INVENTORY = 1
} ui_mode_t;

void zero_ui();
void newline_ui();
void draw_letter(char letter, render_obj_t * object);
void draw_string(char * string, render_obj_t * object);
void draw_ui(render_obj_t * object); 
void draw_direction(render_obj_t * object);
void handle_ui(SDL_Event event);

#endif
