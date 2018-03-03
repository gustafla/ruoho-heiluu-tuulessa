#include "procedural_texture.h"
#include "shader_quad.h"

GLuint renderProceduralTexture(int w, int h, std::string fragmentFilename,
    Framebuffer::TexFormats formats) {

  // Store VP
  GLint v[4];
  glGetIntegerv(GL_VIEWPORT, v);

  // Set VP
  glViewport(0, 0, w, h);

  // Init rendering objects
  Framebuffer fb(w, h, {formats});
  ShaderQuad s(fragmentFilename);

  // Render
  fb.bind();
  s.render();

  // Restore VP
  glViewport(v[0], v[1], v[2], v[3]);

  // Return texture ID and mark for keeping
  return fb.keepTexture();
}
