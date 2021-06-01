#version 330 core //source code of vertex shader
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;

out vec2 TexCoord;

uniform mat4 model;     // 模型矩阵
uniform mat4 view;      // 观察矩阵
uniform mat4 projection;// 投影矩阵

uniform mat4 rotate;
uniform mat4 translate;

void main(){
    gl_Position = projection * view * model * translate * rotate * vec4(aPos, 1.0);
    //gl_Position = projection * view * model * rotate * vec4(aPos, 1.0);
    TexCoord = aTexCoord;
}