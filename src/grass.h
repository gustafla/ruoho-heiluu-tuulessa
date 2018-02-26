#pragma once

#include <glm/vec3.hpp>
#include "gl_headers.h"

class Demo; // demo.h, can't include here

class Grass {
  public:
    Grass(Demo &demo);
    void render();

  private:
    GLuint buffer;
    GLuint vertexArray;
    GLuint program;

    Demo &demo;
};
