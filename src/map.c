#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <map.h>
#include <time.h>
#include <drawer.h>
#include <stdbool.h>
#include <player.h>
#include <camera.h>
#include <physics.h>
#include <map_defs.h>
#include <string.h>
#include <perlin.h>
#include <mjson.h>
#include <ticks.h>

data_map_t data_map;
world_data_t chunk_map[CHUNK_WIDTH * CHUNK_LENGTH * CHUNK_HEIGHT];
int air_temperature;

int changed_blocks_size = 1;
int changed_blocks_index = 0;
change_data_t * changed_blocks;

int map_seed = 0;

int get_map_seed(){
  return map_seed;
}

void set_map_seed(int seed){
  map_seed = seed;
}

int get_chunk_index(int x_pos, int y_pos, int z_pos){
  return x_pos + y_pos * CHUNK_WIDTH + z_pos * CHUNK_WIDTH * CHUNK_LENGTH;
}

void get_height(int * result, int x_pos, int y_pos){
  for (int z = 0 ; z < CHUNK_HEIGHT ; z++){
    if (get_block(x_pos, y_pos, z).block.id == 0){
      if (is_block_shaded(x_pos, y_pos, z-1)){
	continue;
      }
      *result = z-1;
      return;
    }
  }
  *result = 0;
}

void set_block(block_data_t block, int x_pos, int y_pos, int z_pos){
  int height;
  int chunk_index = get_chunk_index(x_pos, y_pos, z_pos);
  get_height(&height, x_pos, y_pos);
  chunk_map[chunk_index].block = block;
  chunk_map[chunk_index].current_state = 0;
  chunk_map[chunk_index].hp = block.hp;
  chunk_map[chunk_index].id = block.id;
  chunk_map[chunk_index].height_map = height;
  chunk_map[chunk_index].temperature = air_temperature;

  if (block.id == FIRE){
    chunk_map[chunk_index].temperature = get_block_properties(FIRE).ignition/2;
  }
}

world_data_t get_block(int x_pos, int y_pos, int z_pos){
  int chunk_index = get_chunk_index(x_pos, y_pos, z_pos);
  return chunk_map[chunk_index];
}

int is_block_shaded(int x_pos, int y_pos, int z_pos){
  if (get_block(x_pos, y_pos, z_pos+1).block.solid){
    return 0;
  }
  for (int z = z_pos+1 ; z < MAP_HEIGHT ; z++){
    if (get_block(x_pos, y_pos, z).block.solid){
      return 1;
    }
  }
  return 0;
}

void load_block_properties(char * path){
  int block_def_size = 1024;
  char * block_def_copy = malloc(block_def_size);
  FILE * def_file;
  int current_char = 0;
  
  if ((def_file = fopen(path, "r")) == NULL){
    fprintf(stderr, "Unable to load blocks defs\n");
    exit(-1);
  }

  // make a copy of the data, so that it can be tokenized
  do {
    char def_char = fgetc(def_file);

    if (current_char == block_def_size){
      block_def_size += 256;
      block_def_copy = realloc(block_def_copy, block_def_size);
    }
    
    if (feof(def_file)){
      break;
    } else {
      block_def_copy[current_char] = def_char;
      current_char++;
    }
  } while (true);

  const struct json_attr_t json_subobjects[] = {
    {"name", t_string, STRUCTOBJECT(block_data_t, name), .len = BLOCK_NAME_LENGTH},
    {"hp", t_integer, STRUCTOBJECT(block_data_t, hp)},
    {"solid", t_boolean, STRUCTOBJECT(block_data_t, solid)},
    {"transparent", t_boolean, STRUCTOBJECT(block_data_t, transparent)},
    {"texture", t_integer, STRUCTOBJECT(block_data_t, texture)},
    {"dropped_item", t_integer, STRUCTOBJECT(block_data_t, dropped_item)},
    {"count", t_integer, STRUCTOBJECT(block_data_t, count)},
    {"block_type", t_integer, STRUCTOBJECT(block_data_t, block_type)},
    {"output_id", t_integer, STRUCTOBJECT(block_data_t, output_id)},
    {"regen", t_boolean, STRUCTOBJECT(block_data_t, regen)},
    {"regen_ticks", t_integer, STRUCTOBJECT(block_data_t, regen_ticks)},
    {"hardness", t_integer, STRUCTOBJECT(block_data_t, hardness)},
    {"ignition", t_integer, STRUCTOBJECT(block_data_t, ignition)},
    {"extinguish_id", t_integer, STRUCTOBJECT(block_data_t, extinguish_id)},
    {"is_item", t_boolean, STRUCTOBJECT(block_data_t, is_item)},
    {"id", t_integer, STRUCTOBJECT(block_data_t, id)},
    {NULL},
  };

  const struct json_attr_t json_subobject_list[] = {
    {"class", t_check,.dflt.check = "BLOCKS"},
    {"blocks", t_array, STRUCTARRAY(data_map.data,
				  json_subobjects,
				  &data_map.size)},
    {NULL},
  };

  memset(&data_map, '\0', sizeof(data_map));
  json_read_object(block_def_copy, json_subobject_list, NULL);

  printf("Successfully loaded block defs\n");
  
  free(block_def_copy);
  fclose(def_file);
}

