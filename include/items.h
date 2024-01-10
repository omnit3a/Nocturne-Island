#ifndef ITEMS_H
#define ITEMS_H

#include <stdbool.h>

#define ITEM_DATA_PATH "assets/defs/items.json"

#define ITEM_NAME_LENGTH 24
#define ITEMS_AMOUNT 256

typedef struct item_data_s {
	char name[ITEM_NAME_LENGTH];
        bool absorbs_liquid;
	int container_size;
	int block_id;
	int id;
} item_data_t;

typedef struct item_map_s {
	int size;
	item_data_t data[ITEMS_AMOUNT];
} item_map_t;

typedef struct item_state_s {
	int fullness;
} item_state_t;

void load_item_properties(char * path);
item_data_t get_item_properties(int item_id);
int get_item_id(int block_id);

#endif
