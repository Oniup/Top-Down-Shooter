#version 450 core

out vec4 frag_colour;

in vec2 _uv;

// TODO: for the batch renderer, the sampler2D needs to be an array of textures
uniform sampler2D u_textures;
// uniform int u_textures_count;

#define NULL 0;

void main() {
  vec4 colour = vec4(1.0, 0.0, 1.0, 1.0);
  colour = texture(u_textures, _uv.xy);
  frag_colour = colour;
}

