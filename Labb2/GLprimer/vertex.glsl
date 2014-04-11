#version 330 core

uniform float time;
uniform mat4 T;
uniform mat4 R;
uniform mat4 M;

layout(location = 0) in vec3 Position;

layout(location = 1) in vec3 Color;
out vec3 interpolatedColor;

void main() {
    //gl_Position = vec4(Position*sin(time), 1.0);
    gl_Position = M * vec4(Position, 1.0);
    interpolatedColor = Color;
}


