#include <stdio.h>
#include <stdlib.h>
#include <map.h>
#include <time.h>
#include <drawer.h>
#include <stdbool.h>
#include <player.h>
#include <camera.h>
#include <physics.h>

char world_map[MAP_WIDTH][MAP_LENGTH][MAP_HEIGHT];
block_data_t data_map[BLOCKS_AMOUNT] = {
  {-101, false, true, 0, 0, 0, UNKNOWN_TYPE},
  {3, true, false, STONE, STONE, 1, STONE_TYPE},
  {2, true, false, GRASS, STONE, 1, TERRAIN_TYPE},
  {1, true, false, WOOD, WOOD, 1, WOODEN_TYPE},
  {-101, false, true, WATER, 0, 0, UNKNOWN_TYPE},
  {4, true, false, MAGMA, STONE, 3, STONE_TYPE},
  {1, true, false, SAND, SAND, 1, TERRAIN_TYPE},
  {2, true, false, TREE_BOTTOM, WOOD, 2, WOODEN_TYPE},
  {2, true, false, OAK_TREE_LEAVES, WOOD, 2, WOODEN_TYPE},
  {2, true, false, PINE_TREE_LEAVES, WOOD, 2, WOODEN_TYPE},
  {-101, false, false, BLOCK_OUTLINE, 0, 0, UNKNOWN_TYPE},
  {-101, true, false, NOKIUM, 0, 0, UNKNOWN_TYPE},
  {2, true, false, STAIRS, STONE, 3, STONE_TYPE},
  {2, true, false, STAIRS, STONE, 3, STONE_TYPE},
  {2, true, false, STAIRS, STONE, 3, STONE_TYPE},
  {2, true, false, STAIRS, STONE, 3, STONE_TYPE},
  {1, true, false, WORK_BENCH, WORK_BENCH, 1, FURNITURE_TYPE},
  {1, true, false, TABLE, TABLE, 1, FURNITURE_TYPE},
  {3, true, false, IRON_ORE, IRON_CHUNKS, 2, METAL_TYPE},
  {3, true, false, COAL_ORE, COAL_CHUNKS, 2, METAL_TYPE},
  {2, true, false, IRON_CHUNKS, IRON_CHUNKS, 1, METAL_TYPE},
  {2, true, false, COAL_CHUNKS, COAL_CHUNKS, 1, METAL_TYPE},
  {1, true, false, ROPE, ROPE, 1, WOODEN_TYPE},
  {1, true, false, NAILS, NAILS, 1, METAL_TYPE},
  {1, true, false, PICKAXE, PICKAXE, 1, METAL_TYPE},
  {1, true, false, SHOVEL, SHOVEL, 1, METAL_TYPE},
  {1, true, false, AXE, AXE, 1, METAL_TYPE},
};
int block_hp_map[MAP_WIDTH][MAP_LENGTH][MAP_HEIGHT];

void loadBlockProperties(char * path, block_data_t * data){
  for (int id = 0 ; id < BLOCKS_AMOUNT ; id++){
    data[id]->id = id;
  }
}

void unloadBlockProperties(block_data_t * data){
  
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
  for (int i = 0 ; i < 400 ; i++){
    generateCave(map, seed, 500, 20);
  }

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

/* 3D Random walk for cave generation */
void generateCave(char map[MAP_WIDTH][MAP_LENGTH][MAP_HEIGHT], int seed, int size, int iterations){
  srand(seed);
  int xPos = rand() % MAP_WIDTH;
  int yPos = rand() % MAP_LENGTH;
  int zPos = 7;
  int xStart = xPos;
  int yStart = yPos;
  int zStart = zPos;
  int dir_value = 0;
  for (int reps = 0 ; reps < iterations ; reps++){
    srand(seed+reps);
    xPos = xStart;
    yPos = yStart;
    zPos = zStart;
    for (int i = 0 ; i < size ; i++){
      dir_value = rand() % 6;
      switch (dir_value){
        case 0:
	  if (yPos != 0){
	    yPos--;
	    break;
	  }
        case 1:
	  if (yPos != 0){
	    xPos--;
	    break;
	  }
        case 2:
	  if (yPos != MAP_LENGTH-1){
	    yPos++;
	    break;
	  }
        case 3:
	  if (xPos != MAP_WIDTH-1){
	    xPos++;
	    break;
	  }
        case 4:
	  if (zPos != 0){
	    zPos--;
	    break;
	  }
        case 5:
	  if (zPos != MAP_HEIGHT-1){
	    zPos++;
	    break;
	  }
	  continue;
      }
      if (xPos != playerX && yPos != playerY){
	if (getBlockProperties(map, xPos, yPos, zPos).block != NOKIUM &&
	    (getBlockProperties(map, xPos, yPos, zPos).block < 7 ||
	     getBlockProperties(map, xPos, yPos, zPos).block > 9)){
	  map[xPos][yPos][zPos] = 0;
	}
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
