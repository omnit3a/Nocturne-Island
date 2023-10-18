#include <stdio.h>
#include <stdlib.h>
#include <player.h>
#include <lighting.h>
#include <map.h>
#include <drawer.h>

int * calculateFogRGB(){
  static int rgbColors[3];
  int xPos = iBackup;
  int yPos = jBackup;
  for (int i = 0 ; i < 3 ; i++){
    if (xPos < playerX){
      rgbColors[i] = (playerX - xPos) * 5;
      if (yPos < playerY){
	rgbColors[i] += (playerY - yPos) * 5;
      } else {
	rgbColors[i] += (yPos - playerY) * 5;
      }
    } else {
      rgbColors[i] = (xPos - playerX) * 5;
      if (yPos < playerY){
	rgbColors[i] += (playerY - yPos) * 5;
      } else {
	rgbColors[i] += (yPos - playerY) * 5;
      }
    }
    if (rgbColors[i] > 32){
      rgbColors[i] = 38;
    }
  }
  return rgbColors;
}
