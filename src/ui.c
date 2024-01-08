#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <stdio.h>
#include <stdlib.h>
#include <drawer.h>
#include <ui.h>
#include <player.h>
#include <camera.h>
#include <map_defs.h>
#include <ticks.h>
#include <menu_defs.h>
#include <menu.h>
#include <inventory.h>

int x_cursor = 0;
int y_cursor = 0;
int ui_x_scale = 1;
int ui_y_scale = 1;
int cols = 0;
int rows = 0;
ui_menu_t next_menu;

TTF_Font * font;

ui_menu_t game_menus[MENU_AMOUNT] = {
  {&handle_game_menu, &draw_game_menu, 0, GAME_UI_ID, PAUSE_UI_ID},
  {&handle_pause_menu, &draw_pause_menu, 0, PAUSE_UI_ID, GAME_UI_ID},
  {&handle_inventory_menu, &draw_inventory_menu, 0, INVENTORY_UI_ID, GAME_UI_ID},
  {&handle_crafting_menu, &draw_crafting_menu, 0, CRAFTING_UI_ID, GAME_UI_ID},
  {&handle_death_menu, &draw_death_menu, 0, DEATH_UI_ID, GAME_UI_ID},
};

void init_ui(){
  get_camera_view(&cols, &rows);
  cols *= 4;
  rows *= 2;
  zero_ui();
  ui_x_scale = (DEFAULT_SCREEN_WIDTH / cols);
  ui_y_scale = (DEFAULT_SCREEN_HEIGHT / rows);
}

void zero_ui(){
  x_cursor = 0;
  y_cursor = 0;
}

void newline_ui(){
  x_cursor = 0;
  y_cursor++;
  y_cursor = y_cursor % rows;
}

void ui_draw_horizon(render_obj_t * object){
	object->surface = SDL_LoadBMP(HORIZON_PATH);
        object->texture = SDL_CreateTextureFromSurface(object->renderer, object->surface);
	transform_t rot = get_player_direction();
	object->clip.y = 0;
	switch (rot.z){
	case -1:
		object->clip.x = 0;
		break;
	case 0:
		object->clip.x = 16;
		break;
	case 1:
		object->clip.x = 32;
		break;
	}
	object->clip.w = 16;
	object->clip.h = 16;
	int width = 64;
	int height = 64;
	object->target.x = 0;
	object->target.y = DEFAULT_SCREEN_HEIGHT - height;
	object->target.w = width;
	object->target.h = height;
	SDL_RenderCopy(object->renderer, object->texture, &object->clip, &object->target);

	SDL_DestroyTexture(object->texture);
	SDL_FreeSurface(object->surface);
}

void draw_string(char * string, SDL_Color color, render_obj_t * object){
  font = TTF_OpenFont(FONT_PATH, 16);
  int width = 0;
  int height = 0;

  object->surface = TTF_RenderText_Solid(font, string, color);
  object->texture = SDL_CreateTextureFromSurface(object->renderer, object->surface);

  SDL_QueryTexture(object->texture, NULL, NULL, &width, &height);
  object->target.x = x_cursor * width * 2;
  object->target.y = y_cursor * height * 2;
  object->target.w = width * 2;
  object->target.h = height * 2;
  SDL_RenderCopy(object->renderer, object->texture, NULL, &object->target);

  SDL_DestroyTexture(object->texture);
  SDL_FreeSurface(object->surface);
  TTF_CloseFont(font);
}

void handle_block_select(SDL_Event event){
  char code = event.key.keysym.sym-48;
  if (code < 0 || code > 9){
    return;
  }
  if (code == 0){
    set_current_item(9);
  } else {
    set_current_item(code);
  }
}

void draw_menu(render_obj_t * object){
  ui_menu_t current_menu;  
  get_menu_info(&current_menu, get_active_menu());
  current_menu.draw(object);
}

void handle_menu(int * exit, int * death, SDL_Event event){
  ui_menu_t current_menu;
  get_menu_info(&current_menu, get_active_menu());
  int result = current_menu.update(event);

  switch(result){
    case HANDLE_DEATH:
      *death = 1;
      break;
    case HANDLE_EXIT:
      *exit = 0;
      break;
    case HANDLE_CLOSE:
      close_menu(&current_menu);
      get_menu_info(&next_menu, get_active_menu());
      open_menu(&next_menu);
      break;
    default:
      break;
  }
}

void get_menu_info(ui_menu_t * info, int id){
  ui_menu_t menu = game_menus[id];
  info->update = menu.update;
  info->draw = menu.draw;
  info->active = menu.active;
  info->id = menu.id;
  info->fallback_id = menu.fallback_id;
}

void open_menu(ui_menu_t * menu){
  for (int id = 0 ; id < MENU_AMOUNT ; id++){
    game_menus[id].active = 0;
  }  
  menu->active = 1;
  set_active_menu(menu->id);
}

void close_menu(ui_menu_t * menu){
  menu->active = 0;
}
