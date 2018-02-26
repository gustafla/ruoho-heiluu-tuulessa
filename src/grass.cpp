#include "grass.h"
#include "demo.h"
#include "util.h"
#include "shaders.h"

Grass::Grass(Demo &demo): demo(demo) {
  GLfloat v[] = {
    -0.5f, -0.5f, 0.0f,
     0.5f, -0.5f, 0.0f,
     0.0f,  0.5f, 0.0f
  };
  
  // Gen buffer and VA objects
  glGenBuffers(1, &buffer);
  glGenVertexArrays(1, &vertexArray);
  glBindVertexArray(vertexArray);

  // Bind buffer and load data
  glBindBuffer(GL_ARRAY_BUFFER, buffer);
  glBufferData(GL_ARRAY_BUFFER, sizeof(v), v, GL_STATIC_DRAW);

  // "Record" to VAO which VBO is in use and how to configure rendering with it
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat),
      (void*)0);
  glEnableVertexAttribArray(0);

  // Load, compile and link shaders
  program = linkProgram(loadFile("vertex.glsl"), loadFile("fragment.glsl"));
  if (!program) {
    die(EXIT_FAILURE);
  }
}

void Grass::render() {
  glUseProgram(program);
  glBindVertexArray(vertexArray);
  glDrawArrays(GL_TRIANGLES, 0, 3);
  glBindVertexArray(0);
}
