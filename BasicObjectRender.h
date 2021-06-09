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