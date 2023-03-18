#include <SDL2/SDL.h>
#include <map.h>
#include <lighting.h>

rgb_light_t light_map[MAP_WIDTH][MAP_LENGTH][MAP_HEIGHT];

void fillLightMap(rgb_light_t light_map[MAP_WIDTH][MAP_LENGTH][MAP_HEIGHT]){
  for (int i = 0 ; i < MAP_HEIGHT ; i++){
    for (int j = 0 ; j < MAP_WIDTH ; j++){
      for (int n = 0 ; n < MAP_LENGTH ; n++){
	light_map[j][n][i].i = DEFAULT_INTENSITY;
	light_map[j][n][i].r = DEFAULT_INTENSITY;
	light_map[j][n][i].g = DEFAULT_INTENSITY;
	light_map[j][n][i].b = DEFAULT_INTENSITY;
	light_map[j][n][i].brightness = 16;
	light_map[j][n][i].xPos = j;
	light_map[j][n][i].yPos = i;
	light_map[j][n][i].zPos = n;
      }
    }
  }
}

void spreadLight(rgb_light_t light){
  int iteratorZ = 100;
  for (int i = 5 ; i < (light.zPos+light.brightness) % 20 ; i++){
    for (int j = 5; j < (light.xPos+light.brightness) % 20; j++){
      for (int n = 5; n < (light.yPos+light.brightness) % 20; n++){
	light_map[j][n][i].i = iteratorZ;
	light_map[j][n][i].r = iteratorZ;
	light_map[j][n][i].g = iteratorZ;
	light_map[j][n][i].b = iteratorZ;
      }
    }
    if (i < light.brightness){
      iteratorZ++;
    } else {
      iteratorZ--;
    }
  }
}
void calculateLighting(char map[MAP_WIDTH][MAP_LENGTH][MAP_HEIGHT], day_time_t time){
  /* Find position of every light source */
  for (int i = 0 ; i < MAP_HEIGHT ; i++){
    for (int j = 0 ; j < MAP_WIDTH ; j++){
      for (int n = 0 ; n < MAP_LENGTH ; n++){
	if (light_map[j][n][i].brightness > 0){
	  spreadLight(light_map[j][n][i]);
	}
      }
    }
  }
}
