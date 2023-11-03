#include <stdio.h>
#include <stdlib.h>
#include <inventory.h>
#include <map_defs.h>
#include <map.h>
#include <string.h>

inventory_slot_t inventory[INVENTORY_SIZE];
int current_slot;

void init_inventory(){
  current_slot = 0;
  for (int slot = 0 ; slot < INVENTORY_SIZE ; slot++){
    inventory[slot].item = get_block_properties(EMPTY);
    inventory[slot].amount = 0;
    inventory[slot].size = SLOT_SIZE;
    inventory[slot].is_empty = 1;
  }
}

int is_inventory_slot_empty(int slot){
  return inventory[slot].is_empty;
}

int add_inventory_item(block_data_t item, int amount){
  sort_inventory();
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
  sort_inventory();
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

void set_current_item(int slot){
  current_slot = slot;
}


void sort_inventory(){
  int item_count[BLOCKS_AMOUNT];
  block_data_t prev_block = get_current_item()->item;
  for (int id = 0 ; id < BLOCKS_AMOUNT ; id++){
    item_count[id] = 0;
    if (id == 0){
      continue;
    }
    for (int slot = 0 ; slot < INVENTORY_SIZE ; slot++){
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
  int prev_slot = find_inventory_slot(prev_block);
  set_current_item(prev_slot);
}

int check_inventory_item(block_data_t item, int amount){
  for (int slot = 0 ; slot < INVENTORY_SIZE ; slot++){
    if (compare_blocks(inventory[slot].item, item) &&
	inventory[slot].amount >= amount){
      return 1;
    }
  }
  return 0;
}

int is_inventory_full(){
  for (int slot = 0 ; slot < INVENTORY_SIZE ; slot++){
    if (inventory[slot].is_empty){
      return 0;
    }
  }
  return 1;
}

int find_inventory_slot(block_data_t item){
  for (int slot = 0 ; slot < INVENTORY_SIZE ; slot++){
    if (inventory[slot].item.id == item.id){
      return slot;
    }
  }
  return -1;
}

void get_tools_list(inventory_slot_t list[TOOL_AMOUNT]){
  int list_slot = 0;
  list[list_slot].item.id = 0;
  for (int slot = 0 ; slot < INVENTORY_SIZE ; slot++){
    if (inventory[slot].item.block_type == TOOL_TYPE){
      list[list_slot++] = inventory[slot];
    } else {
      list[slot].item = get_block_properties(EMPTY);
      strcpy(list[slot].item.name, "Nothing");
      list[slot].item.id = 0;
      list[slot].amount = 0;
      list[slot].size = SLOT_SIZE;
      list[slot].is_empty = 1;
    }
  }
}
