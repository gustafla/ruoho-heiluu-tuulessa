#pragma once

#include "sync.h" // rocket
#include "music_player.h"
#include <unordered_map>

class Demo {
  public:
    Demo(sync_device *rocket, MusicPlayer const &player);
    float get(std::string track);
    void render();

  private:
    sync_device *rocket;
    MusicPlayer const &player;
    float t;
    // Likely faster to cache tracks to a map as sync_get_track only iterates.
    std::unordered_map<std::string, sync_track const *> tracks;
};
