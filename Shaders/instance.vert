
#version 330 core //source code of vertex shader
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;
layout (location = 3) in mat4 instanceMatrix; // 实例的model矩阵

out vec2 TexCoord;
out vec3 FragPos;
out vec3 Normal;

uniform mat4 view;      // 观察矩阵
uniform mat4 projection;// 投影矩阵

void main(){
    gl_Position = projection * view * instanceMatrix * vec4(aPos, 1.0);
    TexCoord = aTexCoord;
    FragPos = vec3(instanceMatrix * vec4(aPos, 1.0f));
    // 需要将法向量转换到世界坐标， GPU做逆矩阵比较慢，建议在CPU中处理
    // 法矩阵：用来将法向量变换到世界坐标系
    // 法矩阵：model的逆的转置
    Normal = vec3(normalize(transpose(inverse(instanceMatrix)) * vec4(aNormal, 1.0f)).xyz);
}