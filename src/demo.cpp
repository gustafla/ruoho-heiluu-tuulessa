#include "demo.h"
#include "gl_headers.h"
#include "shaders.h"
#include "util.h"
#include <iostream>

#include "grass.h"

Demo::Demo(sync_device *rocket, MusicPlayer const &player, int w, int h):
  m_rocket(rocket), m_player(player), m_t(0.),
  m_projection(glm::perspective(glm::radians(60.), 16./9., .1, 100.)),
  m_lightingPass("lighting.glsl"),
  m_grass(new Grass(*this))
{
  glGenFramebuffers(1, &m_gBuffer);
  glBindFramebuffer(GL_FRAMEBUFFER, m_gBuffer);
  glGenTextures(3, m_gBufferTextures);

  // Position color buffer
  glBindTexture(GL_TEXTURE_2D, m_gBufferTextures[0]);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, w, h, 0, GL_RGB, GL_FLOAT, nullptr);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D,
      m_gBufferTextures[0], 0);

  // Normal color buffer
  glBindTexture(GL_TEXTURE_2D, m_gBufferTextures[1]);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, w, h, 0, GL_RGB, GL_FLOAT, nullptr);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D,
      m_gBufferTextures[1], 0);

  // Albedo and specular color buffer
  glBindTexture(GL_TEXTURE_2D, m_gBufferTextures[2]);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE,
      nullptr);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D,
      m_gBufferTextures[2], 0);

  // Gen and add RBO
  glGenRenderbuffers(1, &m_gBufferRenderBuffer);
  glBindRenderbuffer(GL_RENDERBUFFER, m_gBufferRenderBuffer);
  glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, w, h);
  glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT,
      GL_RENDERBUFFER, m_gBufferRenderBuffer);

  // Check for completeness
  if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
    std::cerr << "g-buffer FBO not complete" << std::endl;
    die(EXIT_FAILURE);
  }

  // FB's attachments for rendering
  GLuint attachments[3] = {
    GL_COLOR_ATTACHMENT0,
    GL_COLOR_ATTACHMENT1,
    GL_COLOR_ATTACHMENT2};
  glDrawBuffers(3, attachments); // This is part of the FBO state

  // shader program for g-buffer
  m_gBufferShaderProgram = linkProgram(
      loadFile("vertex.glsl"), loadFile("fragment.glsl"));
  if (!m_gBufferShaderProgram) {
    die(EXIT_FAILURE);
  }

#if (SYNC_PLAYER)
  player.pause(false);
#endif
}

Demo::~Demo() {
  delete m_grass;
  glDeleteRenderbuffers(1, &m_gBufferRenderBuffer);
  glDeleteTextures(3, m_gBufferTextures);
  glDeleteFramebuffers(1, &m_gBuffer);
  glDeleteProgram(m_gBufferShaderProgram);
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

GLfloat const *Demo::view() {
  return glm::value_ptr(m_view);
}

GLfloat const *Demo::projection() {
  return glm::value_ptr(m_projection);
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
  m_view = glm::lookAt(cameraPos, cameraTarget, glm::vec3(0., 1., 0.));

  // Bind G-Buffer
  glBindFramebuffer(GL_FRAMEBUFFER, m_gBuffer);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glUseProgram(m_gBufferShaderProgram);

  // Render visuals
  m_grass->render();

  // Bind screen buffer
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // Bind G-Buffer textures
  for (int i=0; i<3; i++) {
    glActiveTexture(GL_TEXTURE0 + i);
    glBindTexture(GL_TEXTURE_2D, m_gBufferTextures[i]);
  }

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
