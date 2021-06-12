#ifndef BASIC_OBJ_RENDER_H
#define BASIC_OBJ_RENDER_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Camera.h"
#include "Light.h"
#include "ResourceManager.h"

// 根据三角形计算出其切向量和副切向量，第一列是切向量，第二列是副切向量
struct TangentCoor{
    glm::vec3 tangent;
    glm::vec3 bitangent;
};
static TangentCoor cacTangentCoord(glm::vec3 pos1, glm::vec3 pos2, glm::vec3 pos3, glm::vec2 uv1, glm::vec2 uv2, glm::vec2 uv3);

class RenderOBJ{
public:
    RenderOBJ();
    void bindVertexBuffer();
protected:
    unsigned int VAO;
};

class CubeRender : public RenderOBJ{
public:
    CubeRender();
    ~CubeRender();
    void Draw();
};

class PlaneRender : public RenderOBJ{
public:
    PlaneRender();
    ~PlaneRender();
    void Draw();
};

class Spirit2DRender : public RenderOBJ{
public:
    Spirit2DRender();
    ~Spirit2DRender();
    void Draw();
    void Draw(Shader &shader);
};

#endif