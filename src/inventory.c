#include <stdio.h>
#include <stdlib.h>
#include <inventory.h>
#include <map_defs.h>
#include <map.h>

inventory_slot_t inventory[INVENTORY_SIZE];
int current_slot;

void init_inventory(){
  current_slot = 0;
  inventory_slot_t empty = {
    get_block_properties(EMPTY),
    0,
    SLOT_SIZE,
    1
  };
  for (int slot = 0 ; slot < INVENTORY_SIZE ; slot++){
    inventory[slot] = empty;
  }
}

int is_inventory_slot_empty(int slot){
  return inventory[slot].is_empty;
}

int add_inventory_item(block_data_t item, int amount){
  for (int slot = 0 ; slot < INVENTORY_SIZE ; slot++){
    if (is_inventory_slot_empty(slot) || compare_blocks(inventory[slot].item, item)){
      if (inventory[slot].amount + amount > inventory[slot].size){
	return 0;
      }
      inventory[slot].item = item;
      inventory[slot].amount += amount;
      inventory[slot].is_empty = 0;
      sort_inventory();
      return 1;
    }
  }
  return 0;
}

int remove_inventory_item(block_data_t item, int amount){
  for (int slot = 0 ; slot < INVENTORY_SIZE ; slot++){
    if (!is_inventory_slot_empty(slot) && compare_blocks(inventory[slot].item, item)){
      if (inventory[slot].amount - amount > 0){
	inventory[slot].amount -= amount;
	sort_inventory();
	return 1;
      }
      if (inventory[slot].amount - amount == 0){
	inventory[slot].amount = 0;
	inventory[slot].is_empty = 1;
	sort_inventory();
	return 1;
      }
    }
  }
  return 0;
}

inventory_slot_t * get_inventory_item(int slot){
  return &inventory[slot];
}

inventory_slot_t * get_current_item(){
  return &inventory[current_slot];
}

void sort_inventory(){
  int item_count[BLOCKS_AMOUNT];
  for (int id = 0 ; id < BLOCKS_AMOUNT ; id++){
    item_count[id] = 0;
    for (int slot = 0 ; slot < INVENTORY_SIZE-1 ; slot++){
      if (get_inventory_item(slot)->item.id == id){
	item_count[id] += get_inventory_item(slot)->amount;
      }
    }
  }
  init_inventory();
  int items_added = 0;
  for (int id = 0 ; id < BLOCKS_AMOUNT && items_added < INVENTORY_SIZE; id++){
    if (item_count[id] > 0){
      inventory[items_added].item = get_block_properties(id);
      inventory[items_added].amount = item_count[id];
      inventory[items_added].is_empty = 0;
      items_added++;
    }
  }
}

