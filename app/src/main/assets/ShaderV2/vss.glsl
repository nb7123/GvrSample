#version 100

attribute vec3 pos;

uniform mat4 u_model;
uniform mat4 u_view;
uniform mat4 u_projection;

void main() {
    gl_Position = vec4(pos, 1.0);
}