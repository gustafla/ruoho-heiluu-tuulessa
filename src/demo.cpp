#include "demo.h"
#include "GL/glew.h"

#include "grass.h"

Demo::Demo(sync_device *rocket, MusicPlayer const &player):
  rocket(rocket), player(player), t(0.), grass(new Grass(*this)) {}

Demo::~Demo() {
  delete grass;
}

double Demo::get(std::string name) {
  sync_track const *track;
  try {
    track = tracks.at(name);
  } catch(std::out_of_range) {
    track = sync_get_track(rocket, name.c_str());
    tracks.emplace(name, track);
  }
  return sync_get_val(track, t*ROW_RATE);
}

void Demo::render() {
  t = player.getTime();
  glClearColor(get("red"), get("green"), get("blue"), 1.);
  glClear(GL_COLOR_BUFFER_BIT);
}
