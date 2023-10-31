#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <crafting.h>
#include <map.h>
#include <inventory.h>
#include <player.h>

crafting_recipe_t recipes[CRAFTING_RECIPE_AMOUNT];
crafting_recipe_t craftable_recipes[CRAFTABLE_LIST_AMOUNT];
int craftable_recipe_count = 0;

void translate_recipe_def(char * def){
  int values[5] = {0, 0, 0, 0, 0};
  crafting_item_t ingredients[8];
  char * token;
  char * delim = " ";
  int field = 0;
  char name[BLOCK_NAME_LENGTH];
  char * start_list = "start";
  char * end_list = "end";
  int in_list = 0;
  int list_count = 0;
  
  token = strtok(def, delim);

  for (int item = 0 ; item < 8 ; item++){
    ingredients[item].item = get_block_properties(0);
    ingredients[item].amount = 0;
  }
  
  while (token != NULL){
    if (in_list){
      if (strcmp(end_list, token) == 0){
	in_list = 0;
      } else {
	if (list_count < 7){
	  ingredients[list_count].item = get_block_properties(atoi(token));
	  token = strtok(NULL, delim);
	  ingredients[list_count].amount = atoi(token);
	  list_count++;
	  goto tokenize;
	} else {
	  in_list = 0;
	}
      }
      goto tokenize;
    }

    if (strcmp(start_list, token) == 0){
      in_list = 1;
      goto tokenize;
    }
    
    if (field == 0){
      strcpy(name, token);
    } else {
      values[field-1] = atoi(token);
    }
    field++;
  tokenize:
    token = strtok(NULL, delim);
  }

  int line = values[2];
  strcpy(recipes[line].name, name);
  for (int item = 0 ; item < list_count ; item++){
    recipes[line].ingredients[item].item = ingredients[item].item;
    recipes[line].ingredients[item].amount = ingredients[item].amount;
  }
  recipes[line].output = get_block_properties(values[0]);
  recipes[line].amount = values[1];
  recipes[line].id = line;
  recipes[line].workshop_id = values[3];
}

void load_crafting_recipes(char * path){
  int recipe_def_size = sizeof(crafting_recipe_t);
  char * recipe_def_copy = malloc(recipe_def_size);
  char ** recipe_def_lines = malloc(256 * CRAFTING_RECIPE_AMOUNT);
  FILE * def_file;
  int current_char = 0;
  
  if ((def_file = fopen(path, "r")) == NULL){
    fprintf(stderr, "Unable to load recipe defs\n");
    exit(-1);
  }

  // make a copy of the data, so that it can be tokenized
  do {
    char def_char = fgetc(def_file);

    if (current_char == recipe_def_size){
      recipe_def_size += 256;
      recipe_def_copy = realloc(recipe_def_copy, recipe_def_size);
    }
    
    if (feof(def_file)){
      break;
    } else {
      recipe_def_copy[current_char] = def_char;
      current_char++;
    }
  } while (true);

  char * def_line;
  char delim[2] = "\n";
  int current_line = 0;

  // tokenize the data
  def_line = strtok(recipe_def_copy, delim);

  while (def_line != NULL){
    recipe_def_lines[current_line++] = def_line;
    def_line = strtok(NULL, delim);
  }

  recipe_def_lines[current_line] = NULL;

  current_line = 0;
  while (recipe_def_lines[current_line] != NULL){
    translate_recipe_def(recipe_def_lines[current_line]);
    current_line++;
  }

  printf("Successfully loaded recipe defs\n");
  
  free(recipe_def_lines);
  free(recipe_def_copy);
  fclose(def_file);
}

int is_recipe_craftable(crafting_recipe_t * list, int recipe){
  transform_t pos = get_player_entity()->position;
  int max_items = 0;
  int found_items = 0;
  for (int item = 0 ; list[recipe].ingredients[item].item.id != 0 ; item++){
    max_items++;
    if (check_inventory_item(list[recipe].ingredients[item].item,
			     list[recipe].ingredients[item].amount)){
      found_items++;
    }
  }

  if (found_items == max_items){
    return 1;
  }
  
  return 0;
}

int craft_item(crafting_recipe_t * list, int recipe){
  if (is_recipe_craftable(list, recipe) && !is_inventory_full()){
    add_inventory_item(list[recipe].output, list[recipe].amount);
    
    for (int item = 0 ; list[recipe].ingredients[item].item.id != 0 ; item++){
      remove_inventory_item(list[recipe].ingredients[item].item,
			    list[recipe].ingredients[item].amount);
    }
    
    return 1;
  } else {
    return 0;
  }
}

void get_craftable_recipes(crafting_recipe_t * recipe_list){
  int filled_list = 0;
  craftable_recipe_count = 0;
  transform_t pos = get_player_entity()->position;
  for (int item = 0 ; item < CRAFTABLE_LIST_AMOUNT ; item++){
    recipe_list[item].id = 0;
  }
  for (int item = 0 ; filled_list < CRAFTABLE_LIST_AMOUNT &&
	 item < CRAFTING_RECIPE_AMOUNT ; item++){
    if (is_recipe_craftable(recipes, item) && is_next_to_workshop(recipes[item].workshop_id, pos.x, pos.y, pos.z)){
      memcpy(&recipe_list[filled_list], &recipes[item], sizeof(crafting_recipe_t));
      recipe_list[filled_list].id = filled_list; 
      filled_list++;
      craftable_recipe_count++;
    }
  }
}

crafting_recipe_t * get_recipe_list(){
  return recipes;
}

int get_craftable_recipe_count(){
  return craftable_recipe_count;
}
