#ifndef UI_H_
#define UI_H_

#include <SDL2/SDL.h>
#include <drawer.h>

/* Font related definitions */
#define FONT_PATH "assets/ui/font.ttf" 
#define FONT_WIDTH 8
#define FONT_HEIGHT 16
#define FONT_ATLAS_SIZE 16

#define HORIZON_PATH "assets/ui/horizon.bmp"

typedef struct ui_menu_s {
  int (*update)(SDL_Event event);
  void (*draw)(render_obj_t * object);
  int active;
  int id;
  int fallback_id;
} ui_menu_t;

void init_ui();

void zero_ui();
void newline_ui();
void ui_draw_horizon(render_obj_t * object);
void draw_string(char * string, SDL_Color color, render_obj_t * object);

void handle_block_select(SDL_Event event);

void draw_menu(render_obj_t * object);
void handle_menu(int * exit, int * death, SDL_Event event);
void get_menu_info(ui_menu_t * info, int id);
void open_menu(ui_menu_t * menu);
void close_menu(ui_menu_t * menu);
int get_current_menu_id();

#endif
