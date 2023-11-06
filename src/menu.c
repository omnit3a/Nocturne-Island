#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <menu_defs.h>
#include <menu.h>
#include <drawer.h>
#include <string.h>
#include <player.h>
#include <ui.h>
#include <inventory.h>
#include <crafting.h>
#include <messages.h>
#include <ticks.h>
#include <tools.h>
#include <controls.h>

int active_menu = 0;

SDL_Color text_white = {255, 255, 255};
SDL_Color text_gray = {128, 128, 128};
SDL_Color text_black = {0, 0, 0};
SDL_Color text_red = {255, 0, 0};
SDL_Color text_green = {0, 255, 0};
SDL_Color text_blue = {0, 0, 255};

int get_active_menu(){
  return active_menu;
}

void set_active_menu(int menu){
  active_menu = menu;
}

void draw_game_menu(render_obj_t * object){
  block_data_t block;
  char format[40] = "";
  char text[20];
  init_ui();
  block = get_current_item()->item;
  strcpy(format, CURRENT_VERSION_MSG);
  draw_string(format, text_white, object);
  newline_ui();

  strcpy(format, "");
  strcat(format, CURRENT_BLOCK_MSG);
  if (block.id == 0){
    strcat(format, EMPTY_MSG);
  } else {
    strcat(format, block.name);
  }
  draw_string(format, text_white, object);
  newline_ui();

  strcpy(format, "");
  sprintf(format, HP_MSG, get_player_health());
  draw_string(format, text_white, object);
  newline_ui();
  
  strcpy(format, "");
  sprintf(format, HUNGER_MSG, get_player_hunger());
  draw_string(format, text_white, object);
  newline_ui();
  
  strcpy(format, "");
  sprintf(format, THIRST_MSG, get_player_thirst());
  draw_string(format, text_white, object);
  newline_ui();

  if (get_block_progress() > 0){
    strcpy(format, "");
    strcat(format, BLOCK_HP_MSG);
    sprintf(text, "%d", get_block_progress());
    strcat(format, text);
    draw_string(format, text_white, object);
    
    newline_ui();
  }
}

/* Switch between UI Modes */
int handle_game_menu(SDL_Event event){

  if (get_player_health() <= 0){
    active_menu = DEATH_UI_ID;
    return HANDLE_CLOSE;
  }

  int keycode = translate_keypress(event, active_menu);
  
  switch (keycode){
    case SDLK_e:
      active_menu = INVENTORY_UI_ID;
      return HANDLE_CLOSE;

    case SDLK_c:
      active_menu = CRAFTING_UI_ID;
      return HANDLE_CLOSE;

    case SDLK_t:
      handle_action(get_current_item()->item.id);
      return HANDLE_CLOSE;
      
    case SDLK_ESCAPE:
      active_menu = PAUSE_UI_ID;
      return HANDLE_CLOSE;
  }
  
  return HANDLE_REGULAR;
}

void draw_inventory_menu(render_obj_t * object){
  init_ui();
  block_data_t block;
  char slot_label[] = " - ";
  char format[40];
  char amount[20];
  SDL_Color select_color;
  for (int slot = 0 ; slot < INVENTORY_SIZE ; slot++){
    select_color = text_white;
    if (slot == get_current_slot()){
      select_color = text_green;
    }

    block = get_inventory_item(slot)->item;
    slot_label[1] = slot + 97;
    strcpy(format, slot_label);
    if (block.id == 0){
      strcat(format, EMPTY_MSG);
      draw_string(format, select_color, object);
      newline_ui();
      continue;
    }
    
    strcat(format, block.name);
    sprintf(amount, ": %d", get_inventory_item(slot)->amount);
    strcat(format, amount);
    draw_string(format, select_color, object);
    
    newline_ui();
  }
}

int handle_inventory_menu(SDL_Event event){
  int keycode = event.key.keysym.sym-97;
  if (keycode >= 0 && keycode < INVENTORY_SIZE){
    set_current_item(keycode);
    return HANDLE_REGULAR;
  }

  keycode = event.key.keysym.sym;
  
  switch(keycode){
    case SDLK_ESCAPE:
      active_menu = GAME_UI_ID;
      return HANDLE_CLOSE;
  }
  return HANDLE_REGULAR;
}

void draw_pause_menu(render_obj_t * object){
  char format[40];
  strcpy(format, "");
  init_ui();
  draw_string(MENU_NAME_MSG, text_white, object);
  newline_ui();
  
  sprintf(format, MENU_EXIT_MSG, get_keycode_name(SDLK_ESCAPE, PAUSE_UI_ID));
  draw_string(format, text_white, object);
  newline_ui();

  sprintf(format, MENU_CLOSE_MSG, get_keycode_name(SDLK_q, PAUSE_UI_ID));
  draw_string(format, text_white, object);
  newline_ui();
}

int handle_pause_menu(SDL_Event event){
  int keycode = translate_keypress(event, active_menu);
  switch(keycode){
    case SDLK_q:
      return HANDLE_EXIT;
    case SDLK_ESCAPE:
      active_menu = GAME_UI_ID;
      return HANDLE_CLOSE;
  }
  return HANDLE_REGULAR;
}

crafting_recipe_t recipe_list[CRAFTABLE_LIST_AMOUNT];

void draw_crafting_menu(render_obj_t * object){
  init_ui();
  get_craftable_recipes(recipe_list);
  int line_number = 1;
  char slot_label[4] = " - ";
  char format[40];
  
  while (line_number <= CRAFTABLE_LIST_AMOUNT){
    slot_label[1] = line_number+96;
    strcpy(format, slot_label);
    strcat(format, recipe_list[line_number].name);
    draw_string(format, text_white, object);
    newline_ui();
    line_number++;
  }
}

int handle_crafting_menu(SDL_Event event){
  get_craftable_recipes(recipe_list);
  
  int keycode = event.key.keysym.sym-96;
  if (keycode >= 0 && keycode < CRAFTABLE_LIST_AMOUNT){
    if (craft_item(recipe_list, keycode)){
      return HANDLE_CLOSE;
    } else {
      return HANDLE_REGULAR;
    }
  }

  keycode = event.key.keysym.sym;
  switch(keycode){
    case SDLK_ESCAPE:
      active_menu = GAME_UI_ID;
      return HANDLE_CLOSE;
  }
  return HANDLE_REGULAR;
}

void draw_death_menu(render_obj_t * object){
  init_ui();
  char format[40];
  char amount[20];

  draw_string(PLAYER_DEAD_MSG, text_red, object);
  newline_ui();

  strcpy(format, DAYS_SURVIVED_MSG);
  sprintf(amount, "%d", get_days_survived());
  strcat(format, amount);
  draw_string(format, text_white, object);
  newline_ui();

  sprintf(format, NEW_GAME_MSG, get_keycode_name(SDLK_r, DEATH_UI_ID));
  draw_string(format, text_white, object);
  newline_ui();

  sprintf(format, END_GAME_MSG, get_keycode_name(SDLK_q, DEATH_UI_ID));
  draw_string(format, text_white, object);
  newline_ui();
}

int handle_death_menu(SDL_Event event){

  int keycode = translate_keypress(event, active_menu);
  
  switch(keycode){
    case SDLK_r:
      return HANDLE_DEATH;
    case SDLK_q:
      return HANDLE_EXIT;
  }
  return HANDLE_REGULAR;
}
