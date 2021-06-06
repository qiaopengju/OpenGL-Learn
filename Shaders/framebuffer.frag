#version 330 // source code of fragment shader
out vec4 FragColor;
in vec2 TexCoord;

uniform sampler2D frameTexture; //采样器
uniform sampler2D frameBuffer;

uniform float width;
uniform float height;

const float offset = 1.0 / 300.0;
vec2 offsets[9] = vec2[](
    vec2(-offset, -offset), // left top
    vec2(   0.0f,  offset), // top
    vec2( offset,  offset), // right top
    vec2(-offset,  0.0f),   // left
    vec2(   0.0f,  0.0f),   // center
    vec2( offset,  0.0f),   // right
    vec2(-offset, -offset), // left bottom
    vec2(   0.0f, -offset), // bottom 
    vec2( offset, -offset) // left bottom
);
float kernel[9]; // 核

vec4 cacuKernel(); // 计算核颜色

void main(){
    if (gl_FragCoord.x < width / 3){
        if (gl_FragCoord.y < height / 2){ // 左下，反色
            FragColor = vec4(1.0) - texture(frameTexture, TexCoord);
        } else{ // 左上，灰度
            FragColor = texture(frameTexture, TexCoord);
            float average = 0.2126 * FragColor.r + 0.7152 * FragColor.g + 0.0722 * FragColor.b;
            FragColor = vec4(vec3(average), 1.0);
        }
    } else if (gl_FragCoord.x > width / 3 * 2){
        if (gl_FragCoord.y < height / 2){ // 右下，锐化
            kernel = float[](
                -1, -1, -1,
                -1,  9, -1,
                -1, -1, -1
            );
        } else{ // 右上，模糊
            kernel = float[](
                1.0/16, 2.0/16, 1.0/16,
                2.0/16, 4.0/16, 2.0/16,
                1.0/16, 2.0/16, 1.0/16
            );
        }
        FragColor = cacuKernel();
    } else{
        FragColor = texture(frameBuffer, TexCoord);
        if (gl_FragCoord.y < height / 2){ // 正下方，正常
            FragColor = texture(frameBuffer, TexCoord);
        } else{ // 正上方，边缘检测
            kernel = float[](
                1,  1, 1,
                1, -8, 1,
                1,  1, 1
            );
            FragColor = cacuKernel();
        }
    }
}

vec4 cacuKernel(){ // 计算核颜色
    vec3 sampleTex[9];
    for (int i = 0; i < 9; i++) // 取样周围颜色
        sampleTex[i] = vec3(texture(frameBuffer, TexCoord.st + offsets[i])); 
    vec3 result = vec3(0.0);
    for (int i = 0; i < 9; i++)
        result += sampleTex[i] * kernel[i];
    return vec4(result, 1.0);
}