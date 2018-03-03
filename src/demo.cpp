#include "demo.h"
#include "gl_headers.h"
#include "shaders.h"
#include "util.h"
#include <iostream>

#include "grass.h"
#include "ground.h"

Demo::Demo(sync_device *rocket, MusicPlayer &player, int w, int h):
  m_rocket(rocket), m_player(player), m_t(0.),
  m_projection(glm::perspective(glm::radians(60.), 16./9., .1, 500.)),
  m_gBufferShaderProgram(linkProgram(loadFile("vertex.glsl"),
        loadFile("fragment.glsl"))),
  m_gBuffer(w, h, {{GL_RGB16F, GL_RGB, GL_FLOAT}, {GL_RGB16F, GL_RGB, GL_FLOAT},
      {GL_RGBA, GL_RGBA, GL_UNSIGNED_BYTE}}),
  m_lightingPass("lighting.glsl"),
  m_grass(new Grass(*this)),
  m_ground(new Ground(*this))
{
  if (!m_gBufferShaderProgram) {
    std::cerr << "Failed to link g-buffer shader program" << std::endl;
    die(EXIT_FAILURE);
  }

  m_uTexDiffuse = getUniformLocation("u_texDiffuse");
  m_uTexSpecular = getUniformLocation("u_texSpecular");
  m_uProjection = getUniformLocation("u_projection");
  m_uView = getUniformLocation("u_view");

  // Gen noise tex
  glGenTextures(1, &m_texNoise);
  glBindTexture(GL_TEXTURE_2D, m_texNoise);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, DEMO_POST_NOISE_SIZE,
      DEMO_POST_NOISE_SIZE, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

#if (SYNC_PLAYER)
  player.pause(false);
#endif
}

Demo::~Demo() {
  delete m_grass;
  delete m_ground;
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
  m_ground->render();
  m_grass->render();

  // Bind screen buffer
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // Bind G-Buffer textures
  m_gBuffer.bindTextures();

  // Update and bind noise texture
  static GLubyte buf[DEMO_POST_NOISE_SIZE * DEMO_POST_NOISE_SIZE * 3];
  for (int i=0; i<DEMO_POST_NOISE_SIZE*DEMO_POST_NOISE_SIZE*3; ++i) {
    buf[i] = fastrand()%255;
  }
  glActiveTexture(GL_TEXTURE3);
  glBindTexture(GL_TEXTURE_2D, m_texNoise);
  glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, DEMO_POST_NOISE_SIZE,
      DEMO_POST_NOISE_SIZE, GL_RGB, GL_UNSIGNED_BYTE, buf);

  // Update lighting uniforms
  m_lightingPass.setUniform("u_cameraPos", 3, glm::value_ptr(cameraPos));
  m_lightingPass.setUniform("u_cameraTarget", 3, glm::value_ptr(cameraTarget));

  // Render lighting pass
  m_lightingPass.render();

#if (DEBUG)
  GLenum err = glGetError();
  if (err != GL_NO_ERROR) {
    std::cout << "glGetError failed" << std::endl;
    switch(err) {
      case GL_INVALID_ENUM:      std::cerr << "GL_INVALID_ENUM"; break;
      case GL_INVALID_VALUE:     std::cerr << "GL_INVALID_VALUE"; break;
      case GL_INVALID_OPERATION: std::cerr << "GL_INVALID_OPERATION"; break;
      case GL_STACK_OVERFLOW:    std::cerr << "GL_STACK_OVERFLOW"; break;
      case GL_STACK_UNDERFLOW:   std::cerr << "GL_STACK_UNDERFLOW"; break;
      case GL_OUT_OF_MEMORY:     std::cerr << "GL_OUT_OF_MEMORY"; break;
      default: break;
    }
    die(err);
  }
#endif
#if (SYNC_PLAYER)
  if (!m_player.isPlaying()) {
    die(EXIT_SUCCESS);
  }
#endif
}

GLint Demo::getUniformLocation(std::string const &name) {
  return glGetUniformLocation(m_gBufferShaderProgram, name.c_str());
}
