#include "shader_quad.h"
#include "shaders.h"
#include "util.h"
#include <iostream>

std::string const vertexSource = "#version 330 core\n"
"layout (location = 0) in vec3 a_pos;\n"
"layout (location = 1) in vec2 a_texturePos;\n"
"out vec2 v_texturePos;\n"
"void main() {\n"
"  v_texturePos = a_texturePos;\n"
"  gl_Position = vec4(a_pos, 1.);\n"
"}\n";

ShaderQuad::ShaderQuad(std::string const &fragmentFilename) {
  GLfloat v[] = {
    -1., -1., 0., 0., 0.,
     1., -1., 0., 1., 0.,
     1.,  1., 0., 1., 1.,
     1.,  1., 0., 1., 1.,
    -1.,  1., 0., 0., 1.,
    -1., -1., 0., 0., 0.
  };

  // Gen buffer and VA objects
  glGenBuffers(1, &m_buffer);
  glBindBuffer(GL_ARRAY_BUFFER, m_buffer);
  glGenVertexArrays(1, &m_vertexArray);
  glBindVertexArray(m_vertexArray);

  // Load data
  glBufferData(GL_ARRAY_BUFFER, sizeof(v), v, GL_STATIC_DRAW);

  // "Record" to VAO which VBO is in use and how to configure rendering with it
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat),
      (void*)0);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat),
      (void*)(3*sizeof(GLfloat)));
  glEnableVertexAttribArray(1);

  // Link shader
  m_program = linkProgram(vertexSource, loadFile(fragmentFilename));
  if (!m_program) {
    std::cerr << fragmentFilename << " failed to link" << std::endl;
    die(EXIT_FAILURE);
  }

  // Cache ufm locations
  m_uTex0 = glGetUniformLocation(m_program, "u_tex0");
  m_uTex1 = glGetUniformLocation(m_program, "u_tex1");
  m_uTex2 = glGetUniformLocation(m_program, "u_tex2");
  m_uTex3 = glGetUniformLocation(m_program, "u_tex3");
}

ShaderQuad::~ShaderQuad() {
  glDeleteVertexArrays(1, &m_vertexArray);
  glDeleteBuffers(1, &m_buffer);
  glDeleteProgram(m_program);
}

void ShaderQuad::render() {
  glUseProgram(m_program);

  glUniform1i(m_uTex0, 0);
  glUniform1i(m_uTex1, 1);
  glUniform1i(m_uTex2, 2);
  glUniform1i(m_uTex3, 3);

  glBindVertexArray(m_vertexArray);

  glDrawArrays(GL_TRIANGLES, 0, 6);

  glBindVertexArray(0);
}

void ShaderQuad::setUniform(std::string name, int n, GLfloat v[4]) {
  glUseProgram(m_program);
  GLint ufm = glGetUniformLocation(m_program, name.c_str());

  switch(n) {
    case 1: glUniform1fv(ufm, 1, v); break;
    case 2: glUniform2fv(ufm, 1, v); break;
    case 3: glUniform3fv(ufm, 1, v); break;
    case 4: glUniform4fv(ufm, 1, v); break;
    default: break;
  }
}
