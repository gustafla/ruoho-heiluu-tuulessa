#version 330 core

layout (location = 0) in vec3 a_pos;
layout (location = 1) in vec2 a_texturePos;
layout (location = 2) in vec3 a_normal;

uniform mat4 u_model;
uniform mat4 u_view;
uniform mat4 u_projection;

out vec3 v_pos; // In world space
out vec3 v_normal;
out vec2 v_texturePos;

void main() {
  vec4 world = u_model * vec4(a_pos, 1.);

  // Varyings
  v_pos = world.xyz;
  v_normal = (u_model * vec4(a_normal, 0.)).xyz; // Rotate
  v_texturePos = a_texturePos;

  // Output
  gl_Position = u_projection * u_view * world;
}
