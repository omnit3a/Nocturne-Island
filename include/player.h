#ifndef PLAYER_H_
#define PLAYER_H_

#include <SDL2/SDL.h>
#include <drawer.h>
#include <map.h>
#include <entity.h>

#define SPAWN_X CHUNK_WIDTH/2
#define SPAWN_Y CHUNK_LENGTH/2
#define SPAWN_Z 11

void spawn_player();
int get_mining_speed();
void player_mine_block();
void player_place_block();
void handle_player_movement(SDL_Event event);
void handle_player_rotation(SDL_Event event);
entity_t * get_player_entity();
void init_player_entity();
transform_t get_player_direction();

#endif
