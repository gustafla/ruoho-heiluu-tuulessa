#pragma once

#include <SDL.h>
#include <string>

class MusicPlayer {
  public:
    MusicPlayer(std::string filename);
    ~MusicPlayer();
    void pause(bool pause);
    void setPosition(float time);
  
  private:
    static void callback(void *userData, Uint8 *stream, int len);
    int samples;
    Uint8 *audio;
    Uint8 *position;
    SDL_AudioSpec audioSpec;
    SDL_AudioDeviceID audioDevice;
};
