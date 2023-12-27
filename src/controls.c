#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <map.h>
#include <mjson.h>
#include <controls.h>

input_list_t control_defs;

void load_control_scheme(char * path){
  int control_def_size = 1024;
  char * control_def_copy = malloc(control_def_size);
  FILE * def_file;
  int current_char = 0;
  
  if ((def_file = fopen(path, "r")) == NULL){
    fprintf(stderr, "Unable to load control scheme\n");
    exit(-1);
  }

  // make a copy of the data, so that it can be tokenized
  do {
    char def_char = fgetc(def_file);

    if (current_char == control_def_size){
      control_def_size += 256;
      control_def_copy = realloc(control_def_copy, control_def_size);
    }
    
    if (feof(def_file)){
      break;
    } else {
      control_def_copy[current_char] = def_char;
      current_char++;
    }
  } while (true);

  const struct json_attr_t json_subobjects[] = {
    {"name", t_string, STRUCTOBJECT(input_def_t, name), .len = CONTROL_NAME_LENGTH},
    {"keycode_name", t_string, STRUCTOBJECT(input_def_t, keycode_name), .len = CONTROL_NAME_LENGTH},
    {"keycode_result", t_string, STRUCTOBJECT(input_def_t, keycode_result), .len = CONTROL_NAME_LENGTH},
    {"any_menu", t_boolean, STRUCTOBJECT(input_def_t, any_menu)},
    {"menu_id", t_integer, STRUCTOBJECT(input_def_t, menu_id)},
    {NULL},
  };

  const struct json_attr_t json_subobject_list[] = {
    {"class", t_check,.dflt.check = "CONTROLS"},
    {"controls", t_array, STRUCTARRAY(control_defs.inputs,
				      json_subobjects,
				      &control_defs.size)},
    {NULL},
  };

  memset(&control_defs, '\0', sizeof(control_defs));
  json_read_object(control_def_copy, json_subobject_list, NULL);

  printf("Successfully loaded control scheme defs\n");
  
  free(control_def_copy);
  fclose(def_file);
}

SDL_Keycode translate_keypress(SDL_Event event, int menu_id){
  for (int id = 0 ; id < control_defs.size ; id++){
    SDL_Keycode code = SDL_GetKeyFromName(control_defs.inputs[id].keycode_name);
    if (event.key.keysym.sym == code &&
	(control_defs.inputs[id].menu_id == menu_id || control_defs.inputs[id].any_menu)){
      return SDL_GetKeyFromName(control_defs.inputs[id].keycode_result);
    }
  }
  return SDLK_0;
}

char * get_keycode_name(SDL_Keycode keycode, int menu_id){
  for (int id = 0 ; id < control_defs.size ; id++){
    SDL_Keycode search_code = SDL_GetKeyFromName(control_defs.inputs[id].keycode_result);
    if (search_code == keycode &&
	(menu_id == control_defs.inputs[id].menu_id || control_defs.inputs[id].any_menu)){
      return control_defs.inputs[id].keycode_name;
    }
  }
  return "";
}
