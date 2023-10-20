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

char world_map[MAP_WIDTH][MAP_LENGTH][MAP_HEIGHT];
block_data_t data_map[BLOCKS_AMOUNT];
int block_hp_map[MAP_WIDTH][MAP_LENGTH][MAP_HEIGHT];

void translateBlockDef(char * def, int line){
  int values[7] = {0, 0, 0, 0, 0, 0, 0};
  char * token;
  char * delim = " ";
  int field = 0;
  
  token = strtok(def, delim);
  
  while (token != NULL){
    values[field++] = atoi(token);
    token = strtok(NULL, delim);
  }
  
  data_map[line].hp = values[0];
  data_map[line].solid = values[1];
  data_map[line].transparent = values[2];
  data_map[line].block = values[3];
  data_map[line].dropped_item = values[4];
  data_map[line].count = values[5];
  data_map[line].block_type = values[6];
  
}

void loadBlockProperties(char * path, block_data_t * data){
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
    translateBlockDef(block_def_lines[current_line], current_line);
    current_line++;
  }

  printf("Successfully loaded block defs\n");
  
  free(block_def_lines);
  free(block_def_copy);
  fclose(def_file);
}

block_data_t getBlockProperties(char map[MAP_WIDTH][MAP_LENGTH][MAP_HEIGHT], int xPos, int yPos, int zPos){
  return data_map[(int)map[xPos][yPos][zPos]];
}

/* Initialize every tile in map to empty block */
void fillMap(char map[MAP_WIDTH][MAP_LENGTH][MAP_HEIGHT]){
  for (int i = 0 ; i < MAP_HEIGHT ; i++){
    for (int j = 0 ; j < MAP_WIDTH ; j++){
      for (int n = 0 ; n < MAP_LENGTH ; n++){
	map[j][n][i] = 0;
	block_hp_map[j][n][i] = 0;
      }
    }
  }
}

/* Trees have a higher probability of spawning on mountains, as there is more
 * light.
 * When a lighting system is implemented, make it so that trees are more likely
 * to spawn in well lit areas.
 */
void placeTrees(char map[MAP_WIDTH][MAP_LENGTH][MAP_HEIGHT],char height_map[MAP_WIDTH][MAP_LENGTH], int seed){
  srand(seed);
  int offset = 1;
  for (int i = 0 ; i < MAP_WIDTH ; i++){
    for (int j = 0 ; j < MAP_LENGTH ; j++){
      offset = 1;
      if (height_map[i][j] < MAP_HEIGHT){
	if (height_map[i][j] == CLIFF_HEIGHT){
	  offset = 30;
	}
	offset += rand() % SPAWN_RATE_VARIANCE;
        if ((rand() % 1000) <= TREE_CHANCE+offset){
	  map[i][j][(int)height_map[i][j]+1] = TREE_BOTTOM;
		if ((rand() % 3) > 0){
	  	map[i][j][(int)height_map[i][j]+2] = PINE_TREE_LEAVES;
		} else {
			map[i][j][(int)height_map[i][j]+2] = OAK_TREE_LEAVES;
		}
	}
      }
    }
  }
}

