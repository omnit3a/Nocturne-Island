#include <stdio.h>
#include <stdlib.h>
#include <map.h>
#include <time.h>
#include <drawer.h>
#include <stdbool.h>

char world_map[MAP_WIDTH][MAP_LENGTH][MAP_HEIGHT];
bool solid_map[MAP_WIDTH][MAP_LENGTH][MAP_HEIGHT];

void fillMap(char map[MAP_WIDTH][MAP_LENGTH][MAP_HEIGHT]){
  for (int i = 0 ; i < MAP_HEIGHT ; i++){
    for (int j = 0 ; j < MAP_WIDTH ; j++){
      for (int n = 0 ; n < MAP_LENGTH ; n++){
	map[j][n][i] = 0;
	solid_map[j][n][i] = false;
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
	  offset = 3;
	}
        if ((rand() % 1000) <= TREE_CHANCE*offset){
	  map[i][j][(int)height_map[i][j]+1] = TREE_BOTTOM;
	  map[i][j][(int)height_map[i][j]+3] = TREE_LEAVES;
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
 *   Place water table
 * Step 6:
 *   Place trees
 */
void generateHills(char map[MAP_WIDTH][MAP_LENGTH][MAP_HEIGHT], int seed){
  fillMap(map);
  srand(seed);
  int isMountain = 0;
  int prevIsMountain;
  char temp_height_map[MAP_WIDTH][MAP_LENGTH];
  char height_map[MAP_WIDTH][MAP_LENGTH];
  for (int i = 0 ; i < MAP_WIDTH ; i++){
    for (int j = 0 ; j < MAP_LENGTH ; j++){
      height_map[i][j] = 0;
      if (prevIsMountain){
        isMountain = ((rand() % 100) < 75);
      } else {
        isMountain = ((rand() % 100) < 10);
      }
      if (isMountain){
        height_map[i][j] = CLIFF_HEIGHT;
      }
      prevIsMountain = isMountain;
    }
  }

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

  for (int i = 0 ; i < MAP_WIDTH ; i++){
    for (int j = 0 ; j < MAP_LENGTH ; j++){
      map[i][j][(int)height_map[i][j]] = GRASS;
      for (int n = height_map[i][j]-1 ; n > 0 ; n--){
        map[i][j][n] = STONE;
      }
      map[i][j][0] = NOKIUM;
    }
  }
  placeTrees(map, height_map, seed);
}

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
}

void generateSolidity(char map[MAP_WIDTH][MAP_LENGTH][MAP_HEIGHT]){
  for (int i = 0 ; i < MAP_WIDTH ; i++){
    for (int j = 0 ; j < MAP_LENGTH ; j++){
      for (int n = 0 ; n < MAP_HEIGHT ; n++){
        switch(map[i][j][n]){
	  case 0:
	    solid_map[i][j][n] = false;
	    break;
	  case WATER:
	    solid_map[i][j][n] = false;
	    break;
	  default:
	    solid_map[i][j][n] = true;
	    break;
	}
      }
    }
  }
} 
