#version 330 core //source code of vertex shader
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aTexCoord;

out vec3 vertexColor;   // 向片段着色器输出颜色
out vec2 TexCoord;

uniform mat4 rotate;    // 旋转变换
uniform mat4 translate;      // 平移变换

void main(){
    //gl_Position = rotate * translate * vec4(aPos, 1.0);
    gl_Position = translate * rotate * vec4(aPos, 1.0);
    vertexColor = aColor;
    TexCoord = aTexCoord;
}