block_data_t get_block_properties(int block){
  return data_map.data[block];
}

int get_air_temperature(){
  return air_temperature;
}

void set_air_temperature(int value){
  air_temperature = value;
}

void set_temperature(int temperature, int x, int y, int z){
  int chunk_index = get_chunk_index(x, y, z);
  chunk_map[chunk_index].temperature = temperature;
}

void init_map(int seed){
  set_map_seed(seed);
  srand(seed);
  set_air_temperature(BASE_TEMPERATURE + rand() % 20);
  for (int index = 0 ; index < CHUNK_WIDTH * CHUNK_LENGTH ; index++){
    int x = index % CHUNK_WIDTH;
    int y = index / CHUNK_LENGTH;
    for (int z = 0 ; z < CHUNK_HEIGHT ; z++){
      set_temperature(get_air_temperature(), x, y, z);
    }
  }
}

/* Initialize every tile in map to empty block */
void fill_map(){
  for (int index = 0 ; index < CHUNK_WIDTH * CHUNK_LENGTH ; index++){
    int x = index % CHUNK_WIDTH;
    int y = index / CHUNK_LENGTH;
    for (int z = 0 ; z < CHUNK_HEIGHT ; z++){
      if (get_block(x, y, z).block.id == EMPTY){
	continue;
      }
      set_block(get_block_properties(EMPTY), x, y, z);
    }
  }
}

void place_foliage(int x_off, int y_off, char height_map[CHUNK_WIDTH][CHUNK_LENGTH]){
  for (int index = 0 ; index < CHUNK_WIDTH * CHUNK_LENGTH ; index++){
    int x = index % CHUNK_WIDTH;
    int y = index / CHUNK_LENGTH;

    if (get_block(x, y, height_map[x][y]).block.id != GRASS){
      continue;
    }

    float noise = pnoise2d(x+x_off, y+y_off, 0.25, 10, 1, 1,get_map_seed()) + 1.5;
    
    if (noise > 1.8 && noise < 2){
      int trunk_height;
      for (trunk_height = 1 ; trunk_height < TREE_HEIGHT ; trunk_height++){
	set_block(get_block_properties(TREE_TRUNK), x, y, height_map[x][y]+trunk_height);
      }
      set_block(get_block_properties(TREE_LEAVES), x, y, height_map[x][y]+TREE_HEIGHT);
      continue;
    }
    
    if (noise >= 2.1 && noise < 2.2){
      /* Worlds either have red or blue berries */
      if (get_map_seed() % 2){
	set_block(get_block_properties(RED_BERRY_BUSH), x, y, height_map[x][y]+1);
      } else {
	set_block(get_block_properties(BLUE_BERRY_BUSH), x, y, height_map[x][y]+1);
      }
      continue;
    }
  }
}

