#version 330

out vec4 FragColor;

uniform vec3 colorLight;

void main(){
    FragColor = vec4(colorLight, 1.0f); // default:白色箱子
}