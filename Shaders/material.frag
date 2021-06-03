#version 330        // source code of fragment shader
in vec3 Normal;     // 法向量
in vec3 FragPos;    // 点在世界坐标系下坐标
out vec4 FragColor;

//uniform vec3 colorObject;
uniform vec3 colorLight;

struct Material{
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    int shininess;
};
struct Light{ // 光的强度
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform vec3 lightPosition;
uniform vec3 viewPosition;

uniform Material material;
uniform Light light;

void main(){
    // 环境因子
    vec3 ambient = material.ambient * light.ambient;
    // 计算漫反射因子
    // 与法向量, 光照到该点的方向向量有关
    vec3 lightDir = normalize(FragPos - lightPosition); // 光照方向，光指向物体
    float diff = max(dot(Normal, -lightDir), 0.0f); // 点乘为负的光是无效的，漫反射因子
    vec3 diffuse = (diff * material.diffuse) * light.diffuse;
    // 计算镜面反射
    // 与反射光与视角的夹角有关
    vec3 refDir = normalize(reflect(lightDir, Normal)); //反射光方向
    vec3 viewDir = normalize(viewPosition - FragPos);
    float spec = pow(max(dot(refDir, viewDir), 0.0f), material.shininess);
    vec3 specular = (spec * material.specular) * light.specular;

    vec3 result = (ambient + diffuse + specular) * colorLight;
    FragColor = vec4(result, 1.0f);
}