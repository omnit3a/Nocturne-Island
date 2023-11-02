#include <stdio.h>
#include <stdlib.h>
#include <map.h>
#include <pathfinding.h>
#include <entity.h>

path_stack_t * path_create(){
  path_stack_t * result = malloc(sizeof(path_stack_t *));
  result->stack_pointer = 0;
  return result;
}

void path_destroy(path_stack_t * stack){
  free(stack);
}

void path_push(path_stack_t * stack, transform_t position, int explored){
  int sp = stack->stack_pointer;
  stack->nodes[sp].position = position;
  stack->nodes[sp].explored = explored;
  stack->stack_pointer++;
  if (stack->stack_pointer == PATH_STACK_SIZE){
    stack->stack_pointer = 0;
    printf("Pathfinding: Stack overflow\n");
  }
}

path_node_t path_pop(path_stack_t * stack){
  int sp = --stack->stack_pointer;
  if (sp < 0){
    stack->stack_pointer = PATH_STACK_SIZE - 1;
    printf("Pathfinding: Stack underflow\n");
  }
  return stack->nodes[sp];
}
path_node_t path_peek(path_stack_t * stack){
  int sp = stack->stack_pointer - 1;
  return stack->nodes[sp];
}

void path_pathfind(path_stack_t * stack, transform_t start, transform_t end){
}
