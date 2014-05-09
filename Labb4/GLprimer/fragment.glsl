#version 330 core


out vec4 finalcolor;

in vec3 interpolatedNormal;



void main() {

    finalcolor = vec4(vec3(interpolatedNormal.z), 1.0);

}
