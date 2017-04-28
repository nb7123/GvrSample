#version 300 es
precision mediump float;

in vec2 tex_coord;

uniform sampler2D tex0;

out vec4 color;

void main() {
    color = texture(tex0, tex_coord);
}
