#ifndef MAP_H_
#define MAP_H_
#include <SDL2/SDL.h>
#include <stdbool.h>

#define MAP_WIDTH 100
#define MAP_LENGTH 100
#define MAP_HEIGHT 50

#include <drawer.h>

#define GROUND_LEVEL 2 /* Base ground thickness */
#define TREE_CHANCE 20 /* Chance out of 1000 that a tree spawns on a tile */
#define IRON_CHANCE 100
#define COAL_CHANCE 100
#define CLIFF_HEIGHT 10
#define GROUND_HEIGHT 5

#define BLOCKS_AMOUNT 256

typedef struct block_data_s {
  int hp;
  bool solid;
  bool transparent;
  int block;
  int dropped_item;
  int count;
} block_data_t;

extern char world_map[MAP_WIDTH][MAP_LENGTH][MAP_HEIGHT];
extern block_data_t data_map[BLOCKS_AMOUNT];

block_data_t getBlockProperties(char map[MAP_WIDTH][MAP_LENGTH][MAP_HEIGHT], int xPos, int yPos, int zPos);
void fillMap(char map[MAP_WIDTH][MAP_LENGTH][MAP_HEIGHT]);
void placeTrees(char map[MAP_WIDTH][MAP_LENGTH][MAP_HEIGHT],char height_map[MAP_WIDTH][MAP_LENGTH], int seed);
void placeOres(char map[MAP_WIDTH][MAP_LENGTH][MAP_HEIGHT], char height_map[MAP_WIDTH][MAP_LENGTH], int seed);
void generateHills(char map[MAP_WIDTH][MAP_LENGTH][MAP_HEIGHT], int seed);
void cullHiddenBlocks(char dest_map[MAP_WIDTH][MAP_LENGTH][MAP_HEIGHT], char src_map[MAP_WIDTH][MAP_LENGTH][MAP_HEIGHT]);
#endif
