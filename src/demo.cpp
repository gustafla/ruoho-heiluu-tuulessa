#include "demo.h"
#include "gl_headers.h"

#include "grass.h"

Demo::Demo(sync_device *rocket, MusicPlayer const &player):
  m_rocket(rocket), m_player(player), m_t(0.), m_grass(new Grass(*this)) {}

Demo::~Demo() {
  delete m_grass;
}

double Demo::get(std::string name) {
  sync_track const *track;
  try {
    track = m_tracks.at(name);
  } catch(std::out_of_range) {
    track = sync_get_track(m_rocket, name.c_str());
    m_tracks.emplace(name, track);
  }
  return sync_get_val(track, m_t*ROW_RATE);
}

void Demo::render() {
  m_t = m_player.getTime();
  glClearColor(get("red"), get("green"), get("blue"), 1.);
  glClear(GL_COLOR_BUFFER_BIT);

  m_grass->render();
}
