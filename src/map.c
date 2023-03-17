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

void generateHills(char map[MAP_WIDTH][MAP_LENGTH][MAP_HEIGHT], int seed){
  fillMap(map);
  srand(seed);
  int heightSub = 7;
  char temp_height_map[MAP_WIDTH][MAP_LENGTH];
  char height_map[MAP_WIDTH][MAP_LENGTH];
  for (int i = 0 ; i < MAP_WIDTH ; i++){
    for (int j = 0 ; j < MAP_LENGTH ; j++){
      for (int m = 0 ; m < MAP_HEIGHT ; m++){
	if (m == heightSub+1){
	  map[i][j][m] = GRASS;
	} else if (m < heightSub+1){
	  map[i][j][m] = STONE;
	}
	height_map[i][j] = (rand() % heightSub)+1;
      }
    }
  }
  int surroundVals[4];
  int average;
  for (int r = 0 ; r < 5 ; r++){
    for (int i = 0 ; i < MAP_WIDTH ; i++){
      for (int j = 0 ; j < MAP_LENGTH ; j++){
        if (i > 0){
      	  surroundVals[0] = height_map[i-1][j];
        } else {
      	  surroundVals[0] = 0;
        }
        if (j > 0){
      	  surroundVals[1] = height_map[i][j-1];
        } else {
      	  surroundVals[1] = 0;
        }
        if (i < MAP_WIDTH-1){
      	  surroundVals[2] = height_map[i+1][j];
        } else {
      	  surroundVals[2] = 0;
        }
        if (j < MAP_LENGTH-1){
      	  surroundVals[3] = height_map[i][j+1];
        } else {
      	  surroundVals[3] = 0;
        }
        for (int a = 0 ; a < 4 ; a++){
          average += surroundVals[a];
        }
        average = average / 4;
        temp_height_map[i][j] = average/1.25;
      }
    }
    for (int i = 0 ; i < MAP_WIDTH ; i++){
      for (int j = 0 ; j < MAP_LENGTH ; j++){
      	height_map[i][j] = temp_height_map[i][j];
      }
    }
  }
  
  for (int i = 0 ; i < MAP_WIDTH ; i++){
    for (int j = 0 ; j < MAP_LENGTH ; j++){
      for (int n = heightSub+1 ; n >= temp_height_map[i][j] ; n--){
      	map[i][j][n] = 0;
      }
      map[i][j][(int)height_map[i][j]] = GRASS;
    }
  }
  for (int i = 0 ; i < MAP_WIDTH ; i++){
    for (int j = 0 ; j < MAP_LENGTH ; j++){
      if (map[i][j][1] == 0){
        map[i][j][1] = WATER;
      }
      map[i][j][0] = SAND;
    }
  }
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
