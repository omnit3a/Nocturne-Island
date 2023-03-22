#include <stdio.h>
#include <stdlib.h>
#include <player.h>
#include <lighting.h>
#include <map.h>
#include <drawer.h>

int * calculateFogRGB(){
  static int rgbColors[3];
  for (int i = 0 ; i < 3 ; i++){
    if (iBackup < playerX){
      rgbColors[i] = (playerX - iBackup) * 5;
      if (jBackup < playerY){
	rgbColors[i] += (playerY - jBackup) * 5;
      } else {
	rgbColors[i] += (jBackup - playerY) * 5;
      }
    } else {
      rgbColors[i] = (iBackup - playerX) * 5;
      if (jBackup < playerY){
	rgbColors[i] += (playerY - jBackup) * 5;
      } else {
	rgbColors[i] += (jBackup - playerY) * 5;
      }
    }
    if (rgbColors[i] > 32){
      rgbColors[i] = 38;
    }
  }
  return rgbColors;
}
