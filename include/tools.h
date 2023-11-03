#pragma once

typedef struct tool_action_s {
  int (*action)(void);
  int item_id;
  int id;
} tool_action_t;

void handle_action(int id);

int sharp_stick_action();
int sharp_rock_action();
int stone_axe_action();
