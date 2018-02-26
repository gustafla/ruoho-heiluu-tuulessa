#pragma once

#include <glm/vec3.hpp>
#include "GL/glew.h"

class Demo; // demo.h, can't include here

class Grass {
  public:
    Grass(Demo &demo);
    void render();

  private:
    GLuint buffer;
    GLuint program;
    Demo &demo;
};
