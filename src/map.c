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

block_data_t data_map[BLOCKS_AMOUNT];
world_data_t chunk_map[CHUNK_WIDTH * CHUNK_LENGTH * CHUNK_HEIGHT];

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
    if (!get_block(x_pos, y_pos, z).block.solid){
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
}

world_data_t get_block(int x_pos, int y_pos, int z_pos){
  int chunk_index = get_chunk_index(x_pos, y_pos, z_pos);
  return chunk_map[chunk_index];
}

void set_block_state(int state, int x_pos, int y_pos, int z_pos){
  int chunk_index = get_chunk_index(x_pos, y_pos, z_pos);
  chunk_map[chunk_index].current_state = state;
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

void translate_block_def(char * def){
  int values[11] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
  char * token;
  char * delim = " ";
  int field = 0;
  char name[BLOCK_NAME_LENGTH];
  
  token = strtok(def, delim);
  
  while (token != NULL){
    if (field == 0){
      strcpy(name, token);
    } else {
      values[field-1] = atoi(token);
    }
    field++;
    token = strtok(NULL, delim);
  }

  int line = values[10];
  strcpy(data_map[line].name, name);
  data_map[line].hp = values[0];
  data_map[line].solid = values[1];
  data_map[line].transparent = values[2];
  for (int state = 0 ; state < BLOCK_STATES ; state++){
    data_map[line].block[state] = values[3+state];
  }
  data_map[line].dropped_item = values[7];
  data_map[line].count = values[8];
  data_map[line].block_type = values[9];
  data_map[line].id = values[10];
}

void load_block_properties(char * path){
  int block_def_size = 1024;
  char * block_def_copy = malloc(block_def_size);
  char ** block_def_lines = malloc(256 * BLOCKS_AMOUNT);
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

  char * def_line;
  char delim[2] = "\n";
  int current_line = 0;

  // tokenize the data
  def_line = strtok(block_def_copy, delim);

  while (def_line != NULL){
    block_def_lines[current_line++] = def_line;
    def_line = strtok(NULL, delim);
  }

  block_def_lines[current_line] = NULL;

  current_line = 0;
  while (block_def_lines[current_line] != NULL){
    translate_block_def(block_def_lines[current_line]);
    current_line++;
  }

  printf("Successfully loaded block defs\n");
  
  free(block_def_lines);
  free(block_def_copy);
  fclose(def_file);
}

block_data_t get_block_properties(int block){
  return data_map[block];
}

/* Initialize every tile in map to empty block */
void fill_map(){
  for (int index = 0 ; index < CHUNK_WIDTH * CHUNK_LENGTH ; index++){
    int x = index % CHUNK_WIDTH;
    int y = index / CHUNK_LENGTH;
    for (int z = 0 ; z < CHUNK_HEIGHT ; z++){
      set_block(get_block_properties(EMPTY), x, y, z);
    }
  }
}

void place_trees(int x_off, int y_off, char height_map[CHUNK_WIDTH][CHUNK_LENGTH]){
  for (int index = 0 ; index < CHUNK_WIDTH * CHUNK_LENGTH ; index++){
    int height;
    int x = index % CHUNK_WIDTH;
    int y = index / CHUNK_LENGTH;
    get_height(&height, x, y);
    float noise = pnoise2d(x+x_off, y+y_off, 0.25, 10, get_map_seed()) + 1;
    if (noise > 1.8){
      set_block(get_block_properties(TREE_BOTTOM), x, y, height+1);
      set_block(get_block_properties(TREE_LEAVES), x, y, height+2);
    }
  }
}

void generate_hills(int x_off, int y_off){
  fill_map();
  /* Step 1 */
  char height_map[CHUNK_WIDTH][CHUNK_LENGTH];
  for (int index = 0 ; index < CHUNK_WIDTH * CHUNK_LENGTH ; index++){
    int x = index % CHUNK_WIDTH;
    int y = index / CHUNK_LENGTH;
    height_map[x][y] = 0;
  }

  for (int index = 0 ; index < CHUNK_WIDTH*CHUNK_LENGTH ; index++){
    int x = index % CHUNK_WIDTH;
    int y = index / CHUNK_LENGTH;
    height_map[x][y] += pnoise2d((x+x_off)-1, (y+y_off), 1, 10, get_map_seed()) * 5;
    height_map[x][y] += pnoise2d((x+x_off), (y+y_off)-1, 1, 10, get_map_seed()) * 5;
    height_map[x][y] += pnoise2d((x+x_off)+1, (y+y_off), 1, 10, get_map_seed()) * 5;
    height_map[x][y] += pnoise2d((x+x_off), (y+y_off)+1, 1, 10, get_map_seed()) * 5;
    height_map[x][y] /= 4;
    if (height_map[x][y] < 1){
      height_map[x][y] = 1;
    }
  }
  
  for (int index = 0 ; index < CHUNK_WIDTH * CHUNK_LENGTH ; index++){
    int x = index % CHUNK_WIDTH;
    int y = index / CHUNK_LENGTH;
    set_block(get_block_properties(GRASS), x, y, height_map[x][y]);
    set_block_state(rand() % 3, x, y, height_map[x][y]);
    for (int z = height_map[x][y]-1 ; z > 0 ; z--){
      if (height_map[x][y] >= CLIFF_HEIGHT){
	set_block(get_block_properties(STONE), x, y, z);
      } else {
	set_block(get_block_properties(DIRT), x, y, z);
      }
    }
    set_block(get_block_properties(NOKIUM), x, y, 0);
  }
  
  place_trees(x_off, y_off, height_map);
  
  for (int change = 0 ; change < changed_blocks_index ; change++){
    for (int index = 0 ; index < CHUNK_WIDTH * CHUNK_LENGTH ; index++){
      int x = index % CHUNK_WIDTH;
      int y = index / CHUNK_LENGTH;
      
      int change_x = get_changed_blocks(change).x;
      int change_y = get_changed_blocks(change).y;
      int change_z = get_changed_blocks(change).z;
      
      int x_pos = x + x_off;
      int y_pos = y + y_off;
      
      if (x_pos == change_x && y_pos == change_y){
	set_block(get_changed_blocks(change).data.block, x, y, change_z);
      }
    }
  }
}

int compare_blocks(block_data_t a, block_data_t b){
  int equality = 0;
  equality += a.hp == b.hp;
  equality += a.solid == b.solid;
  equality += a.transparent == b.transparent;
  for (int state = 0 ; state < BLOCK_STATES ; state++){
    equality += a.block[state] == b.block[state];
  } 
  equality += a.dropped_item == b.dropped_item;
  equality += a.count == b.count;
  equality += a.block_type == b.block_type;
  return equality == 10;
}

int is_block_mineable(block_data_t block){
  return block.hp >= 0;
}

int is_next_to_block(block_data_t block, int x_pos, int y_pos, int z_pos){
  if (compare_blocks(get_block(x_pos-1, y_pos, z_pos).block, block)){
    return 1;
  }
  if (compare_blocks(get_block(x_pos, y_pos-1, z_pos).block, block)){
    return 1;
  }
  if (compare_blocks(get_block(x_pos+1, y_pos, z_pos).block, block)){
    return 1;
  }
  if (compare_blocks(get_block(x_pos, y_pos+1, z_pos).block, block)){
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

void set_changed_blocks(world_data_t data, int x_pos, int y_pos, int z_pos){
  changed_blocks_size++;
  reallocate_changed_blocks(1);
  changed_blocks[changed_blocks_index].data = data;
  changed_blocks[changed_blocks_index].x = x_pos;
  changed_blocks[changed_blocks_index].y = y_pos;
  changed_blocks[changed_blocks_index].z = z_pos;
  changed_blocks_index++;
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
  free(changed_blocks);
}
