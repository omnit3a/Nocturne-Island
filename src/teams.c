#include <stdio.h>
#include <stdlib.h>
#include <teams.h>
#include <player.h>

int playerTeam = DEFAULT_TEAM;

void setTeam(int team, int respawn){
  playerTeam = team;
  if (respawn){
    playerX = SPAWN_X;
    playerY = SPAWN_Y;
    playerZ = SPAWN_Z;
  }
}

void swapTeam(int respawn){
  playerTeam = !playerTeam;
  if (respawn){
    playerX = SPAWN_X;
    playerY = SPAWN_Y;
    playerZ = SPAWN_Z;
  }
}
