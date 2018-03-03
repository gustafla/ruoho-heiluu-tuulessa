#pragma once

#include "gl_headers.h"
#include <initializer_list>

class Framebuffer {
  public:

    struct TexFormats {
      GLint internalFormat;
      GLenum format;
      GLenum type;
    };

    Framebuffer(int w, int h);
    Framebuffer(int w, int h, std::initializer_list<TexFormats> formats);
    ~Framebuffer();
    void bindTextures();
    void bind();
    GLuint keepTexture(int i=0);

  private:
    int m_nTextures;
    GLuint m_frameBuffer;
    GLuint m_renderBuffer;
    GLuint m_textures[8];
    bool m_keepTextures[8]; // Because might need after destruction
};
