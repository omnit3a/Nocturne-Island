#pragma once

#include <entity.h>

#define PATH_STACK_SIZE 1024

typedef struct path_node_s {
  transform_t position;
  int explored;
} path_node_t;

typedef struct path_stack_s {
  path_node_t nodes[PATH_STACK_SIZE];
  int stack_pointer;
} path_stack_t;

path_stack_t * path_create();
void path_destroy(path_stack_t * stack);

void path_push(path_stack_t * stack, transform_t position, int explored);
path_node_t path_pop(path_stack_t * stack);
path_node_t path_peek(path_stack_t * stack);

void path_pathfind(path_stack_t * stack, transform_t start, transform_t end);
