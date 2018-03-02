#pragma once

#include "gl_headers.h"
#include <string>

class ShaderQuad {
  public:
    ShaderQuad(std::string const &fragmentFilename);
    ~ShaderQuad();
    void render();

  private:
    GLuint m_program;
    GLuint m_buffer;
    GLuint m_vertexArray;

    // ufms
    GLint m_uTex0;
    GLint m_uTex1;
    GLint m_uTex2;
};