void place_items(int x_off, int y_off, char height_map[CHUNK_WIDTH][CHUNK_LENGTH]){
  for (int index = 0 ; index < CHUNK_WIDTH * CHUNK_LENGTH ; index++){
    int x = index % CHUNK_WIDTH;
    int y = index / CHUNK_LENGTH;

    if (get_block(x, y, height_map[x][y]).block.id != GRASS ||
	get_block(x, y, height_map[x][y]+1).block.id != EMPTY){
      continue;
    }

    float noise = pnoise2d(x+x_off, y+y_off, 0.25, 10, 1, 1, get_map_seed()) + 1;
    
    if (noise > 2 &&
	height_map[x][y] >= CLIFF_HEIGHT-1){
      set_block(get_block_properties(ROCKS), x, y, height_map[x][y]+1);
      continue;
    }
    if (noise > 1.9 &&
	is_next_to_block(get_block_properties(TREE_TRUNK), x, y, height_map[x][y]+1)){
      set_block(get_block_properties(BRANCH), x, y, height_map[x][y]+1);
    }
  }
}

void generate_hills(int x_off, int y_off){
  fill_map();
  /* Step 1 */
  char height_map[CHUNK_WIDTH][CHUNK_LENGTH];
  const int end_index = CHUNK_WIDTH * CHUNK_LENGTH;

  /* height map generation with averaging */
  for (int index = 0 ; index < end_index ; index++){
    int x = index % CHUNK_WIDTH;
    int y = index / CHUNK_LENGTH;
    height_map[x][y] = 0;
    height_map[x][y] += pnoise2d((x+x_off)-1, (y+y_off), 1, 10, 0.25, 3.5, get_map_seed());
    height_map[x][y] += pnoise2d((x+x_off), (y+y_off)-1, 1, 10, 0.25, 3.5, get_map_seed());
    height_map[x][y] += pnoise2d((x+x_off)+1, (y+y_off), 1, 10, 0.25, 3.5, get_map_seed());
    height_map[x][y] += pnoise2d((x+x_off), (y+y_off)+1, 1, 10, 0.25, 3.5, get_map_seed());
    height_map[x][y] /= 4;
    if (height_map[x][y] < 1){
      height_map[x][y] = 1;
    }
  }

  /* Water placement */
  for (int index = 0 ; index < end_index ; index++){
    int x = index % CHUNK_WIDTH;
    int y = index / CHUNK_LENGTH;
    height_map[x][y] += 1;
    if (height_map[x][y] == 2){
      float noise = pnoise2d(x+x_off, y+y_off, 1, 10, 0.1, 2, get_map_seed() * 5);
      if (noise < -1.75){
	height_map[x][y] = 1;
        set_block(get_block_properties(WATER), x, y, 2);
	set_block(get_block_properties(SAND), x, y, 1);
      }
    }
  }

  /* Height map usage */
  for (int index = 0 ; index < end_index ; index++){
    int x = index % CHUNK_WIDTH;
    int y = index / CHUNK_LENGTH;

    set_block(get_block_properties(NOKIUM), x, y, 0);
    
    if (height_map[x][y] == 1){
      continue;
    }
    
    set_block(get_block_properties(GRASS), x, y, height_map[x][y]);

    for (int z = height_map[x][y]-1 ; z > 0 ; z--){
      if (height_map[x][y] < CLIFF_HEIGHT){
	set_block(get_block_properties(DIRT), x, y, z);
	continue;
      }
      
      set_block(get_block_properties(STONE), x, y, z);

      /* cave generation */
      float cave_noise = pnoise3d(x+x_off, y+y_off, z, 0.75, 10, 0.5, 1, get_map_seed());
      if (cave_noise > 1 && cave_noise < 6 && z > 1){
	set_block(get_block_properties(EMPTY), x, y, z);
      }
      
    }
  }

  place_foliage(x_off, y_off, height_map);
  place_items(x_off, y_off, height_map);

  /* changed block placement */
  for (int change = 0 ; change < changed_blocks_index ; change++){
    for (int index = 0 ; index < end_index ; index++){
      int x = (index % CHUNK_WIDTH) - 1;
      int y = (index / CHUNK_LENGTH) - 1;
      
      int change_x = get_changed_blocks(change).x;
      int change_y = get_changed_blocks(change).y;
      int change_z = get_changed_blocks(change).z;
      int regen = get_changed_blocks(change).data.block.regen;
      int regen_tick = get_changed_blocks(change).data.block.regen_ticks;
      block_data_t regen_block = get_changed_blocks(change).prev_data.block;
      
      switch(get_changed_blocks(change).data.block.id){
        case FIRE:
	  regen_block = get_block_properties(get_changed_blocks(change).prev_data.block.extinguish_id);
	  break;
        case STEAM:
	  regen_block = get_block_properties(WATER);
	  break;
      }

      if (get_block(x, y, change_z).temperature >= get_block(x, y, change_z).block.ignition &&
	  get_block(x, y, change_z).block.ignition > 237){
        set_block(get_block_properties(FIRE), x, y, change_z);
      }
      
      int x_pos = x + x_off;
      int y_pos = y + y_off;

      if (x_pos != change_x || y_pos != change_y){
	continue;
      }

      if (regen && SDL_GetTicks() - get_changed_blocks(change).tick_changed >= regen_tick){
	world_data_t prev_data = get_changed_blocks(change).data;
	set_block(regen_block, x, y, change_z);
	set_changed_blocks(1,
			   prev_data,
			   get_block(x, y, change_z),
			   x,
			   y,
			   change_z);
	continue;
      }
	
      set_block(get_changed_blocks(change).data.block, x, y, change_z);

    }
  }
}

