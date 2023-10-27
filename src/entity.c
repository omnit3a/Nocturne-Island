#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <entity.h>
#include <drawer.h>
#include <map.h>

void entity_move(entity_t * entity, transform_t * offset){
  entity->position.x = offset->x;
  entity->position.y = offset->y;
  entity->position.z = offset->z;
}

void entity_rotate(entity_t * entity, transform_t * offset){
  entity->rotation.x = offset->x;
  entity->rotation.y = offset->y;
  entity->rotation.z = offset->z;
}

void entity_apply_sprite(entity_t * entity, sprite_t * sprite){
  entity->sprite.renderer = sprite->renderer;
  strcpy(entity->sprite.path, sprite->path);
}

void entity_set_tag(entity_t * entity, tag_t * tag){
  strcpy(entity->tag.name, tag->name);
  entity->tag.id = tag->id;
  entity->tag.value = tag->value;
}

void entity_update_tag(entity_t * entity, tag_t * tag){
  entity->tag.value = tag->value;
}

void set_tag_value(tag_t * tag, int value){
  tag->value = value;
}

int get_tag_value(tag_t * tag){
  return tag->value;
}
