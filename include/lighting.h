#ifndef LIGHTING_H_
#define LIGHTING_H_

#include <player.h>
#include <map.h>
#include <stdbool.h>

#define MAX_BRIGHTNESS 16
#define DEFAULT_INTENSITY 90

typedef struct rgb_light_s {
  unsigned char r; /* red intensity */
  unsigned char g; /* green intensity */
  unsigned char b; /* blue intensity */
  unsigned char i; /* brightness */
  int xPos;
  int yPos;
  int zPos;
  bool exclusive; /* only emit light in the specified direction */
  rotation_t xyRotation;
  int brightness; /* starting brightness */
} rgb_light_t;

typedef enum day_time_e {
  MORNING = 1,
  NOON = 2,
  EVENING = 1,
  NIGHT = -1
} day_time_t;

extern rgb_light_t light_map[MAP_WIDTH][MAP_LENGTH][MAP_HEIGHT]; 

void fillLightMap(rgb_light_t light_map[MAP_WIDTH][MAP_LENGTH][MAP_HEIGHT]);
void spreadLight(rgb_light_t light);
void calculateLighting(char map[MAP_WIDTH][MAP_LENGTH][MAP_HEIGHT], day_time_t time);

#endif
