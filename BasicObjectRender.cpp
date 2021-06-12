#include "BasicObjectRender.h"

RenderOBJ::RenderOBJ(){ }

void RenderOBJ::bindVertexBuffer(){
    glBindVertexArray(VAO);
}

/* 正方体：顶点数据 vec3 position; vec3 normals; vec2 texcoords */
CubeRender::CubeRender(){
    static GLfloat cubeVertices[] = { // 正方体顶点
        /*----positions----|------normals-------|--texture coords-*/
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
         0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,

        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,

        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,

        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f
    };
    static glm::vec3 tangent[36 * 2]; // 存储tangent和bitangent
    for (int i = 0; i < 36; i += 3){ // 每次处理三个点
        glm::vec3 pos1 = glm::vec3(cubeVertices[8 * i], cubeVertices[8 * i + 1], cubeVertices[8 * i + 2]);
        glm::vec3 pos2 = glm::vec3(cubeVertices[8 * (i+1)], cubeVertices[8 * (i+1) + 1], cubeVertices[8 * (i+1) + 2]);
        glm::vec3 pos3 = glm::vec3(cubeVertices[8 * (i+2)], cubeVertices[8 * (i+2) + 1], cubeVertices[8 * (i+2) + 2]);
        glm::vec2 uv1 = glm::vec2(cubeVertices[8 * i + 6], cubeVertices[8 * i + 7]);
        glm::vec2 uv2 = glm::vec2(cubeVertices[8 * (i+1) + 6], cubeVertices[8 * (i+1) + 7]);
        glm::vec2 uv3 = glm::vec2(cubeVertices[8 * (i+2) + 6], cubeVertices[8 * (i+2) + 7]);
        TangentCoor result = cacTangentCoord(pos1, pos2, pos3, uv1, uv2, uv3);
        for (int j = 0; j < 3; j++){ // 给三个顶点赋值
            tangent[6 * i + 2 * j] = result.tangent;
            tangent[6 * i + 2 * j + 1] = result.bitangent;
        }
    }
    unsigned int VBO;

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), cubeVertices, GL_STATIC_DRAW);
    // vertices positions
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)0);
    // vertices normals
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));
    // vertices texture coordinates
    glad_glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)(6 * sizeof(GLfloat)));

    //tangent & bitangent
    unsigned int VBOtangent;

    glGenBuffers(1, &VBOtangent);
    glBindBuffer(GL_ARRAY_BUFFER, VBOtangent);
    glBufferData(GL_ARRAY_BUFFER, sizeof(tangent), &tangent[0], GL_STATIC_DRAW);
    // tangent
    glad_glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void*)0);
    // bitangent
    glad_glEnableVertexAttribArray(4);
    glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

CubeRender::~CubeRender(){
    glDeleteVertexArrays(1, &VAO);
}

void CubeRender::Draw(){
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);
}

/* 平面：顶点数据 vec3 positions; vec2 texcoords */
PlaneRender::PlaneRender(){
    static GLfloat planeVertices[] = {   // 平面顶点
        // first triangle
        //-position-------|--tex-coord
         0.5f,  0.5f, 0.0f, 1.0f, 1.0f, 1.0f,   // 右上
         0.5f, -0.5f, 0.0f, 1.0f, 1.0f, 0.0f,   // 右下
        -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f,   // 左下
        // second triangle
         0.5f,  0.5f, 0.0f, 1.0f, 1.0f, 1.0f,   // 右上
        -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f,   // 左下
        -0.5f,  0.5f, 0.0f, 1.0f, 0.0f, 1.0f    // 左上
    };
    unsigned int VBO;

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glBufferData(GL_ARRAY_BUFFER, sizeof(planeVertices), planeVertices, GL_STATIC_DRAW);
    // vertices positions
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (void*)0);
    // vertices texcoords 
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

PlaneRender::~PlaneRender(){
    glDeleteVertexArrays(1, &VAO);
}

void PlaneRender::Draw(){
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
}

/* 2D精灵: 顶点数据 vec2 positions; vec2 texcoords */
Spirit2DRender::Spirit2DRender(){
    static float spirit2DVertices[] = { // 2D平面图顶点
        // first triangle
        //-position-|--tex-coord
         1.0f,  1.0f, 1.0f, 1.0f,   // 右上
         1.0f, -1.0f, 1.0f, 0.0f,   // 右下
        -1.0f, -1.0f, 0.0f, 0.0f,   // 左下
        // second triangle
         1.0f, 1.0f, 1.0f, 1.0f,   // 右上
        -1.0, -1.0f, 0.0f, 0.0f,   // 左下
        -1.0f, 1.0f, 0.0f, 1.0f    // 左上
    };
    unsigned int VBO;

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glBufferData(GL_ARRAY_BUFFER, sizeof(spirit2DVertices), spirit2DVertices, GL_STATIC_DRAW);
    // vertices positions
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (void*)0);
    // vertices normals
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (void*)(2 * sizeof(GLfloat)));

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

Spirit2DRender::~Spirit2DRender(){
    glDeleteVertexArrays(1, &VAO);
}

void Spirit2DRender::Draw(){
    glDisable(GL_DEPTH_TEST);
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
}

TangentCoor cacTangentCoord(glm::vec3 pos1, glm::vec3 pos2, glm::vec3 pos3, glm::vec2 uv1, glm::vec2 uv2, glm::vec2 uv3){
    glm::vec2 deltaUV1 = uv2 - uv1;
    glm::vec2 deltaUV2 = uv3 - uv2;
    glm::vec3 edge1 = pos2 - pos1;
    glm::vec3 edge2 = pos3 - pos2;

    glm::vec3 tangent, bitangent;
    GLfloat f = 1.0f / (deltaUV1.s * deltaUV2.t - deltaUV2.s * deltaUV1.t);
    tangent.x = f * (deltaUV2.s * edge1.x - deltaUV1.t * edge2.x);
    tangent.y = f * (deltaUV2.s * edge1.y - deltaUV1.t * edge2.y);
    tangent.z = f * (deltaUV2.s * edge1.z - deltaUV1.t * edge2.z);
    bitangent.x = f * (-deltaUV2.s * edge1.x + deltaUV1.s * edge2.x);
    bitangent.y = f * (-deltaUV2.s * edge1.y + deltaUV1.s * edge2.y);
    bitangent.z = f * (-deltaUV2.s * edge1.z + deltaUV1.s * edge2.z);
    TangentCoor result;
    result.tangent = tangent;
    result.bitangent = bitangent;
    return result;
}