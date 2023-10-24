#ifndef TEAMS_H_
#define TEAMS_H_
#include <map.h>

#define RED_TEAM 0
#define BLUE_TEAM 1

#define SPAWN_X MAP_WIDTH/2 
#define SPAWN_Y MAP_LENGTH/2
#define SPAWN_Z 11
#define DEFAULT_TEAM RED_TEAM

extern int playerTeam;

void setTeam(int team, int respawn);
void swapTeam(int respawn);

#endif
