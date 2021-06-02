#version 330 core //source code of vertex shader
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 nNormal; // 法向量

out vec3 Normal; // 法向量输出给片段着色器
out vec3 FragPos;// 世界坐标系中的位置

uniform mat4 model;     // 模型矩阵
uniform mat4 view;      // 观察矩阵
uniform mat4 projection;// 投影矩阵

void main(){
    gl_Position = projection * view * model * vec4(aPos, 1.0);
    // 需要将法向量转换到世界坐标， GPU做逆矩阵比较慢，建议在CPU中处理
    // 法矩阵：用来将法向量变换到世界坐标系
    // 法矩阵：model的逆的转置
    Normal = vec3(normalize(transpose(inverse(model)) * vec4(nNormal, 1.0f)).xyz);
    FragPos = vec3(model * vec4(aPos, 1));
}