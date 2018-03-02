#version 330 core

out vec4 fragColor;

in vec2 v_texture_pos;

uniform sampler2D u_tex0;
uniform sampler2D u_tex1;
uniform sampler2D u_tex2;

void main() {
  //vec4 albedo_specular = texture(u_tex2, v_texture_pos);
  //fragColor = albedo_specular;
  fragColor = vec4(texture(u_tex2, v_texture_pos).rgb, 1.);
}
