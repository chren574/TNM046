#version 330 core

uniform float time;
uniform mat4 T;

layout(location = 0) in vec3 Position;
layout(location = 1) in vec3 Normal;
layout(location = 2) in vec2 TexCoord;

out vec3 interpolatedNormal;
//out vec2 st;
//out vec3 shadedcolor;



void main() {

    gl_Position = vec4(Position, 1.0);


    vec3 transformedNormal = Normal;
    interpolatedNormal = normalize(transformedNormal);






//    vec3 transformedNormal = mat3(T) * Normal;
//    interpolatedNormal = normalize(transformedNormal);
//
//
//    gl_Position = vec4(Position, 1.0);
////    interpolatedNormal = Normal;
//    st = TexCoord;
}


