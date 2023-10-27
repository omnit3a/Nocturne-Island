#pragma once

#include <drawer.h>

typedef struct transform_s {
  int x;
  int y;
  int z;
} transform_t;

typedef struct tag_s {
  int id;
  int value;
} tag_t;

typedef struct sprite_s {
  render_obj_t renderer;
} sprite_t;

typedef struct entity_s {
  transform_t position;
  transform_t rotation;
  sprite_t sprite;
  tag_t tag;
} entity_t;

void entity_move(entity_t * entity, transform_t * offset);
void entity_rotate(entity_t * entity, transform_t * offset);
void entity_set_sprite(entity_t * entity, sprite_t * sprite);
void entity_set_tag(entity_t * entity, tag_t * tag);
void entity_update_tag(entity_t * entity, tag_t * tag);
void set_tag_value(tag_t * tag, int value);
int get_tag_value(tag_t * tag);
