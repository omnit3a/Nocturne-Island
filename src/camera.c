#include <SDL2/SDL.h>
#include <camera.h>
#include <stdio.h>
#include <stdlib.h>
#include <drawer.h>
#include <ui.h>

SDL_Renderer * camera_renderer;
render_obj_t camera_object;
render_obj_t overlay_object;

void get_camera_view(int * x, int * y ){
  *x = 13;
  *y = 9;  
}

void setup_camera(SDL_Renderer * renderer, SDL_Window * window){
  camera_object.window = window;
  camera_object.renderer = renderer;
  overlay_object.renderer = renderer;
}

void update_camera(){
  SDL_SetRenderDrawColor(camera_object.renderer, 0, 0, 0, 255);
  SDL_RenderClear(camera_object.renderer);

  draw_view(&camera_object);
  draw_player(&camera_object);
  draw_ui(&camera_object);
  
  SDL_RenderPresent(camera_object.renderer);
}
