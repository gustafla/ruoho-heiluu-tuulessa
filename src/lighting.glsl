#version 330 core

out vec4 fragColor;

in vec2 v_texturePos;

uniform sampler2D u_tex0;
uniform sampler2D u_tex1;
uniform sampler2D u_tex2;

void main() {
  vec4 albedoSpecular = texture(u_tex2, v_texturePos);
  fragColor = albedoSpecular;
  //fragColor = vec4(texture(u_tex0, v_texturePos).rgb, 1.);
}
