#include "grass.h"
#include "demo.h"
#include "util.h"
#include "shaders.h"
#include "gl_headers.h"
#include <cmath>

Grass::Grass(Demo &demo): m_demo(demo) {
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
      (void*)2);
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat),
      (void*)5);
  glEnableVertexAttribArray(2);

  // Load, compile and link shaders
  m_program = linkProgram(loadFile("vertex.glsl"), loadFile("fragment.glsl"));
  if (!m_program) {
    die(EXIT_FAILURE);
  }

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
  glDeleteProgram(m_program);
}

void Grass::render() {
  glBindVertexArray(m_vertexArray);
  GLuint uniformMvp = m_demo.getUniformLocation("mvp");

  for (auto v: m_positions) { // render n blades of grass
    float r = (sin(v.x + m_demo.get("grass.t"))*0.3+0.3) * m_demo.get("wind");
    r += sin(v.z*0.3+0.7 - m_demo.get("grass.t")*0.9)*0.2;
    glm::mat4 t = glm::translate(glm::mat4(1.), v);
    glm::mat4 rot = glm::rotate(t, r, glm::vec3(1,0,0));
    glm::mat4 mvp = m_demo.camera() * glm::rotate(rot, .7f, glm::vec3(0,-1,0));
    glUniformMatrix4fv(uniformMvp, 1, GL_FALSE, glm::value_ptr(mvp));
    glDrawArrays(GL_TRIANGLES, 0, 3);
  }

  glBindVertexArray(0);
}
