#include "shaders.h"
#include <iostream>

GLuint compileShader(std::string src, GLenum type) {
  GLuint shader = glCreateShader(type);
  GLchar const *src_cstr = src.c_str();
  glShaderSource(shader, 1, &src_cstr, nullptr);
  glCompileShader(shader);

  int success;
  char infoLog[512];
  glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
  if(!success) {
    glGetShaderInfoLog(shader, 512, NULL, infoLog);
    std::cerr << infoLog << std::endl;
    glDeleteShader(shader); // free failed shader
    return 0; //glCreateShader returns 0 if an error occurs.
  }

  return shader;
}

GLuint linkProgram(std::string vertexSrc, std::string fragmentSrc) {
  GLuint vs, fs, sp = 0;
  vs = compileShader(vertexSrc, GL_VERTEX_SHADER);
  fs = compileShader(vertexSrc, GL_FRAGMENT_SHADER);

  if (vs && fs) { // Both have been nicely compiled
    sp = glCreateProgram();
    glAttachShader(sp, vs);
    glAttachShader(sp, fs);
    glLinkProgram(sp);

    int success;
    char infoLog[512];
    glGetProgramiv(sp, GL_LINK_STATUS, &success);
    if(!success) {
      glGetProgramInfoLog(sp, 512, NULL, infoLog);
      std::cerr << infoLog << std::endl;
      glDeleteProgram(sp);
      sp = 0;
    }
  }

  glDeleteShader(vs); // A value of 0 for shader will be silently ignored
  glDeleteShader(fs); // So calling glDeleteShader is pretty worry-free

  return sp;
}
