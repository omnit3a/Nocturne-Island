#ifndef MAP_H_
#define MAP_H_
#include <SDL2/SDL.h>
#include <stdbool.h>
#include <camera.h>

#define BLOCK_DATA_PATH "assets/defs/blocks.json"

#define MAP_WIDTH 100
#define MAP_LENGTH 100
#define MAP_HEIGHT 50
#define CHUNK_WIDTH 14
#define CHUNK_LENGTH 14
#define CHUNK_HEIGHT MAP_HEIGHT

#define CLIFF_HEIGHT 6
#define TREE_HEIGHT 3

#define BLOCK_NAME_LENGTH 24
#define BLOCKS_AMOUNT 256
#define BLOCK_STATES 4

#define BASE_TEMPERATURE 237

typedef struct block_data_s {
  char name[BLOCK_NAME_LENGTH];
  int hp;
  bool solid;
  bool transparent;
  int texture;
  int dropped_item;
  int count;
  int block_type;
  int output_id;
  int regen;
  int regen_ticks;
  int hardness;
  int ignition;
  int extinguish_id;
  int id;
} block_data_t;

typedef struct data_map_s {
  int size;
  block_data_t data[BLOCKS_AMOUNT];
} data_map_t;

typedef struct world_data_s {
  block_data_t block;
  int current_state;
  int hp;
  int id;
  int height_map;
  int temperature;
} world_data_t;

typedef struct change_data_s {
  world_data_t prev_data;
  world_data_t data;
  int x;
  int y;
  int z;
  int tick_changed;
} change_data_t;

int get_map_seed();
void set_map_seed(int seed);
int get_chunk_index(int x_pos, int y_pos, int z_pos);
void get_height(int * result, int x_pos, int y_pos);
void set_block(block_data_t block, int x_pos, int y_pos, int z_pos);
world_data_t get_block(int x_pos, int y_pos, int z_pos);
int is_block_shaded(int x_pos, int y_pos, int z_pos);
void load_block_properties(char * path);
block_data_t get_block_properties(int block);
int get_air_temperature();
void set_air_temperature(int value);
void set_temperature(int temperature, int x, int y, int z);

void init_map(int seed);
void fill_map();
void place_foliage(int x_off, int y_off, char height_map[CHUNK_WIDTH][CHUNK_LENGTH]);
void place_items(int x_off, int y_off, char height_map[CHUNK_WIDTH][CHUNK_LENGTH]);
void generate_hills(int x_off, int y_off);

int compare_blocks(block_data_t a, block_data_t b);
int is_block_mineable(block_data_t block);
int is_next_to_block(block_data_t block, int x_pos, int y_pos, int z_pos);
int is_next_to_workshop(int workshop_id, int x_pos, int y_pos, int z_pos);

int get_changed_blocks_size();
int get_changed_blocks_index();
void set_changed_blocks(world_data_t prev_data, world_data_t data, int x_pos, int y_pos, int z_pos);
change_data_t get_changed_blocks(int index);
void allocate_changed_blocks();
void reallocate_changed_blocks(int size_offset);
void free_changed_blocks();

#endif

