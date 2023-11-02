#pragma once

#include <map.h>

#define CRAFTING_DATA_PATH "assets/defs/recipes.txt"
#define CRAFTING_RECIPE_AMOUNT 256
#define CRAFTING_NAME_LENGTH 24
#define CRAFTING_ITEMS_AMOUNT 8
#define CRAFTABLE_LIST_AMOUNT 18

typedef struct crafting_item_s {
  block_data_t item;
  int amount;
} crafting_item_t;

typedef struct crafting_recipe_s {
  char name[CRAFTING_NAME_LENGTH];
  crafting_item_t ingredients[CRAFTING_ITEMS_AMOUNT];
  block_data_t output;
  int amount;
  int workshop_id;
  int autoplace;
  int id;
} crafting_recipe_t;

void translate_recipe_def(char * def);
void load_crafting_recipes(char * path);
int is_recipe_craftable(crafting_recipe_t * list, int recipe);
int craft_item(crafting_recipe_t * list, int recipe);
void get_craftable_recipes(crafting_recipe_t * recipe_list);
crafting_recipe_t * get_recipe_list();
int get_craftable_recipe_count();