int compare_blocks(block_data_t a, block_data_t b){
  int equality = 0;
  equality += a.hp == b.hp;
  equality += a.solid == b.solid;
  equality += a.transparent == b.transparent;
  equality += a.texture == b.texture;
  equality += a.dropped_item == b.dropped_item;
  equality += a.count == b.count;
  equality += a.block_type == b.block_type;
  return equality == 7;
}

int is_block_mineable(block_data_t block){
  return block.hp >= 0;
}

int is_next_to_block(block_data_t block, int x_pos, int y_pos, int z_pos){
  if (get_block(x_pos-1, y_pos, z_pos).block.id == block.id){
    return 1;
  }
  if (get_block(x_pos, y_pos-1, z_pos).block.id == block.id){
    return 1;
  }
  if (get_block(x_pos+1, y_pos, z_pos).block.id == block.id){
    return 1;
  }
  if (get_block(x_pos, y_pos+1, z_pos).block.id == block.id){
    return 1;
  }
  return 0;
}

int is_next_to_workshop(int workshop_id, int x_pos, int y_pos, int z_pos){
  return is_next_to_block(get_block_properties(workshop_id),x_pos,y_pos,z_pos);
}

int get_changed_blocks_size(){
  return changed_blocks_size;
}

int get_changed_blocks_index(){
  return changed_blocks_index;
}

void set_changed_blocks(int use_block, world_data_t prev_data, world_data_t data, int x_pos, int y_pos, int z_pos){
  int index = changed_blocks_index;
  int replace = 0;
  for (int change = 0 ; change < changed_blocks_size ; change++){
    if (changed_blocks[change].x == x_pos &&
	changed_blocks[change].y == y_pos &&
	changed_blocks[change].z == z_pos){
      index = change;
      replace = 1;
      break;
    }
  }
  
  if (!replace){
    changed_blocks_size++;
    reallocate_changed_blocks(1);
    changed_blocks_index++;
  }

  world_data_t new_data = data;
  
  if (!use_block){
    new_data = prev_data;
  }
  
  changed_blocks[index].prev_data = prev_data;
  changed_blocks[index].data = new_data;
  changed_blocks[index].x = x_pos;
  changed_blocks[index].y = y_pos;
  changed_blocks[index].z = z_pos;
  changed_blocks[index].data.temperature = data.temperature;
  changed_blocks[index].tick_changed = SDL_GetTicks();
}

change_data_t get_changed_blocks(int index){
  return changed_blocks[index];
}

void allocate_changed_blocks(){
  changed_blocks = malloc(changed_blocks_size * sizeof(change_data_t));
}

void reallocate_changed_blocks(int size_offset){
  changed_blocks = realloc(changed_blocks, changed_blocks_size * sizeof(change_data_t));
}

void free_changed_blocks(){
	if (changed_blocks == NULL){
		return;
	}
	free(changed_blocks);
}
