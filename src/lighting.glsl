#version 330 core

out vec4 fragColor;

in vec2 v_texturePos;

uniform sampler2D u_tex0;
uniform sampler2D u_tex1;
uniform sampler2D u_tex2;
uniform sampler2D u_tex3;

vec3 post(vec3 color) {
  return color + texture(u_tex3, v_texturePos).rgb*0.1; // add noise
}

void main() {
  vec4 albedoSpecular = texture(u_tex2, v_texturePos);
  fragColor = vec4(post(albedoSpecular.rgb), 1.);
}
