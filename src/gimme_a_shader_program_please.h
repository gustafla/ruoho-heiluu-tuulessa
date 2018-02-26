#pragma once

#include "GL/glew.h"
#include <string>

GLuint gimme_a_shader_program_please(std::string vertexSrc,
    std::string fragmentSrc);
