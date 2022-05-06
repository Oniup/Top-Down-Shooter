#version 450 core


layout (location = 0) in vec3   a_vertex_pos;
layout (location = 1) in vec2   a_uv;
layout (location = 2) in vec4   a_overlay_colour;
layout (location = 3) in float  a_texture_id;


out vec2 _uv;
out vec4 _overlay_colour;
out float _texture_id;


uniform mat4 u_projection;
uniform mat4 u_transform;




void main() {
  gl_Position = u_projection * u_transform * vec4(a_vertex_pos.xyz, 1.0);
  // gl_Position = vec4(a_vertex_pos.xyz, 1.0);
  // TODO: implement camera into engine so we can have a view matrix

  // passing them to the fragment shader
  _uv = a_uv;
  _overlay_colour = a_overlay_colour;
  _texture_id = a_texture_id;
}