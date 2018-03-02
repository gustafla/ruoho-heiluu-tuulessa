#version 330 core

layout (location = 0) out vec3 gPosition;
layout (location = 1) out vec3 gNormal;
layout (location = 2) out vec4 gAlbedoSpecular;

in vec3 v_pos;
in vec3 v_normal;
in vec2 v_texturePos;

uniform sampler2D u_texDiffuse;
uniform sampler2D u_texSpecular;

void main() {
  gPosition = v_pos;
  gNormal = normalize(v_normal);
  gAlbedoSpecular.rgb = texture(u_texDiffuse, v_texturePos).rgb;
  gAlbedoSpecular.a = texture(u_texSpecular, v_texturePos).r;
}
