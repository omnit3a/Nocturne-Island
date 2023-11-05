#pragma once

#include <SDL2/SDL.h>

#define CONTROL_SCHEME_PATH "assets/defs/controls.json"

#define CONTROL_AMOUNT 1024
#define CONTROL_NAME_LENGTH 24

typedef struct input_def_s {
  char name[CONTROL_NAME_LENGTH];
  char keycode_name[CONTROL_NAME_LENGTH];
  char keycode_result[CONTROL_NAME_LENGTH];
  int any_menu;
  int menu_id;
} input_def_t;

typedef struct input_list_s {
  int size;
  input_def_t inputs[CONTROL_AMOUNT];
} input_list_t;

void load_control_scheme(char * path);
SDL_Keycode translate_keypress(SDL_Event event, int menu_id);
