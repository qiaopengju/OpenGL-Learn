#version 330
#define MAX_POINT_LIGHTS 20
#define MAX_SPOT_LIGHTS 1

struct Material{
    //vec3 diffuse;  // 漫反射贴图, 最终的颜色
    //vec3 specular; // 镜面反射贴图, 最终的颜色
    sampler2D texture_diffuse1;  // 漫反射贴图
    //sampler2D texture_diffuse2;  // 漫反射贴图
    //sampler2D texture_diffuse3;  // 漫反射贴图
    sampler2D texture_specular1; // 镜面反射贴图
    //sampler2D texutre_specular2; // 镜面反射贴图
    //sampler2D texutre_specular3; // 镜面反射贴图
    int shininess;
};
struct DirectionalLight{    // 平行光
    vec3 direction; 

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};
struct PointLight{          // 点光源
    vec3 position;

    // 光线衰减参数
    float constant;
    float linear;
    float quadratic;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};
struct SpotLight{           //聚光灯
    vec3 position;
    vec3 direction;
    float cutOff;
    float outerCutOff;
    // 光线衰减参数
    float constant;
    float linear;
    float quadratic;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};
vec3 CalcDirLight(DirectionalLight light, vec3 viewDir);
vec3 CalcPointLight(PointLight light, vec3 viewDir);
vec3 CalcSpotLight(SpotLight light, vec3 viewDir);

in vec3 Normal;     // 法向量
in vec2 TexCoord;   // 贴图坐标
in vec3 FragPos;    // 点在世界坐标系下坐标
out vec4 FragColor;

uniform vec3 viewPosition;

uniform Material material;                
uniform DirectionalLight directionalLight;
uniform int pointLightNum;
uniform PointLight pointLight[MAX_POINT_LIGHTS];
//uniform int spotLightNum;
//uniform SpotLight spotLight[MAX_SPOT_LIGHTS];
uniform SpotLight spotLight;
uniform bool showDirLight, showPointLight, showSpotLight;

void main(){
    vec3 result = vec3(0.f);
    vec3 viewDir = normalize(viewPosition - FragPos);
    // caculate Directional light
    if (showDirLight) result += CalcDirLight(directionalLight, viewDir);
    // caculate Point light
    if (showPointLight){
        for (int i = 0; i < pointLightNum; i++){
            result += CalcPointLight(pointLight[i], viewDir);
        }
    }
    // caculate Spot light
    if (showSpotLight) result += CalcSpotLight(spotLight, viewDir);

    FragColor = vec4(result, 1.0f);
}

vec3 CalcDirLight(DirectionalLight light, vec3 viewDir){
    vec3 lightDir = normalize(-light.direction);
    // diffuse
    float diff = max(dot(Normal, lightDir), 0.0f);
    // specular
    vec3 relfectDir = reflect(-lightDir, Normal);
    float spec = pow(max(dot(relfectDir, viewDir), 0.0f), material.shininess);
    // result
    vec3 ambient = light.ambient * vec3(texture(material.texture_diffuse1, TexCoord));
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.texture_diffuse1, TexCoord));
    vec3 specular = light.specular * spec * vec3(texture(material.texture_specular1, TexCoord));
    return (ambient + diffuse + specular);
}
vec3 CalcPointLight(PointLight light, vec3 viewDir){
    vec3 lightDir = normalize(light.position - FragPos);
    // diffuse
    float diff = max(dot(Normal, lightDir), 0.0f);
    // specular
    vec3 relfectDir = reflect(-lightDir, Normal);
    float spec = pow(max(dot(viewDir, relfectDir), 0.0f), material.shininess);
    // weaken
    float distance = length(light.position - FragPos);
    // 衰减系数
    float attenuation = 1.0 / (light.constant + light.linear*distance + light.quadratic*distance*distance);
    // result
    vec3 ambient = light.ambient * vec3(texture(material.texture_diffuse1, TexCoord));
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.texture_diffuse1, TexCoord));
    vec3 specular = light.specular * spec * vec3(texture(material.texture_specular1, TexCoord));
    return (ambient + diffuse + specular) * attenuation;
}
vec3 CalcSpotLight(SpotLight light, vec3 viewDir){
    vec3 lightDir = normalize(light.position - FragPos);
    // diffuse
    float diff = max(dot(Normal, lightDir), 0.0f);
    // specular
    vec3 relfectDir = reflect(-lightDir, Normal);
    float spec = pow(max(dot(viewDir, relfectDir), 0.0f), material.shininess);
    // 片段到光与光方向的夹角余弦值
    float theta = dot(normalize(light.position - FragPos), normalize(-light.direction));
    float epsilon = light.cutOff - light.outerCutOff; // 余弦计算
    // 软化边缘
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);
    // weaken
    float distance = length(light.position - FragPos);
    float attenuation = 1.0 / (light.constant + light.linear*distance + light.quadratic*distance*distance);

    vec3 ambient = light.ambient * vec3(texture(material.texture_diffuse1, TexCoord));
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.texture_diffuse1, TexCoord));
    vec3 specular = light.specular * spec * vec3(texture(material.texture_specular1, TexCoord));
    return (ambient + diffuse + specular) * intensity * attenuation;
    //return (ambient + diffuse + specular) * intensity;
}