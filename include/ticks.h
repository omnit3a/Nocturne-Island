#pragma once

#define TICKS_PER_SECOND 100
#define SDL_TICKS_PER_DAY 300 * 1000

int get_current_tick();
void tick_update();
void day_night_update();
int is_daytime();
void hunger_update();
void water_flow_update();
void fire_update(int x_off, int y_off);
int get_days_survived();
