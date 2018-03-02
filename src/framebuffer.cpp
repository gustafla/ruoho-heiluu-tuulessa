#include "framebuffer.h"
#include "util.h"
#include <iterator>
#include <iostream>

Framebuffer::Framebuffer(int w, int h):
  Framebuffer(w, h, {{GL_RGBA, GL_RGBA, GL_UNSIGNED_BYTE}})
{}

Framebuffer::Framebuffer(int w, int h,
    std::initializer_list<TexFormats> formats):
  m_nTextures(formats.size()), m_keepTextures({false})
{

  if (m_nTextures > 8) {
    std::cerr << "Requested too complex FBO" << std::endl;
    die(EXIT_FAILURE);
  }

  GLuint const att[8] = {
    GL_COLOR_ATTACHMENT0,
    GL_COLOR_ATTACHMENT1,
    GL_COLOR_ATTACHMENT2,
    GL_COLOR_ATTACHMENT3,
    GL_COLOR_ATTACHMENT4,
    GL_COLOR_ATTACHMENT5,
    GL_COLOR_ATTACHMENT6,
    GL_COLOR_ATTACHMENT7
  };

  glGenFramebuffers(1, &m_frameBuffer);
  glBindFramebuffer(GL_FRAMEBUFFER, m_frameBuffer);
  glGenTextures(m_nTextures, m_textures);

  // Color attachments / textures
  for (std::initializer_list<TexFormats>::iterator i = formats.begin();
      i != formats.end(); i++) {

    int n = std::distance(formats.begin(), i);
    glBindTexture(GL_TEXTURE_2D, m_textures[n]);
    glTexImage2D(GL_TEXTURE_2D, 0, i->internalFormat, w, h, 0, i->format,
        i->type, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_FRAMEBUFFER, att[n], GL_TEXTURE_2D,
        m_textures[n], 0);
  }

  // Gen and add RBO
  glGenRenderbuffers(1, &m_renderBuffer);
  glBindRenderbuffer(GL_RENDERBUFFER, m_renderBuffer);
  glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, w, h);
  glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT,
      GL_RENDERBUFFER, m_renderBuffer);

  // Check for completeness
  if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
    std::cerr << "FBO not complete" << std::endl;
    die(EXIT_FAILURE);
  }

  // FB's attachments for rendering
  glDrawBuffers(m_nTextures, att); // part of the FBO state
}

Framebuffer::~Framebuffer() {
  glDeleteFramebuffers(1, &m_frameBuffer);
  glDeleteRenderbuffers(1, &m_renderBuffer);
  for (int i=0; i<m_nTextures; i++) {
    if (!m_keepTextures[i]) {
      glDeleteTextures(1, &m_textures[i]);
    }
  }
}

void Framebuffer::bindTextures() {
  for (int i=0; i<m_nTextures; i++) {
    glActiveTexture(GL_TEXTURE0 + i);
    glBindTexture(GL_TEXTURE_2D, m_textures[i]);
  }
}

void Framebuffer::bind() {
  glBindFramebuffer(GL_FRAMEBUFFER, m_frameBuffer);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

GLuint Framebuffer::texture(int i) {
  return m_textures[i];
}

void Framebuffer::keepTexture(int i) {
  m_keepTextures[i] = true;
}
