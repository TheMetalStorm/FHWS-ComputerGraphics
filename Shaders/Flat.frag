#version 330


in vec2 texCord;

in vec4 vertex_color;
uniform sampler2D tex;

void main() {
    gl_FragColor = vertex_color;
}
