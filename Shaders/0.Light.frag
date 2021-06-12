#version 330
#define MAX_POINT_LIGHTS 20
#define MAX_SPOT_LIGHTS 1

struct Material{
    sampler2D texture_diffuse1;  // 漫反射贴图
    sampler2D texture_diffuse2;  // 漫反射贴图
    sampler2D texture_diffuse3;  // 漫反射贴图

    sampler2D texture_specular1; // 镜面反射贴图
    sampler2D texutre_specular2; // 镜面反射贴图
    sampler2D texutre_specular3; // 镜面反射贴图
    int shininess;

    sampler2D texture_normal1; // 法线贴图 
    sampler2D texture_normal2; // 法线贴图 
    sampler2D texture_normal3; // 法线贴图 
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
float CalcShadow(vec4 FragPosLightSpace);

in VS_OUT{
    vec2 TexCoord;   // 贴图坐标
    vec3 FragPos;    // 点在世界坐标系下坐标
    mat3 TBN;
    vec4 FragPosLightSpace;
} vs_in;
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
uniform sampler2D depthMap;

vec3 Normal;     // 法向量
float shadow;

void main(){
    shadow = CalcShadow(vs_in.FragPosLightSpace); // 计算阴影参数
    Normal = vec3(texture(material.texture_normal1, vs_in.TexCoord)) * 2 - vec3(1.0); // translate [0, 1] to [-1, 1]
    Normal = vs_in.TBN * normalize(Normal);
    vec3 result = vec3(0.f);
    vec3 viewDir = normalize(viewPosition - vs_in.FragPos);
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
    //vec3 relfectDir = reflect(-lightDir, Normal);
    //float spec = pow(max(dot(relfectDir, viewDir), 0.0f), material.shininess);
    // 使用视角与光线的半角计算Specular
    vec3 halfDir = normalize(lightDir + viewDir);
    float spec = pow(max(dot(halfDir, Normal), 0.0f), material.shininess * 4);
    // result
    vec3 ambient = light.ambient * vec3(texture(material.texture_diffuse1, vs_in.TexCoord));
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.texture_diffuse1, vs_in.TexCoord));
    vec3 specular = light.specular * spec * vec3(texture(material.texture_specular1, vs_in.TexCoord));
    return ambient + (diffuse + specular) * (1 - shadow);
    //return (ambient + diffuse + specular);
}
vec3 CalcPointLight(PointLight light, vec3 viewDir){
    vec3 lightDir = normalize(light.position - vs_in.FragPos);
    // diffuse
    float diff = max(dot(Normal, lightDir), 0.0f);
    // specular
    vec3 relfectDir = reflect(-lightDir, Normal);
    float spec = pow(max(dot(viewDir, relfectDir), 0.0f), material.shininess);
    // weaken
    float distance = length(light.position - vs_in.FragPos);
    // 衰减系数
    float attenuation = 1.0 / (light.constant + light.linear*distance + light.quadratic*distance*distance);
    // result
    vec3 ambient = light.ambient * vec3(texture(material.texture_diffuse1, vs_in.TexCoord));
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.texture_diffuse1, vs_in.TexCoord));
    vec3 specular = light.specular * spec * vec3(texture(material.texture_specular1, vs_in.TexCoord));
    //return (ambient + diffuse + specular) * attenuation;
    return ((ambient + (diffuse + specular) * (1 - shadow))) * attenuation;
}
vec3 CalcSpotLight(SpotLight light, vec3 viewDir){
    vec3 lightDir = normalize(light.position - vs_in.FragPos);
    // diffuse
    float diff = max(dot(Normal, lightDir), 0.0f);
    // specular
    vec3 relfectDir = reflect(-lightDir, Normal);
    float spec = pow(max(dot(viewDir, relfectDir), 0.0f), material.shininess);
    // 片段到光与光方向的夹角余弦值
    float theta = dot(normalize(light.position - vs_in.FragPos), normalize(-light.direction));
    float epsilon = light.cutOff - light.outerCutOff; // 余弦计算
    // 软化边缘
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);
    // weaken
    float distance = length(light.position - vs_in.FragPos);
    float attenuation = 1.0 / (light.constant + light.linear*distance + light.quadratic*distance*distance);

    vec3 ambient = light.ambient * vec3(texture(material.texture_diffuse1, vs_in.TexCoord));
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.texture_diffuse1, vs_in.TexCoord));
    vec3 specular = light.specular * spec * vec3(texture(material.texture_specular1, vs_in.TexCoord));
    //return (ambient + diffuse + specular) * intensity * attenuation;
    return ((ambient + (diffuse + specular) * (1 - shadow))) * intensity * attenuation;
    //return (ambient + diffuse + specular) * intensity;
}

float CalcShadow(vec4 FragPosLightSpace){
    vec3 projCoord = FragPosLightSpace.xyz / FragPosLightSpace.w; // 做透视除法，转换到NDC
    projCoord = projCoord * 0.5 + 0.5; // turn [-1,1] to [0,1]
    float nearstDepth = texture(depthMap, projCoord.xy).r;
    float bais = max(0.05 * dot(normalize(Normal),normalize(directionalLight.direction)), 0.005);
    if (projCoord.z > 1.0) return 0; // 坐标超出远平面，设其不在阴影中
    // percentage close filtering
    vec2 pixelSize = 1.0 / textureSize(depthMap, 0);
    // 采样周围9个像素的深度，返回平均值
    float result = 0;
    for (int i = -1; i <= 1; i++){
        for (int j = -1; j <= 1; j++){
            if (projCoord.z - bais > texture(depthMap, projCoord.xy + vec2(i, j) * pixelSize).r) result += 1;
        }
    }
    return result / 9.0;
}