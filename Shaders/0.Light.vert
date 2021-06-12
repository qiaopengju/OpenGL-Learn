#version 330 core //source code of vertex shader
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;
layout (location = 3) in vec3 aTangent;
layout (location = 4) in vec3 aBitangent;

out VS_OUT{
    vec2 TexCoord;
    vec3 FragPos;
    mat3 TBN;
    vec4 FragPosLightSpace;
} vs_out;

uniform mat4 model;     // 模型矩阵
uniform mat4 view;      // 观察矩阵
uniform mat4 projection;// 投影矩阵
uniform mat4 depthMatrix;

void main(){
    gl_Position = projection * view * model * vec4(aPos, 1.0);
    vs_out.TexCoord = aTexCoord;
    vs_out.FragPos = vec3(model * vec4(aPos, 1.0f));
    // 需要将法向量转换到世界坐标， GPU做逆矩阵比较慢，建议在CPU中处理
    // 法矩阵：用来将法向量变换到世界坐标系
    // 法矩阵：model的逆的转置
    mat4 normalMatrix = transpose(inverse(model));
    vec3 T = vec3(normalize(normalMatrix * vec4(aTangent, 0)));
    vec3 B = vec3(normalize(normalMatrix * vec4(aBitangent, 0)));
    vec3 N = vec3(normalize(normalMatrix * vec4(aNormal, 0)));
    vs_out.TBN = mat3(T, B, N);
    vs_out.FragPosLightSpace = depthMatrix * model * vec4(aPos, 1.0);
}