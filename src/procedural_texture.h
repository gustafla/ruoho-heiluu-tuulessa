#pragma once

#include "gl_headers.h"
#include "framebuffer.h"
#include <string>

GLuint renderProceduralTexture(int w, int h, std::string fragmentFilename,
    Framebuffer::TexFormats formats = {GL_RGBA, GL_RGBA, GL_UNSIGNED_BYTE});
