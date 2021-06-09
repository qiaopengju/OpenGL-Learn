#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

class Shader {
public:
    unsigned int ID; // 程序ID
    Shader();
    Shader(const GLchar *vertexPath, const GLchar *fragmentPath); // 构造着色器: vertex + fragment
    Shader(const GLchar *vertexPath, const GLchar *fragmentPath, const GLchar *geometryPath); // 构造着色器 + geometry
    void use(); // 激活着色器函数

    // 设置uniform 
    void setBool(const std::string &name, bool value) const;
    void setInt(const std::string &name, int value) const;
    void setFloat(const std::string &name, float value) const;
    void setVec2(const std::string &name, glm::vec2 value) const;
    void setVec2(const std::string &name, float x, float y) const;
    void setVec3(const std::string &name, glm::vec3 value) const;
    void setVec3(const std::string &name, float x, float y, float z) const;
    void setVec4(const std::string &name, glm::vec4 value) const;
    void setVec4(const std::string &name, float x, float y, float z, float w) const;
    void setMat2(const std::string &name, glm::mat2 value) const;
    void setMat3(const std::string &name, glm::mat3 value) const;
    void setMat4(const std::string &name, glm::mat4 value) const;

private:
    void checkCompileErrors(unsigned int shader, std::string type); // 检测编译错
};
#endif