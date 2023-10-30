#pragma once

#define TICKS_PER_SECOND 100
#define SDL_TICKS_PER_DAY 300 * 1000

int get_current_tick();
void tick_update();
void day_night_update();
int is_daytime();
void set_fps_start();
int get_fps();
