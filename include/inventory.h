#pragma once

#include <map.h>
#include <items.h>

#define SLOT_SIZE 999
#define INVENTORY_SIZE 18

#define TOOL_AMOUNT INVENTORY_SIZE

/* <size> is how many items can fit in the slot */
typedef struct inventory_slot_s {
	block_data_t item;
	item_state_t state;
	int amount;
	int size;
	int is_empty;
} inventory_slot_t;

void init_inventory();
int is_inventory_slot_empty(int slot);
int add_inventory_item(block_data_t item, int amount);
int remove_inventory_item(block_data_t item, int count);

int set_inventory_item_state(item_state_t state, int slot);

inventory_slot_t * get_inventory_item(int slot);
inventory_slot_t * get_current_item();
void set_current_item(int slot);
void sort_inventory();
int check_inventory_item(block_data_t item, int amount);
int is_inventory_full();
int find_inventory_slot(block_data_t item);
void get_tools_list(inventory_slot_t list[TOOL_AMOUNT]);
int get_current_slot();
