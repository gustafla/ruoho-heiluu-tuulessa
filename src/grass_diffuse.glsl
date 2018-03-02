#version 330 core

out vec3 fragColor;

in vec2 v_texturePos;

void main() {
  vec3 color = vec3(0.3, 0.9, 0.2);
  fragColor = color;
}
