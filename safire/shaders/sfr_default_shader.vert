#version 450 core

layout (location = 0) in vec3 a_vertex_pos;
layout (location = 1) in vec2 a_uv;

out vec2 _uv;

// uniform mat4 u_view;
uniform mat4 u_projection;
uniform mat4 u_transform;

void main() {
  gl_Position = u_projection * u_transform * vec4(a_vertex_pos.xyz, 1.0);
  // gl_Position = u_view * u_projection * u_transform * vec4(a_vertex_pos.xy, 0.0, 1.0);  

  _uv = a_uv;
}
