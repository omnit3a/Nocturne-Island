#ifndef INVENTORY_H_
#define INVENTORY_H_

#include <drawer.h>
#include <stdbool.h>

#define INVENTORY_SIZE 10
#define MAX_ITEM_COUNT 4294967295

typedef struct item_s {
  int block;
  unsigned int count;
} item_t;

extern item_t inventory[INVENTORY_SIZE];

void initInventory();
void addItemToInventory(int block, unsigned int count);
bool checkInventoryForItem(int block);
bool checkAndRemoveItem(int block, unsigned int count);
item_t countInventoryItem(int block);
bool checkFullInventory(int threshold);
void groupInventoryItems();
#endif
