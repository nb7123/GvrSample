#version 300 es

layout (location = 0) in vec3 pos;
layout (location = 1) in vec2 t_coord;

out vec2 tex_coord;

uniform mat4 u_model;
uniform mat4 u_view;
uniform mat4 u_projection;

void main() {
    gl_Position = u_projection * u_view * u_model * vec4(pos, 1.0f);
    tex_coord = vec2(t_coord.x, 1.0f-t_coord.y);
}
