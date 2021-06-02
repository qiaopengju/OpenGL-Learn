#version 330

out vec4 FragColor;

in vec3 FragPos;    // 点在世界坐标
in vec2 TexCoord;

uniform sampler2D texturePlane;

uniform vec3 lightPosition;
uniform vec3 colorLight;
uniform float ambientStrength;

void main(){
    vec3 ambient = ambientStrength * colorLight;
    vec3 Normal = vec3(0.0f, 1.0f, 0.0f);
    vec3 result = vec3(texture(texturePlane, TexCoord).rgb);
    float diff = dot(Normal, normalize(lightPosition - FragPos));
    vec3 diffuse = diff * colorLight;
    result = (ambientStrength + diffuse) * result * colorLight;
    //FragColor = vec4(result, 1.0f);
    FragColor = texture(texturePlane, TexCoord);
}