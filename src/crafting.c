#include <stdbool.h>
#include <crafting.h>
#include <player.h>
#include <map.h>
#include <drawer.h>
#include <string.h>
#include <ui.h>
#include <inventory.h>

crafting_recipe_t recipes[CRAFTING_RECIPE_COUNT] = {
  {1,{{STONE, 3}}, STAIRS, 1},
  {1,{{WOOD, 8}}, WORK_BENCH, 1},
  {1,{{WOOD, 6}}, TABLE, 1},
  {1,{{WOOD, 2}}, TREE_BOTTOM, 1},
};

crafting_recipe_t craftable_recipes[CRAFTABLE_RECIPES_COUNT];
crafting_recipe_t current_recipe;

void craftRecipe(crafting_recipe_t recipe){
  item_t item_count[INVENTORY_SIZE] = {0};
  char temp_string[256];

  if (checkFullInventory(3)){
    sprintf(temp_string, "Cannot craft %s ; Inventory full",blockNames[recipe.output_block]);
    strcpy(messageBar, temp_string);
    return;
  }
  
  /* Total up the amount of each item */
  for (int i = 0 ; i < INVENTORY_SIZE ; i++){
    if (inventory[i].block > 0){
      for (int j = 0 ; j < INVENTORY_SIZE ; j++){
	if (item_count[j].block == inventory[i].block){
	  break;
	} else {
	  item_count[i].block = countInventoryItem(inventory[i].block).block;
	  item_count[i].count = countInventoryItem(inventory[i].block).count;
	  break;
	}
      }
    }
  }

  /* Check if player can craft recipe */
  int correct_input_items = 0;
  for (int i = 0 ; i < INVENTORY_SIZE ; i++){
    if (item_count[i].block > 0){
      for (int j = 0 ; j < recipe.amount_of_inputs ; j++){
	if (item_count[i].block == recipe.input_items[j].block && item_count[i].count >= recipe.input_items[j].count){
	  correct_input_items++;
	}
      }
      if (correct_input_items == recipe.amount_of_inputs){
	for (int n = 0 ; n < recipe.amount_of_inputs ; n++){
	  checkAndRemoveItem(recipe.input_items[n].block, recipe.input_items[n].count);
	}
	addItemToInventory(recipe.output_block, recipe.output_count);
	sprintf(temp_string, "Succesfully crafted %s", blockNames[recipe.output_block]);
	strcpy(messageBar, temp_string);
	return;
      }
    }
  }
  sprintf(temp_string, "Cannot craft %s; Not enough resources",blockNames[recipe.output_block]);
  strcpy(messageBar, temp_string);
}

void interactWithWorkbench(char map[MAP_WIDTH][MAP_LENGTH][MAP_HEIGHT]){
  playerOffsetDirection();
  if (getBlockProperties(map, playerXOff, playerYOff, playerZOff).block == WORK_BENCH){
    if (current_recipe.output_block > 0){
      craftRecipe(current_recipe);
    }
  }
}

bool checkCraftableRecipe(crafting_recipe_t recipe){
  item_t item_count[INVENTORY_SIZE] = {0};
  
  if (checkFullInventory(3)){
    return false;
  }
  
  /* Total up the amount of each item */
  for (int i = 0 ; i < INVENTORY_SIZE ; i++){
    if (inventory[i].block > 0){
      for (int j = 0 ; j < INVENTORY_SIZE ; j++){
	if (item_count[j].block == inventory[i].block){
	  break;
	} else {
	  item_count[i].block = countInventoryItem(inventory[i].block).block;
	  item_count[i].count = countInventoryItem(inventory[i].block).count;
	  break;
	}
      }
    }
  }

  /* Check if player can craft recipe */
  int correct_input_items = 0;
  for (int i = 0 ; i < INVENTORY_SIZE ; i++){
    if (item_count[i].block > 0){
      for (int j = 0 ; j < recipe.amount_of_inputs ; j++){
	if (item_count[i].block == recipe.input_items[j].block && item_count[i].count >= recipe.input_items[j].count){
	  correct_input_items++;
	}
      }
      if (correct_input_items == recipe.amount_of_inputs){
	return true;
      }
    }
  }
  return false;
}

void listCraftableItems(){
  int list_iterator = 0;
  for (int i = 0 ; i < CRAFTING_RECIPE_COUNT ; i++){
    if (checkCraftableRecipe(recipes[i])){
      craftable_recipes[list_iterator++] = recipes[i];
    }
    if (i == CRAFTING_RECIPE_COUNT-1 || list_iterator == CRAFTABLE_RECIPES_COUNT){
      break;
    }
  }
}
