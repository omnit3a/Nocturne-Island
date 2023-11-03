#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <tools.h>
#include <map.h>
#include <player.h>
#include <tool_defs.h>
#include <map_defs.h>

tool_action_t actions[TOOL_ACTIONS_AMOUNT] = {
  {&sharp_stick_action, SHARP_STICK, SHARP_STICK_TOOL_ID},
  {&sharp_rock_action, SHARP_ROCK, SHARP_ROCK_TOOL_ID},
};

void handle_action(int id){
  int action_id = 0;
  for (int action = 0 ; action < TOOL_ACTIONS_AMOUNT ; action++){
    if (actions[action].item_id == id){
      action_id = action;
      break;
    }
  }

  if (get_block_properties(id).block_type != TOOL_TYPE){
    return;
  }
  
  int result = actions[action_id].action();
}

int sharp_stick_action(){
  printf("used: sharp_stick\n");
  return 0;
}

int sharp_rock_action(){
  printf("used: sharp_rock\n");
  return 0;
}
