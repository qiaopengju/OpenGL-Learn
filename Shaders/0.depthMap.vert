#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;
layout (location = 3) in vec3 aTangent;
layout (location = 4) in vec3 aBitangent;

uniform mat4 model;     // 模型矩阵
uniform mat4 depthMatrix;

void main(){
    gl_Position = depthMatrix * model * vec4(aPos, 1.0);
}