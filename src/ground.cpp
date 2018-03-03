#include "ground.h"
#include "demo.h"
#include "util.h"
#include "shaders.h"
#include "gl_headers.h"
#include "procedural_texture.h"

Ground::Ground(Demo &demo):
  m_texDiffuse(renderProceduralTexture(1, 1, "ground_diffuse.glsl",
      {GL_RGB, GL_RGB, GL_UNSIGNED_BYTE})),
  m_texSpecular(renderProceduralTexture(1, 1, "ground_specular.glsl",
      {GL_R8, GL_RED, GL_UNSIGNED_BYTE})),
  m_demo(demo)
{
  GLfloat v[] = {
    -400., 0.,  400., 0., 0., 0., 1., 0.,
     400., 0.,  400., 1., 0., 0., 1., 0.,
     400., 0., -400., 1., 1., 0., 1., 0.,
     400., 0., -400., 1., 1., 0., 1., 0.,
    -400., 0., -400., 0., 1., 0., 1., 0.,
    -400., 0.,  400., 0., 0., 0., 1., 0.
  };

  // Gen buffer and VA objects
  glGenBuffers(1, &m_buffer);
  glBindBuffer(GL_ARRAY_BUFFER, m_buffer);
  glGenVertexArrays(1, &m_vertexArray);
  glBindVertexArray(m_vertexArray);

  // Load data
  glBufferData(GL_ARRAY_BUFFER, sizeof(v), v, GL_STATIC_DRAW);

  // "Record" to VAO which VBO is in use and how to configure rendering with it
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat),
      (void*)0);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat),
      (void*)(3*sizeof(GLfloat)));
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat),
      (void*)(5*sizeof(GLfloat)));
  glEnableVertexAttribArray(2);
}

Ground::~Ground() {
  glDeleteVertexArrays(1, &m_vertexArray);
  glDeleteBuffers(1, &m_buffer);
  glDeleteTextures(1, &m_texDiffuse);
  glDeleteTextures(1, &m_texSpecular);
}

void Ground::render() {
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, m_texDiffuse);
  glActiveTexture(GL_TEXTURE1);
  glBindTexture(GL_TEXTURE_2D, m_texSpecular);

  glBindVertexArray(m_vertexArray);

  // Model matrix
  GLuint uModel = m_demo.getUniformLocation("u_model");
  glm::mat4 model = glm::mat4(1.); // No tfm needed
  glUniformMatrix4fv(uModel, 1, GL_FALSE, glm::value_ptr(model));

  // Draw
  glDrawArrays(GL_TRIANGLES, 0, 6);

  glBindVertexArray(0);
}
