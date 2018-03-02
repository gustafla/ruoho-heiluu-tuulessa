#version 330 core

layout (location = 0) out vec3 gPosition;
layout (location = 1) out vec3 gNormal;
layout (location = 2) out vec4 gAlbedoSpecular;

in vec3 v_pos;
in vec3 v_normal;
in vec2 v_texture_pos;

uniform sampler2D u_tex_diffuse;
uniform sampler2D u_tex_specular;

void main() {
  gPosition = v_pos;
  gNormal = normalize(v_normal);
  //gAlbedoSpecular.rgb = texture(u_tex_diffuse, v_texture_pos).rgb;
  //gAlbedoSpecular.a = texture(u_tex_specular, v_texture_pos).r;
  gAlbedoSpecular = vec4(1., 0., 1., 1.);
}
