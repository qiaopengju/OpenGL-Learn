#version 330 core
layout (triangles) in;
layout (line_strip, max_vertices = 6) out;

in VS_OUT{
    vec3 normal;    // normal matrix * aNormal
    vec4 aPos;      // model * aPos
} gs_in[];
out vec3 color;

uniform mat4 projection;
uniform mat4 view;

uniform float MAGNITUDE;
void GenerateLine(int index){
    gl_Position = gl_in[index].gl_Position;
    EmitVertex();
    // normal是经过法矩阵变换到世界坐标的法向量，还需要与view projection相乘才能不变形
    gl_Position = gs_in[index].aPos + vec4(gs_in[index].normal, 0.0) * MAGNITUDE;
    gl_Position = projection * view * gl_Position;
    if (index == 0) color = vec3(1.0, 0.0, 0.0);
    else if (index == 1) color = vec3(0.0, 1.0, 0.0);
    else if (index == 2) color = vec3(0.0, 0.0, 1.0);
    EmitVertex();
    EndPrimitive();
}

void main(){
    GenerateLine(0);
    GenerateLine(1);
    GenerateLine(2);
}