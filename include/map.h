#ifndef MAP_H_
#define MAP_H_
#include <SDL2/SDL.h>
#include <stdbool.h>

#define MAP_WIDTH 100
#define MAP_LENGTH 100
#define MAP_HEIGHT 50

#define HILL_SIZE 2
#define GROUND_LEVEL 2
#define TREE_CHANCE 20
#define CLIFF_HEIGHT 10
#define GROUND_HEIGHT 5

extern char world_map[MAP_WIDTH][MAP_LENGTH][MAP_HEIGHT];
extern bool solid_map[MAP_WIDTH][MAP_LENGTH][MAP_HEIGHT];

void fillMap(char map[MAP_WIDTH][MAP_LENGTH][MAP_HEIGHT]);
void placeTrees(char map[MAP_WIDTH][MAP_LENGTH][MAP_HEIGHT],char height_map[MAP_WIDTH][MAP_LENGTH], int seed);
void generateHills(char map[MAP_WIDTH][MAP_LENGTH][MAP_HEIGHT], int seed);
void cullHiddenBlocks(char dest_map[MAP_WIDTH][MAP_LENGTH][MAP_HEIGHT], char src_map[MAP_WIDTH][MAP_LENGTH][MAP_HEIGHT]);
void generateSolidity(char map[MAP_WIDTH][MAP_LENGTH][MAP_HEIGHT]);
#endif

