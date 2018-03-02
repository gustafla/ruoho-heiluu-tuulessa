#include "demo.h"
#include "gl_headers.h"
#include "shaders.h"
#include "util.h"
#include <iostream>

#include "grass.h"

Demo::Demo(sync_device *rocket, MusicPlayer const &player, int w, int h):
  m_rocket(rocket), m_player(player), m_t(0.),
  m_projection(glm::perspective(glm::radians(60.), 16./9., .1, 100.)),
  m_gBufferShaderProgram(linkProgram(loadFile("vertex.glsl"),
        loadFile("fragment.glsl"))),
  m_gBuffer(w, h, {{GL_RGB16F, GL_RGB, GL_FLOAT}, {GL_RGB16F, GL_RGB, GL_FLOAT},
      {GL_RGBA, GL_RGBA, GL_UNSIGNED_BYTE}}),
  m_lightingPass("lighting.glsl"),
  m_grass(new Grass(*this))
{
  m_uTexDiffuse = getUniformLocation("u_texDiffuse");
  m_uTexSpecular = getUniformLocation("u_texSpecular");
  m_uProjection = getUniformLocation("u_projection");
  m_uView = getUniformLocation("u_view");

#if (SYNC_PLAYER)
  player.pause(false);
#endif
}

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

  glm::vec3 cameraPos(
      get("camera:pos.x"),
      get("camera:pos.y"),
      get("camera:pos.z"));
  glm::vec3 cameraTarget(
      get("camera:target.x"),
      get("camera:target.y"),
      get("camera:target.z"));
  glm::mat4 view = glm::lookAt(cameraPos, cameraTarget, glm::vec3(0., 1., 0.));

  // Bind G-Buffer
  m_gBuffer.bind();

  // Use G-Buf shader for rendering visuals
  glUseProgram(m_gBufferShaderProgram);
  glUniform1i(m_uTexDiffuse, 0);
  glUniform1i(m_uTexSpecular, 1);
  glUniformMatrix4fv(m_uProjection, 1, GL_FALSE, glm::value_ptr(m_projection));
  glUniformMatrix4fv(m_uView, 1, GL_FALSE, glm::value_ptr(view));

  // Render visuals
  m_grass->render();

  // Bind screen buffer
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // Bind G-Buffer textures
  m_gBuffer.bindTextures();

  // Render lighting pass
  m_lightingPass.render();

#if (DEBUG)
  GLenum err = glGetError();
  if (err != GL_NO_ERROR) {
    die(err);
  }
#endif
}

GLint Demo::getUniformLocation(std::string const &name) {
  return glGetUniformLocation(m_gBufferShaderProgram, name.c_str());
}
