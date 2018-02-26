#include "grass.h"
#include "demo.h"
#include "util.h"
#include "shaders.h"

Grass::Grass(Demo &demo): m_demo(demo) {
  GLfloat v[] = {
    -0.5f, -0.5f, 0.0f,
     0.5f, -0.5f, 0.0f,
     0.0f,  0.5f, 0.0f
  };
  
  // Gen buffer and VA objects
  glGenBuffers(1, &m_buffer);
  glBindBuffer(GL_ARRAY_BUFFER, m_buffer);
  glGenVertexArrays(1, &m_vertexArray);
  glBindVertexArray(m_vertexArray);

  // Load data
  glBufferData(GL_ARRAY_BUFFER, sizeof(v), v, GL_STATIC_DRAW);

  // "Record" to VAO which VBO is in use and how to configure rendering with it
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat),
      (void*)0);
  glEnableVertexAttribArray(0);

  // Load, compile and link shaders
  m_program = linkProgram(loadFile("vertex.glsl"), loadFile("fragment.glsl"));
  if (!m_program) {
    die(EXIT_FAILURE);
  }
}

Grass::~Grass() {
  glDeleteVertexArrays(1, &m_vertexArray);
  glDeleteBuffers(1, &m_buffer);
  glDeleteProgram(m_program);
}

void Grass::render() {
  glUseProgram(m_program);
  glBindVertexArray(m_vertexArray);
  glDrawArrays(GL_TRIANGLES, 0, 3);
  glBindVertexArray(0);
}
