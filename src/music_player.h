#pragma once

#include <SDL.h>
#include <string>

class MusicPlayer {
  public:
    MusicPlayer();
    MusicPlayer(std::string filename);
    ~MusicPlayer();
    void pause(bool pause);
    void setPosition(double time);
    bool isPlaying();
    double getTime() const;

  private:
    static void callback(void *userData, Uint8 *stream, int len);

    Uint32 m_callbackTicks; // More precision for timing
    int m_samples;
    Uint8 *m_audio;
    Uint8 *m_position;
    SDL_AudioSpec m_audioSpec;
    SDL_AudioDeviceID m_audioDevice;
};
