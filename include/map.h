#ifndef MAP_H_
#define MAP_H_
#include <SDL2/SDL.h>
#include <stdbool.h>

#define BLOCK_DATA_PATH "assets/defs/blocks.txt"

#define MAP_WIDTH 100
#define MAP_LENGTH 100
#define MAP_HEIGHT 50

#define GROUND_LEVEL 2 /* Base ground thickness */
#define TREE_CHANCE 40 /* Chance out of 1000 that a tree spawns on a tile */
#define CLIFF_HEIGHT 5
#define GROUND_HEIGHT 5
#define SPAWN_RATE_VARIANCE 50

#define BLOCK_NAME_LENGTH 24
#define BLOCKS_AMOUNT 256
#define BLOCK_STATES 4

typedef struct block_data_s {
  char name[BLOCK_NAME_LENGTH];
  int hp;
  bool solid;
  bool transparent;
  int block[BLOCK_STATES];
  int dropped_item;
  int count;
  int block_type;
  int id;
} block_data_t;

typedef struct world_data_s {
  block_data_t block;
  int current_state;
  int hp;
  int id;
  int height_map;
} world_data_t;

int get_world_index(int x_pos, int y_pos, int z_pos);
void get_height(int * result, int x_pos, int y_pos);
void set_block(block_data_t block, int x_pos, int y_pos, int z_pos);
world_data_t get_block(int x_pos, int y_pos, int z_pos);
void set_block_state(int state, int x_pos, int y_pos, int z_pos);
int is_block_shaded(int x_pos, int y_pos, int z_pos);
void translate_block_def(char * def);
void load_block_properties(char * path);
block_data_t get_block_properties(int block);
void fill_map();
void place_trees(char height_map[MAP_WIDTH][MAP_LENGTH], int seed);
void rotate_grass();
void generate_hills(int seed);
int compare_blocks(block_data_t a, block_data_t b);
int is_block_mineable(block_data_t block);
int is_next_to_block(block_data_t block, int x_pos, int y_pos, int z_pos);
int is_next_to_workshop(int workshop_id, int x_pos, int y_pos, int z_pos);
#endif

