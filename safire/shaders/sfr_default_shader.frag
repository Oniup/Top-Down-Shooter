#version 450 core

#define NULL 0;
#define MAX_TEXTURE_COUNT 32


out vec4 frag_colour;


in vec2 _uv;
in vec4 _overlay_colour;
in float _texture_id;


// TODO: for the batch renderer, the sampler2D needs to be an array of textures
uniform sampler2D u_textures[MAX_TEXTURE_COUNT];
uniform int u_textures_count;




void main() 
{
  vec4 colour = vec4(1.0, 0.0, 1.0, 1.0);
  colour = texture(u_textures[int(_texture_id)], _uv.xy);

  // checking if there is any opasity 
  if (colour.a < 0.01) 
    discard;

  // if there is any overlay colour, then add it
  if (_overlay_colour.a > 0.05) 
  {
    vec3 overlay = vec3(_overlay_colour.rgb / _overlay_colour.a);
    colour = colour + vec4(overlay.rgb, 0.0);
  }

  frag_colour = colour;
}
