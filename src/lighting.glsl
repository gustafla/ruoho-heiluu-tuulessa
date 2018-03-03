#version 330 core

out vec4 fragColor;

in vec2 v_texturePos;

uniform sampler2D u_tex0;
uniform sampler2D u_tex1;
uniform sampler2D u_tex2;
uniform sampler2D u_tex3;

uniform vec3 u_cameraPos;
uniform vec3 u_cameraTarget;

vec3 post(vec3 color) {
  color = color * (1. - distance(v_texturePos, vec2(0.5))); // vignette
  return color + texture(u_tex3, v_texturePos).rgb*0.1; // add noise
}

float light(float spec) {
  vec3 normal = texture(u_tex1, v_texturePos).rgb;
  vec3 lightDirection = normalize(u_cameraTarget - u_cameraPos);
  // Just light from the cam
  return dot(-normal, lightDirection);
}

void main() {
  vec4 albedoSpecular = texture(u_tex2, v_texturePos);
  fragColor = vec4(post(albedoSpecular.rgb * light(albedoSpecular.a)), 1.);
}
