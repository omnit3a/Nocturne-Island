#include <SDL2/SDL.h>
#include <ui.h>
#include <inventory.h>
#include <drawer.h>
#include <stdbool.h>

item_t inventory[INVENTORY_SIZE];

/* Fill inventory with empty slots */
void initInventory(){
  for (int i = 0 ; i < INVENTORY_SIZE ; i++){
    inventory[i].block = 0;
    inventory[i].count = 0;
  }
}

/* Check for valid slot in inventory, if one is found then store item */
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

/* Check if player has item in inventory */
bool checkInventoryForItem(blocks_t block){
  for (int i = 0 ; i < INVENTORY_SIZE ; i++){
    if (inventory[i].block == block){
      return true;
    }
  }
  return false;
}

/* Check if player has item in inventory, if they do, then remove that item */
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
