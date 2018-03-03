#version 330 core

out float fragColor;

in vec2 v_texturePos;

void main() {
  fragColor = sin(v_texturePos.x*15.)*0.2+0.8;
}
