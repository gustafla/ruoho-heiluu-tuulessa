#version 330 core

layout (location = 0) in vec3 a_pos;
layout (location = 1) in vec3 a_normal;
layout (location = 2) in vec2 a_texture_pos;

uniform mat4 u_mvp;

out vec3 v_pos;
out vec3 v_normal;
out vec2 v_texture_pos;

void main() {
  vec4 pos = u_mvp * vec4(a_pos, 1.); // Translate and project
  gl_Position = pos; // Output

  // Varyings
  v_pos = pos.xyz;
  v_normal = (u_mvp * vec4(a_normal, 0.)).xyz; // ? :D
  v_texture_pos = a_texture_pos;
}
