#pragma once

#include "sync.h" // rocket
#include "music_player.h"

class Demo {
  public:
    Demo(sync_device *rocket, MusicPlayer const &player);
    void render() const;

  private:
    sync_device *rocket;
    MusicPlayer const &player;
};
