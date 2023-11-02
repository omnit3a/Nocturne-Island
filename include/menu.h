#pragma once
#include <SDL2/SDL.h>
#include <menu_defs.h>
#include <drawer.h>

int get_active_menu();
void set_active_menu(int menu);

void draw_game_menu(render_obj_t * object);
int handle_game_menu(SDL_Event event);
void draw_inventory_menu(render_obj_t * object);
int handle_inventory_menu(SDL_Event event);
void draw_pause_menu(render_obj_t * object);
int handle_pause_menu(SDL_Event event);
void draw_crafting_menu(render_obj_t * object);
int handle_crafting_menu(SDL_Event event);
