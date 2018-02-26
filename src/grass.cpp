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

  glGenBuffers(1, &buffer);
  glBindBuffer(GL_ARRAY_BUFFER, buffer);
  glBufferData(GL_ARRAY_BUFFER, sizeof(v), v, GL_STATIC_DRAW);

  program = linkProgram(loadFile("vertex.glsl"), loadFile("fragment.glsl"));
  if (!program) {
    die(EXIT_FAILURE);
  }
}

void Grass::render() {
  
}
