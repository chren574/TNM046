#version 330 core


in vec3 interpolatedNormal;
in vec2 st;

out vec4 finalcolor;

uniform mat4 T;


void main() {

    //View direction
    vec3 V = vec3(0.0,0.0,1.0);
    //Normal
    vec3 N = interpolatedNormal;
    //Light direction
    vec3 L = mat3(T)*normalize(vec3(1.0, 1.0, 1.0));

    //shininess parameter
    float n = 50;
    //Ambient replection color
    vec3 ka = vec3(1.0,0.5,0.0);
    //Ambient illumination color
    vec3 Ia = vec3(0.2,0.2,0.2);

    //Diffuse surface reflection color
    vec3 kd = vec3(1.0,0.5,0.0);
    //Diffuse illumination color
    vec3 Id = vec3(0.8,0.8,0.8);

    //Diffuse specular surface reflection color
    vec3 ks = vec3(1.0,1.0,1.0);
     //Diffuse specular illumination color
    vec3 Is = vec3(1.0,1.0,1.0);


    vec3 R = 2.0*dot(N,L)*N -L;
    float dotNL = max(dot(N,L), 0.0);
    float dotRV = max(dot(R,V), 0.0);
    vec3 shadedcolor = Ia*ka + Id*kd*dotNL + Is*ks*pow(dotRV, n);

    finalcolor = vec4(shadedcolor, 1.0);

}
