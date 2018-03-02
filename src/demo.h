#pragma once

#include "sync.h" // rocket
#include "music_player.h"
#include <unordered_map>
#include "gl_headers.h"
#include "shader_quad.h"

class Grass; // grass.h

class Demo {
  public:
    Demo(sync_device *rocket, MusicPlayer const &player, int w, int h);
    ~Demo();
    double get(std::string track);
    GLfloat const *projection();
    GLfloat const *view();
    void render();
    GLint getUniformLocation(std::string const &name);

  private:
    sync_device *m_rocket;
    MusicPlayer const &m_player;
    double m_t;
    glm::mat4 m_projection;
    glm::mat4 m_view;
    // Likely faster to cache tracks to a map as sync_get_track only iterates.
    std::unordered_map<std::string, sync_track const *> m_tracks;

    // G-Buffer
    GLuint m_gBufferShaderProgram;
    GLuint m_gBuffer;
    GLuint m_gBufferRenderBuffer;
    GLuint m_gBufferTextures[3];

    // Post buffer
    GLuint m_postBuffer;
    GLuint m_postBufferRenderBuffer;
    GLuint m_postBufferTexture;

    // Lighting and post pass
    ShaderQuad m_lightingPass;
    ShaderQuad m_postPass;

    // Visual components
    Grass *m_grass;
};
