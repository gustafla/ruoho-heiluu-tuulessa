#pragma once

#include <vector>
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

    std::vector<glm::vec3> m_positions;

    GLuint m_texDiffuse;
    GLuint m_texSpecular;

    Demo &m_demo;
};
