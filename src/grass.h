#pragma once

#include <glm/vec3.hpp>
#include "gl_headers.h"

class Demo; // demo.h, can't include here

class Grass {
  public:
    Grass(Demo &demo);
    ~Grass();
    void render();

  private:
    GLuint m_buffer;
    GLuint m_vertexArray;
    GLuint m_program;

    Demo &m_demo;
};
