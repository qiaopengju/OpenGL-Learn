#ifndef CAMERA_H
#define CAMERA_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Camera{
public:
    glm::vec3 cameraPos;        // 相机位置
    glm::vec3 cameraFront;      // 相机指向位置
    glm::vec3 cameraUp;         // 相机上轴
    glm::mat4 view;             // 基于该相机下的view举证
    bool enableMouse;           // 使用鼠标控制相机

    Camera(){
        cameraPos = glm::vec3(0.0f, 1.0f, 5.0f);
        cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
        cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
        glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
        view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
        enableMouse = true;
    }
    void updateView(){
        view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
    }
};
#endif