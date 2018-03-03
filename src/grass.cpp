#include "grass.h"
#include "demo.h"
#include "util.h"
#include "shaders.h"
#include "gl_headers.h"
#include "procedural_texture.h"
#include <cmath>

Grass::Grass(Demo &demo):
  m_texDiffuse(renderProceduralTexture(256, 256, "grass_diffuse.glsl",
      {GL_RGB, GL_RGB, GL_UNSIGNED_BYTE})),
  m_texSpecular(renderProceduralTexture(256, 256, "grass_specular.glsl",
      {GL_R8, GL_RED, GL_UNSIGNED_BYTE})),
  m_demo(demo)
{
  GLfloat v[] = {
    -.1, 0., 0., 0., 0., 0., 0., 1.,
     .1, 0., 0., 1., 0., 0., 0., 1.,
     0., 1., 0., .5, 1., 0., 0., 1.
  };

  // Gen buffer and VA objects
  glGenBuffers(1, &m_buffer);
  glBindBuffer(GL_ARRAY_BUFFER, m_buffer);
  glGenVertexArrays(1, &m_vertexArray);
  glBindVertexArray(m_vertexArray);

  // Load data
  glBufferData(GL_ARRAY_BUFFER, sizeof(v), v, GL_STATIC_DRAW);

  // "Record" to VAO which VBO is in use and how to configure rendering with it
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat),
      (void*)0);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat),
      (void*)(3*sizeof(GLfloat)));
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat),
      (void*)(5*sizeof(GLfloat)));
  glEnableVertexAttribArray(2);

  // Make up some positions
  for (int i = 0; i < 100; i++) {
    double x = (fastrand() % 20000)/10000. -1.;
    double y = (fastrand() % 20000)/10000. -1.;
    m_positions.push_back(glm::vec3(x*3, 0., y*3));
  }
  for (int i = 0; i < 100; i++) {
    double x = (fastrand() % 20000)/10000. -1.;
    double y = (fastrand() % 20000)/10000. -1.;
    m_positions.push_back(glm::vec3(x*6, 0., y*6));
  }
  for (int i = 0; i < 100; i++) {
    double x = (fastrand() % 20000)/10000. -1.;
    double y = (fastrand() % 20000)/10000. -1.;
    m_positions.push_back(glm::vec3(x*12, 0., y*12));
  }
}

Grass::~Grass() {
  glDeleteVertexArrays(1, &m_vertexArray);
  glDeleteBuffers(1, &m_buffer);
  glDeleteTextures(1, &m_texDiffuse);
  glDeleteTextures(1, &m_texSpecular);
}

void Grass::render() {
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, m_texDiffuse);
  glActiveTexture(GL_TEXTURE1);
  glBindTexture(GL_TEXTURE_2D, m_texSpecular);

  glBindVertexArray(m_vertexArray);

  GLuint uModel = m_demo.getUniformLocation("u_model");
  for (auto v: m_positions) { // render n blades of grass
    // fake wind
    float r = (sin(v.x + m_demo.get("grass.t"))*0.3+0.3) * m_demo.get("wind");
    r += sin(v.z*0.3+0.7 - m_demo.get("grass.t")*0.9)*0.2;

    // Model matrix
    glm::mat4 t = glm::translate(glm::mat4(1.), v);
    glm::mat4 rot = glm::rotate(t, r, glm::vec3(1,0,0));
    rot = glm::rotate(rot, .7f, glm::vec3(0,-1,0));

    // Upload
    glUniformMatrix4fv(uModel, 1, GL_FALSE, glm::value_ptr(rot));

    // Draw
    glDrawArrays(GL_TRIANGLES, 0, 3);
  }

  glBindVertexArray(0);
}
