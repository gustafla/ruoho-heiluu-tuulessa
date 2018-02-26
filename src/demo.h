#pragma once

#include "sync.h" // rocket
#include "music_player.h"
#include <unordered_map>

class Grass; // grass.h

class Demo {
  public:
    Demo(sync_device *rocket, MusicPlayer const &player);
    ~Demo();
    double get(std::string track);
    void render();

  private:
    sync_device *m_rocket;
    MusicPlayer const &m_player;
    double m_t;
    // Likely faster to cache tracks to a map as sync_get_track only iterates.
    std::unordered_map<std::string, sync_track const *> m_tracks;

    // Visual components
    Grass *m_grass;
};
