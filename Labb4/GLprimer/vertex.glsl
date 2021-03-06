#version 330 core

uniform float time;
uniform mat4 MV;
uniform mat4 P;

layout(location = 0) in vec3 Position;
layout(location = 1) in vec3 Normal;
//layout(location = 2) in vec2 TexCoord;

out vec3 interpolatedNormal;


void main() {


   vec3 transformedNormal = mat3(MV) * Normal;
   interpolatedNormal = normalize(transformedNormal);

    gl_Position = MV*vec4(Position, 1.0);


}


