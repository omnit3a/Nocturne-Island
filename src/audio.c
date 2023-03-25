#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <audio.h>

int playWav(char * path_name){
  SDL_AudioSpec wav_spec;
  unsigned int wav_length;
  unsigned char * wav_buffer;

  SDL_LoadWAV(path_name, &wav_spec, &wav_buffer, &wav_length);
  SDL_AudioDeviceID device_id = SDL_OpenAudioDevice(NULL, 0, &wav_spec, NULL, 0);
  int success = SDL_QueueAudio(device_id, wav_buffer, wav_length);
  SDL_PauseAudioDevice(device_id, 0);
  SDL_Delay(50);
  SDL_CloseAudioDevice(device_id);
  SDL_FreeWAV(wav_buffer);
  return success;
}
