#version 330 core

out vec4 fragColor;

in vec2 v_texturePos;

uniform sampler2D u_tex0;
uniform sampler2D u_tex1;
uniform sampler2D u_tex2;
uniform sampler2D u_tex3;

uniform vec3 u_cameraPos;
uniform vec3 u_cameraTarget;

vec3 post(vec3 color, vec3 noise) {
  color = color * (1. - distance(v_texturePos*.5, vec2(.25))); // vignette
  return color + noise * 0.1; // add noise
}

float ssao(vec3 pos) {
  float occlusion = 0.;
  for (int i=0; i<32; i++) {
    vec2 ofsUv = vec2(float(i)/32.);
    vec2 ofs = texture(u_tex3, ofsUv).xz * 2. -1.;

    vec2 sampleUv = v_texturePos + ofs * 0.01;
    occlusion += distance(texture(u_tex0, sampleUv).xyz, pos);
  }
  return 1. - (occlusion / 64.);
}

vec3 light(vec3 pos, vec3 normal, vec4 albedoSpecular) {
  vec3 viewDirection = normalize(u_cameraPos - pos);

  vec3 lighting = albedoSpecular.rgb * 0.8; // Ambient

  // Just light from the cam
  float l = max(dot(normal, viewDirection), 0.);
  lighting += l * albedoSpecular.rgb;
  return lighting + pow(l, 5.) * albedoSpecular.a;
}

void main() {
  vec3 pos = texture(u_tex0, v_texturePos).xyz;
  vec3 normal = texture(u_tex1, v_texturePos).xyz;
  vec4 albedoSpecular = texture(u_tex2, v_texturePos);
  vec3 noise = texture(u_tex3, v_texturePos).rgb;

  vec3 color = post(light(pos, normal, albedoSpecular), noise);
  fragColor = vec4(min(color, 1.) * ssao(pos), 1.);
}
