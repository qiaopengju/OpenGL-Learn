#ifndef CAMERA_H
#define CAMERA_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Camera{
public:
    glm::vec3 cameraPos;        // 相机位置
    glm::vec3 cameraTarget;     // 指向点
    glm::vec3 cameraDirection;  // 相机指向方向的反方向
    glm::vec3 cameraRight;      // 相机右轴
    glm::vec3 cameraUp;         // 相机上轴
    glm::mat4 view;

    Camera(){
        cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
        cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);
        cameraDirection = glm::normalize(cameraPos - cameraTarget);
        glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
        cameraRight = glm::normalize(glm::cross(up, cameraDirection));
        cameraUp = glm::normalize(glm::cross(cameraDirection, cameraRight));
    }
};
#endif