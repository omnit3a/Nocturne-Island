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
void draw_letter(char letter, render_obj_t * object);
void draw_string(char * string, render_obj_t * object);
void draw_direction(render_obj_t * object);

void draw_game_ui(render_obj_t * object);
int handle_game_ui(SDL_Event event);

void draw_inventory_ui(render_obj_t * object);
int handle_inventory_ui(SDL_Event event);
void handle_block_select(SDL_Event event);

void draw_pause_ui(render_obj_t * object);
int handle_pause_ui(SDL_Event event);

void draw_crafting_ui(render_obj_t * object);
int handle_crafting_ui(SDL_Event event);

void draw_menu(render_obj_t * object);
void handle_menu(int * exit, SDL_Event event);
void get_menu_info(ui_menu_t * info, int id);
void open_menu(ui_menu_t * menu);
void close_menu(ui_menu_t * menu);
int get_current_menu_id();

#endif
