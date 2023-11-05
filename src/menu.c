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

int active_menu = 0;

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
  draw_string(format, object);
  newline_ui();

  strcpy(format, "");
  strcat(format, CURRENT_BLOCK_MSG);
  if (block.id == 0){
    strcat(format, EMPTY_MSG);
  } else {
    strcat(format, block.name);
  }
  draw_string(format, object);

  newline_ui();

  strcpy(format, "");
  strcat(format, HP_MSG);
  sprintf(text, "%d", get_player_health());
  strcat(format, text);
  draw_string(format, object);

  newline_ui();
  
  strcpy(format, "");
  strcat(format, HUNGER_MSG);
  sprintf(text, "%d", get_player_hunger());
  strcat(format, text);
  draw_string(format, object);
  
  newline_ui();

  if (get_block_progress() > 0){
    strcpy(format, "");
    strcat(format, BLOCK_HP_MSG);
    sprintf(text, "%d", get_block_progress());
    strcat(format, text);
    draw_string(format, object);
    
    newline_ui();
  }
}

/* Switch between UI Modes */
int handle_game_menu(SDL_Event event){
  switch (event.key.keysym.sym){
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
  char slot_label[4] = " - ";
  char format[40];
  char amount[20];
  for (int slot = 0 ; slot < INVENTORY_SIZE ; slot++){
    block = get_inventory_item(slot)->item;
    slot_label[1] = slot + 97;
    strcpy(format, slot_label);
    if (block.id == 0){
      strcat(format, EMPTY_MSG);
      draw_string(format, object);
      newline_ui();
      continue;
    }
    strcat(format, block.name);
    sprintf(amount, ": %d", get_inventory_item(slot)->amount);
    strcat(format, amount);
    draw_string(format, object);
    
    newline_ui();
  }
}

int handle_inventory_menu(SDL_Event event){
  char code = event.key.keysym.sym-97;
  if (code >= 0 && code <= INVENTORY_SIZE){
    set_current_item(code);
    return HANDLE_REGULAR;
  }

  switch(event.key.keysym.sym){
    case SDLK_ESCAPE:
      active_menu = GAME_UI_ID;
      return HANDLE_CLOSE;
  }
  return HANDLE_REGULAR;
}

void draw_pause_menu(render_obj_t * object){
  init_ui();
  draw_string(MENU_NAME_MSG, object);
  newline_ui();
  draw_string(MENU_EXIT_MSG, object);
  newline_ui();
  draw_string(MENU_CLOSE_MSG, object);
  newline_ui();
}

int handle_pause_menu(SDL_Event event){
  switch(event.key.keysym.sym){
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
    draw_string(format, object);
    newline_ui();
    line_number++;
  }
}

int handle_crafting_menu(SDL_Event event){
  get_craftable_recipes(recipe_list);
  
  char code = event.key.keysym.sym-96;
  if (code >= 0 && code <= CRAFTABLE_LIST_AMOUNT){
    if (craft_item(recipe_list, code)){
      return HANDLE_CLOSE;
    } else {
      return HANDLE_REGULAR;
    }
  }
  switch(event.key.keysym.sym){
    case SDLK_ESCAPE:
      active_menu = GAME_UI_ID;
      return HANDLE_CLOSE;
  }
  return HANDLE_REGULAR;
}
