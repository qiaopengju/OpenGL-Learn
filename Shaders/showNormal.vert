#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

out VS_OUT{
    vec3 normal;    // normal matrix * aNormal
    vec4 aPos;      // model * aPos
} vs_out;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main(){
    gl_Position = projection * view * model * vec4(aPos, 1.0);
    vs_out.normal = vec3(normalize(transpose(inverse(model)) * vec4(aNormal, 1.0)).xyz);
    vs_out.aPos = model * vec4(aPos, 1.0);
}