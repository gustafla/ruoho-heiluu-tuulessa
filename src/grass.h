#pragma once

#include <glm/vec3.hpp>

class Demo; // demo.h, can't include here

class Grass {
  public:
    Grass(Demo &demo);
    void render();

  private:
    Demo &demo;
};
