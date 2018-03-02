#version 330 core

out vec3 fragColor;

in vec2 v_texturePos;

void main() {
  vec3 color = vec3(0.3, 0.4, 0.33);
  fragColor = color * (sin(v_texturePos.x*15.)*0.2 +0.7);
}
