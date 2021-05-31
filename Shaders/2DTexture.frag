#version 330 // source code of fragment shader
out vec4 FragColor;
in vec3 vertexColor;
in vec2 TexCoord;

uniform sampler2D texture1; //采样器1
uniform sampler2D texture2; //采样器2

void main(){
    // 混合两个材质的颜色，texture1占80%
    FragColor = mix(texture(texture1, TexCoord), texture(texture2, TexCoord), 0.2);
    /* 左右翻转
    FragColor = texture(texture1, vec2(-TexCoord.s, TexCoord.t));
    */
}