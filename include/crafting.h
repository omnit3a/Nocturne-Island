#ifndef CRAFTING_H_
#define CRAFTING_H_

#include <inventory.h>
#include <stdbool.h>
#include <map.h>

#define CRAFTING_RECIPE_COUNT 512
#define CRAFTABLE_RECIPES_COUNT 26

typedef struct crafting_item_s {
  int block;
  int count;
} crafting_item_t;

typedef struct crafting_recipe_s {
  int amount_of_inputs;
  crafting_item_t input_items[INVENTORY_SIZE];
  int output_block;
  int output_count;
} crafting_recipe_t;

extern crafting_recipe_t recipes[CRAFTING_RECIPE_COUNT];
extern crafting_recipe_t craftable_recipes[CRAFTABLE_RECIPES_COUNT];
extern crafting_recipe_t current_recipe;

void craftRecipe(crafting_recipe_t recipe);
bool checkCraftableRecipe(crafting_recipe_t recipe);
void interactWithWorkbench(char map[MAP_WIDTH][MAP_LENGTH][MAP_HEIGHT]);
void listCraftableItems();

#endif
