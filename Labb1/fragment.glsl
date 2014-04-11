#version 330 core

uniform float time;

in vec3 interpolatedColor;
out vec4 finalcolor;

void main() {
    finalcolor = vec4(interpolatedColor*abs(sin(time)), 1.0);
    //finalcolor = vec4(1.0, 0.5, 6.0, 1.0);
}
