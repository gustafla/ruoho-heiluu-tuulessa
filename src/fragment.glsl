#version 330 core

layout (location = 0) out vec3 g_position;
layout (location = 1) out vec3 g_normal;
layout (location = 2) out vec4 g_albedo_specular;

in vec2 v_texture_pos;
in vec3 v_pos;
in vec3 v_normal;

uniform sampler2D u_tex_diffuse;
uniform sampler2D u_tex_specular;

void main() {
  g_position = v_pos;
  g_normal = normalize(v_normal);
  g_albedo_specular.rgb = texture(u_tex_diffuse, v_texture_pos).rgb;
  g_albedo_specular.a = texture(u_tex_specular, v_texture_pos).r;
}
