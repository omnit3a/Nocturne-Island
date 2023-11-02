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

int active_menu = 0;

int get_active_menu(){
  return active_menu;
}

void set_active_menu(int menu){
  active_menu = menu;
}

void draw_game_menu(render_obj_t * object){
  block_data_t block;
  char format[20];
  init_ui();
  block = get_current_item()->item;
  draw_string(CURRENT_VERSION_MSG, object);
  newline_ui();

  draw_string(CURRENT_BLOCK_MSG, object);
  if (block.id == 0){
    draw_string("Nothing", object);
  } else {
    draw_string(block.name, object);
  }

  newline_ui();

  draw_string("Hunger: ", object);
  sprintf(format, "%d", get_player_hunger());
  draw_string(format, object);
  
  newline_ui();

  if (get_block_progress() > 0){
    draw_string("Block HP: ", object);
    sprintf(format, "%d", get_block_progress());
    draw_string(format, object);
    
    newline_ui();
  }
}

/* Switch between UI Modes */
int handle_game_menu(SDL_Event event){
  handle_block_select(event);
  switch (event.key.keysym.sym){
    case SDLK_e:
      active_menu = INVENTORY_UI_ID;
      return HANDLE_CLOSE;

    case SDLK_c:
      active_menu = CRAFTING_UI_ID;
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
  char amount[20];
  for (int slot = 1 ; slot < INVENTORY_SIZE+1 ; slot++){
    if (slot == INVENTORY_SIZE){
      slot = 0;
    }
    block = get_inventory_item(slot)->item;
    slot_label[1] = slot + 48;
    draw_string(slot_label, object);
    if (block.id == 0){
      draw_string("Nothing", object);
      newline_ui();
      if (slot == 0){
	break;
      }
      continue;
    }
    draw_string(block.name, object);
    sprintf(amount, ": %d", get_inventory_item(slot)->amount);
    draw_string(amount, object);
    
    newline_ui();
    
    if (slot == 0){
      break;
    }
  }
}

int handle_inventory_menu(SDL_Event event){
  switch(event.key.keysym.sym){
    case SDLK_ESCAPE:
      active_menu = GAME_UI_ID;
      return HANDLE_CLOSE;
  }
  return HANDLE_REGULAR;
}

void draw_pause_menu(render_obj_t * object){
  init_ui();
  draw_string("Nocturne Island", object);
  newline_ui();
  draw_string("Press ESC to continue", object);
  newline_ui();
  draw_string("Press Q to exit", object);
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

  while (line_number <= CRAFTABLE_LIST_AMOUNT){
    slot_label[1] = line_number+96;
    draw_string(slot_label, object);
    draw_string(recipe_list[line_number].name, object);
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
