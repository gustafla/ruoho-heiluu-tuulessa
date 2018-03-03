#pragma once

#include <vector>
#include "gl_headers.h"

class Demo; // demo.h, can't include here

class Ground {
  public:
    Ground(Demo &demo);
    ~Ground();
    void render();

  private:
    GLuint m_buffer;
    GLuint m_vertexArray;

    GLuint m_texDiffuse;
    GLuint m_texSpecular;

    Demo &m_demo;
};
