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

world_data_t game_map[MAP_WIDTH][MAP_LENGTH][MAP_HEIGHT];
block_data_t data_map[BLOCKS_AMOUNT];

void set_block(block_data_t block, int x_pos, int y_pos, int z_pos){
  game_map[x_pos][y_pos][z_pos].block = block;
  game_map[x_pos][y_pos][z_pos].current_state = 0;
  game_map[x_pos][y_pos][z_pos].hp = block.hp;
  game_map[x_pos][y_pos][z_pos].id = block.id;
}

world_data_t get_block(int x_pos, int y_pos, int z_pos){
  return game_map[x_pos][y_pos][z_pos];
}

void set_block_state(int state, int x_pos, int y_pos, int z_pos){
  game_map[x_pos][y_pos][z_pos].current_state = state;
}

int is_block_shaded(int x_pos, int y_pos, int z_pos){
  if (!get_block(x_pos, y_pos, z_pos+1).block.transparent){
    return 0;
  }
  for (int z = z_pos+1 ; z < MAP_HEIGHT ; z++){
    if (!get_block(x_pos, y_pos, z).block.transparent){
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
  for (int x = 0 ; x < MAP_WIDTH ; x++){
    for (int y = 0 ; y < MAP_LENGTH ; y++){
      for (int z = 0 ; z < MAP_HEIGHT ; z++){
	set_block(get_block_properties(EMPTY), x, y, z);
      }
    }
  }
}

/* Trees have a higher probability of spawning on mountains, as there is more
 * light.
 * When a lighting system is implemented, make it so that trees are more likely
 * to spawn in well lit areas.
 */
void place_trees(char height_map[MAP_WIDTH][MAP_LENGTH], int seed){
  srand(seed);
  int offset = 1;
  for (int x = 0 ; x < MAP_WIDTH ; x++){
    for (int y = 0 ; y < MAP_LENGTH ; y++){
      offset = 1;
      if (height_map[x][y] < MAP_HEIGHT){
	if (height_map[x][y] == CLIFF_HEIGHT){
	  offset = 30;
	}
	offset += rand() % SPAWN_RATE_VARIANCE;
        if ((rand() % 1000) <= TREE_CHANCE+offset){
	  set_block(get_block_properties(TREE_BOTTOM), x, y, height_map[x][y]+1);
	  set_block(get_block_properties(TREE_LEAVES), x, y, height_map[x][y]+2);
	}
      }
    }
  }
}

void rotate_grass(){
  for (int x = 0 ; x < MAP_WIDTH ; x++){
    for (int y = 0 ; y < MAP_LENGTH ; y++){
      for (int z = 0 ; z < MAP_HEIGHT ; z++){
	if (get_block(x, y, z).id == GRASS){
	  game_map[x][y][z].current_state = rand() % 3;
	}
      }
    }
  }
}


/* World Generation Steps:
 * Step 1:
 *   Place random high points
 * Step 2:
 *   Make cliffs/mountains from those high points
 * Step 3:
 *   Make all non-cliffs/mountains into randomly rough terrain
 * Step 4:
 *   Average the heights of the non-cliffs/mountains
 * Step 5:
 *   Place NOKIUM (unbreakable block) at bottom of world
 * Step 6:
 *   Place trees
 * Step 7:
 *   Place caves
 * Step 8:
 *   Generate block hp map
 */
void generate_hills(int seed){
  fill_map();
  srand(seed);
  /* Step 1 */
  int isMountain = 0;
  int prevIsMountain = 0;
  char temp_height_map[MAP_WIDTH][MAP_LENGTH];
  char height_map[MAP_WIDTH][MAP_LENGTH];
  for (int x = 0 ; x < MAP_WIDTH ; x++){
    for (int y = 0 ; y < MAP_LENGTH ; y++){
      height_map[x][y] = 0;
      if (prevIsMountain){
        isMountain = ((rand() % 100) < 85);
      } else {
        isMountain = ((rand() % 100) < 5);
      }
      if (isMountain){
        height_map[x][y] = CLIFF_HEIGHT;
      }
      prevIsMountain = isMountain;
    }
  }

  /* Step 2 */
  for (int r = 0 ; r < 100 ; r++){
    for (int x = 0 ; x < MAP_WIDTH ; x++){
      for (int y = 0 ; y < MAP_LENGTH ; y++){
	temp_height_map[x][y] = height_map[x][y];
	if (r < 75){
          if (height_map[x-1][y] > 0 && height_map[x+1][y] > 0){
	    temp_height_map[x][y] = CLIFF_HEIGHT;
	  }
	  if (height_map[x][y-1] == 0 && height_map[x][y+1] == 0){
	    temp_height_map[x][y] = CLIFF_HEIGHT;
	  }
	}
 	if ((height_map[x-1][y] == 0 && height_map[x+1][y] == 0) || (height_map[x][y-1] == 0 && height_map[x][y+1] == 0)){
	  temp_height_map[x][y] = 0;
        }
      }
    }
  }

  /* Step 3 */
  int hillChance = 1;
  for (int x = 0 ; x < MAP_WIDTH ; x++){
    for (int y = 0 ; y < MAP_LENGTH ; y++){
      if (temp_height_map[x][y] == 0){
	if (hillChance == 1){
	  hillChance = (((rand() % 100) < 75)%2);
	  if (hillChance){
	    hillChance = 1;
	  } else {
	    hillChance = 2;
	  }
	} else if (hillChance == 2){
	  hillChance = (((rand() % 100) < 75)%2);
	  if (hillChance){
	    hillChance = 2;
	  } else {
	    hillChance = 3;
	  }
	} else if (hillChance == 3){
	  hillChance = (((rand() % 100) < 75)%2);
	  if (hillChance){
	    hillChance = 3;
	  } else {
	    hillChance = 1;
	  }
	}
	temp_height_map[x][y] = hillChance;
      }
    }
  }

  for (int x = 0 ; x < MAP_WIDTH ; x++){
    for (int y = 0 ; y < MAP_LENGTH ; y++){
      height_map[x][y] = temp_height_map[x][y];
    }
  }

  /* Step 4 */
  int surroundVals[4] = {0};
  int average = 0;
  for (int r = 0 ; r < 3 ; r++){
    for (int x = 0 ; x < MAP_WIDTH ; x++){
      for (int y = 0 ; y < MAP_LENGTH ; y++){
	if (x > 0) {
          surroundVals[0] = height_map[x-1][y];
	}
	if (x < MAP_WIDTH-1){
          surroundVals[1] = height_map[x+1][y];
	}
	if (y > 0){
          surroundVals[2] = height_map[x][y-1];
	}
	if (y < MAP_LENGTH-1){
          surroundVals[3] = height_map[x][y+1];
	}
        for (int a = 0 ; a < 4 ; a++){
          average += surroundVals[a];
        }
        average = average / 4;
        temp_height_map[x][y] = average;
      }
    }
    for (int x = 0 ; x < MAP_WIDTH ; x++){
      for (int y = 0 ; y < MAP_LENGTH ; y++){
        if (height_map[x][y] < GROUND_HEIGHT){
	  height_map[x][y] = temp_height_map[x][y];
	}
      }
    }
  }

  /* Step 5 */
  for (int x = 0 ; x < MAP_WIDTH ; x++){
    for (int y = 0 ; y < MAP_LENGTH ; y++){
      set_block(get_block_properties(GRASS), x, y, height_map[x][y]);
      for (int z = height_map[x][y]-1 ; z >= 0 ; z--){
	if (height_map[x][y] < GROUND_HEIGHT){
	  set_block(get_block_properties(DIRT), x, y, z);
	} else {
	  set_block(get_block_properties(STONE), x, y, z);
	}
      }
    }
  }

  /* Step 6 */
  place_trees(height_map, seed);

  rotate_grass();
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
