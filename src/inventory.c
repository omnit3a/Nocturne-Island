#include <SDL2/SDL.h>
#include <ui.h>
#include <inventory.h>
#include <drawer.h>
#include <stdbool.h>

item_t inventory[INVENTORY_SIZE];

void initInventory(){
  for (int i = 0 ; i < INVENTORY_SIZE ; i++){
    inventory[i].block = 0;
    inventory[i].count = 0;
  }
}

void addItemToInventory(blocks_t block, unsigned char count){
  for (int i = 0 ; i < INVENTORY_SIZE ; i++){
    if (inventory[i].count == 0){
      inventory[i].block = 0;
    }
    if (inventory[i].block == block || inventory[i].block == 0){
      if (count == 255){
	continue;
      } else {
	inventory[i].block = block;
	inventory[i].count+=count;
	break;
      }
    }
  }
}

bool checkInventoryForItem(blocks_t block){
  for (int i = 0 ; i < INVENTORY_SIZE ; i++){
    if (inventory[i].block == block){
      return true;
    }
  }
  return false;
}

bool checkAndRemoveItem(blocks_t block, unsigned char count){
  for (int i = 0 ; i < INVENTORY_SIZE ; i++){
    if (inventory[i].block == block && inventory[i].count >= count){
      inventory[i].count -= count;
      if (inventory[i].count == 0){
	inventory[i].block = 0;
      }
      return true;
    }
  }
  return false;
}
