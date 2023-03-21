#ifndef INVENTORY_H_
#define INVENTORY_H_

#include <drawer.h>
#include <stdbool.h>

#define INVENTORY_SIZE 10
#define MAX_ITEM_COUNT 4294967295

typedef struct item_s {
  blocks_t block;
  unsigned int count;
} item_t;

extern item_t inventory[INVENTORY_SIZE];

void initInventory();
void addItemToInventory(blocks_t block, unsigned int count);
bool checkInventoryForItem(blocks_t block);
bool checkAndRemoveItem(blocks_t block, unsigned int count);
item_t countInventoryItem(blocks_t block);
bool checkFullInventory(int threshold);
void groupInventoryItems();
#endif
