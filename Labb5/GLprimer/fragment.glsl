#version 330 core

uniform sampler2D tex;

out vec4 finalcolor;

in vec2 st;


void main() {

    finalcolor = texture(tex, st);

}
