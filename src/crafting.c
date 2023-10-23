#include <stdbool.h>
#include <crafting.h>
#include <player.h>
#include <map.h>
#include <drawer.h>
#include <string.h>
#include <ui.h>
#include <inventory.h>
#include <messages.h>
#include <map_defs.h>

crafting_recipe_t recipes[CRAFTING_RECIPE_COUNT] = {
};

crafting_recipe_t craftable_recipes[CRAFTABLE_RECIPES_COUNT];
crafting_recipe_t current_recipe;

void craftRecipe(crafting_recipe_t recipe){
  char temp_string[256];
  
  if (checkFullInventory(3)){
    sprintf(temp_string, UNCRAFTABLE_FULL_MSG, blockNames[recipe.output_block]);
    strcpy(messageBar, temp_string);
    return;
  }

  /* Total up items in players inventory */
  groupInventoryItems();
  
  /* Check if player can craft recipe */
  if (checkCraftableRecipe(recipe)){
      for (int n = 0 ; n < recipe.amount_of_inputs ; n++){
	checkAndRemoveItem(recipe.input_items[n].block, recipe.input_items[n].count);
      }
      addItemToInventory(recipe.output_block, recipe.output_count);
      sprintf(temp_string, CRAFTING_SUCCESS_MSG, blockNames[recipe.output_block]);
      strcpy(messageBar, temp_string);
      return;
    }
    sprintf(temp_string, UNCRAFTABLE_EMPTY_MSG, blockNames[recipe.output_block]);
    strcpy(messageBar, temp_string);
}

void interactWithWorkbench(char map[MAP_WIDTH][MAP_LENGTH][MAP_HEIGHT]){
  playerOffsetDirection();
  /*
  if (getBlockProperties(map, playerXOff, playerYOff, playerZOff).block == WORK_BENCH){
    if (current_recipe.output_block > 0){
      craftRecipe(current_recipe);
    }
  }
  */
}

bool checkCraftableRecipe(crafting_recipe_t recipe){
  if (checkFullInventory(3)){
    return false;
  }

  groupInventoryItems();
  
  /* Check if player can craft recipe */
  int correct_input_items = 0;
  for (int i = 0 ; i < INVENTORY_SIZE ; i++){
    if (inventory[i].block > 0){
      for (int j = 0 ; j < recipe.amount_of_inputs ; j++){
	if (inventory[i].block == recipe.input_items[j].block && inventory[i].count >= recipe.input_items[j].count){
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