/* Place Iron and Coal ore inside of mountains */
void placeOres(char map[MAP_WIDTH][MAP_LENGTH][MAP_HEIGHT], char height_map[MAP_WIDTH][MAP_LENGTH], int seed){
  srand(seed);
  int zPos;
  int offset = 0;
  for (int i = 0 ; i < MAP_WIDTH ; i++){
    for (int j = 0 ; j < MAP_LENGTH ; j++){
      if (height_map[i][j] > GROUND_HEIGHT){
	zPos = (rand() % (CLIFF_HEIGHT-3))+GROUND_HEIGHT;
	if (getBlockProperties(map, i, j, zPos+1).block == GRASS &&
	    getBlockProperties(map, i, j, zPos-1).solid){

	  offset = rand() % SPAWN_RATE_VARIANCE;
	  
	  /* Select either iron or coal ore */
	  if (rand() % 2){
	    if ((rand() % 1000) <= IRON_CHANCE+offset){
	      map[i][j][zPos] = IRON_ORE;
	    }
	  } else {
	    if ((rand() % 1000) <= COAL_CHANCE+offset){
	      map[i][j][zPos] = COAL_ORE;
	      continue;
	    }
	  }
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
void generateHills(char map[MAP_WIDTH][MAP_LENGTH][MAP_HEIGHT], int seed){
  fillMap(map);
  srand(seed);
  /* Step 1 */
  int isMountain = 0;
  int prevIsMountain;
  char temp_height_map[MAP_WIDTH][MAP_LENGTH];
  char height_map[MAP_WIDTH][MAP_LENGTH];
  for (int i = 0 ; i < MAP_WIDTH ; i++){
    for (int j = 0 ; j < MAP_LENGTH ; j++){
      height_map[i][j] = 0;
      if (prevIsMountain){
        isMountain = ((rand() % 100) < 85);
      } else {
        isMountain = ((rand() % 100) < 5);
      }
      if (isMountain){
        height_map[i][j] = CLIFF_HEIGHT;
      }
      prevIsMountain = isMountain;
    }
  }

  /* Step 2 */
  for (int r = 0 ; r < 100 ; r++){
    for (int i = 0 ; i < MAP_WIDTH ; i++){
      for (int j = 0 ; j < MAP_LENGTH ; j++){
	temp_height_map[i][j] = height_map[i][j];
	if (r < 75){
          if (height_map[i-1][j] > 0 && height_map[i+1][j] > 0){
	    temp_height_map[i][j] = CLIFF_HEIGHT;
	  }
	  if (height_map[i][j-1] == 0 && height_map[i][j+1] == 0){
	    temp_height_map[i][j] = CLIFF_HEIGHT;
	  }
	}
 	if ((height_map[i-1][j] == 0 && height_map[i+1][j] == 0) || (height_map[i][j-1] == 0 && height_map[i][j+1] == 0)){
	  temp_height_map[i][j] = 0;
        }
      }
    }
  }

  /* Step 3 */
  int hillChance = 1;
  for (int i = 0 ; i < MAP_WIDTH ; i++){
    for (int j = 0 ; j < MAP_LENGTH ; j++){
      if (temp_height_map[i][j] == 0){
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
	temp_height_map[i][j] = hillChance;
      }
    }
  }

  for (int i = 0 ; i < MAP_WIDTH ; i++){
    for (int j = 0 ; j < MAP_LENGTH ; j++){
      height_map[i][j] = temp_height_map[i][j];
    }
  }

  /* Step 4 */
  int surroundVals[4];
  int average;
  for (int r = 0 ; r < 3 ; r++){
    for (int i = 0 ; i < MAP_WIDTH ; i++){
      for (int j = 0 ; j < MAP_LENGTH ; j++){
	if (i > 0) {
          surroundVals[0] = height_map[i-1][j];
	}
	if (i < MAP_WIDTH-1){
          surroundVals[1] = height_map[i+1][j];
	}
	if (j > 0){
          surroundVals[2] = height_map[i][j-1];
	}
	if (j < MAP_LENGTH-1){
          surroundVals[3] = height_map[i][j+1];
	}
        for (int a = 0 ; a < 4 ; a++){
          average += surroundVals[a];
        }
        average = average / 4;
        temp_height_map[i][j] = average;
      }
    }
    for (int i = 0 ; i < MAP_WIDTH ; i++){
      for (int j = 0 ; j < MAP_LENGTH ; j++){
        if (height_map[i][j] < GROUND_HEIGHT){
	  height_map[i][j] = temp_height_map[i][j];
	}
      }
    }
  }

  /* Step 5 */
  for (int i = 0 ; i < MAP_WIDTH ; i++){
    for (int j = 0 ; j < MAP_LENGTH ; j++){
      map[i][j][(int)height_map[i][j]] = GRASS;
      for (int n = height_map[i][j]-1 ; n > 0 ; n--){
        map[i][j][n] = STONE;
      }
      map[i][j][0] = NOKIUM;
    }
  }

  /* Step 6 */
  placeTrees(map, height_map, seed);
  placeOres(map, height_map, seed);

  /* Step 7 */
  /* Probably too many caves */
  /*
  for (int i = 0 ; i < 400 ; i++){
    generateCave(map, seed, 500, 20);
  }
  */

  /* Step 8 */
  /* Generate block_hp_map */
  for (int i = 0 ; i < MAP_WIDTH ; i++){
    for (int j = 0 ; j < MAP_LENGTH ; j++){
      for (int n = 0 ; n < MAP_HEIGHT ; n++){
	block_hp_map[i][j][n] = getBlockProperties(map, i, j, n).hp;
      }
    }
  }
}

/* Cull blocks that are surrounded on top, the left and the right */
void cullHiddenBlocks(char dest_map[MAP_WIDTH][MAP_LENGTH][MAP_HEIGHT], char src_map[MAP_WIDTH][MAP_LENGTH][MAP_HEIGHT]){
  for (int i = 0 ; i < MAP_HEIGHT ; i++){
    for (int j = 0 ; j < MAP_WIDTH ; j++){
      for (int n = 0 ; n < MAP_LENGTH ; n++){
	dest_map[j][n][i] = src_map[j][n][i];
      }
    }
  }
  for (int i = 0 ; i < MAP_HEIGHT ; i++){
    for (int j = 0 ; j < MAP_WIDTH ; j++){
      for (int n = 0 ; n < MAP_LENGTH ; n++){
	if (i < MAP_HEIGHT-1 && src_map[j][n][i+1] != 0 && j > MAP_WIDTH-1 && src_map[j+1][n][i] != 0 && n < MAP_LENGTH-1 && src_map[j][n+1][i] != 0){
	  dest_map[j][n][i] = 0;
	}
      }
    }
  }
  setPhysicsMap(dest_map);
  setupCameraMap(dest_map);
}
