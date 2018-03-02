#version 330 core

out vec4 fragColor;

in vec2 v_texture_pos;

uniform sampler2D tex0;
uniform sampler2D tex1;
uniform sampler2D tex2;

void main() {
  vec4 albedo_specular = texture(tex1, v_texture_pos);
  fragColor = vec4(albedo_specular.rgb, 1.);
}
