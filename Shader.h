#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

class Shader {
public:
    unsigned int ID; // 程序ID
    Shader(const GLchar *vertexPath, const GLchar *fragmentPath); // 构造着色器
    void use(); // 激活着色器函数

    // 设置uniform 
    void setBool(const std::string &name, bool value) const;
    void setInt(const std::string &name, int value) const;
    void setFloat(const std::string &name, float value) const;

private:
    void checkCompileErrors(unsigned int shader, std::string type); // 检测编译错
};
#endif

Shader::Shader(const GLchar *vertexPath, const GLchar *fragmentPath){
    // 1.读取着色器source code
    const char *vShaderCode, *fShaderCode;
    std::string vertexCode, fragmentCode;
    std::ifstream vertexFile, fragmentFile;
    // ensure ifstream objects can throw exceptions:
    vertexFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    fragmentFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    try{
        // open file
        vertexFile.open(vertexPath);
        fragmentFile.open(fragmentPath);
        std::stringstream vShaderStream, fShaderStream;
        // read file's buffer into stream
        vShaderStream << vertexFile.rdbuf();
        fShaderStream << fragmentFile.rdbuf();
        // close file
        vertexFile.close();
        fragmentFile.close();
        // convert stream to string
        vertexCode = vShaderStream.str();
        fragmentCode = fShaderStream.str();
    }
    catch (std::ifstream::failure &e){
        std::cout << "ERROR::SHADER::FILE_NOT_SUCCESSFULY_READ" << std::endl;
    }
    // 2.编译着色器
    unsigned int vertex, fragment;
    vShaderCode = vertexCode.c_str();
    fShaderCode = fragmentCode.c_str();
    // vertex shader
    vertex = glCreateShader(GL_VERTEX_SHADER); // 创建Shader
    glShaderSource(vertex, 1, &vShaderCode, NULL); // 设置源码
    glCompileShader(vertex); // 编译shader
    checkCompileErrors(vertex, "VERTEX"); // 检查编译错
    // fragment shader
    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fShaderCode, NULL);
    glCompileShader(fragment);
    checkCompileErrors(fragment, "FRAGMENT");
    // shader program
    ID = glCreateProgram(); // 创建shader程序
    glAttachShader(ID, vertex); // 将着色器Attach到shader程序
    glAttachShader(ID, fragment);
    glLinkProgram(ID); // 链接着色器
    checkCompileErrors(ID, "PROGRAM");
    // 3.删除着色器
    glDeleteShader(vertex);
    glDeleteShader(fragment);
}

void Shader::use(){
    glUseProgram(ID);
}

void Shader::checkCompileErrors(unsigned int shader, std::string type){
    int success;
    char infoLog[1024];
    if (type == "PROGRAM"){
        glGetProgramiv(shader, GL_LINK_STATUS, &success);
        if (!success){
            glGetProgramInfoLog(shader, 1024, NULL, infoLog);
            std::cout << "ERROR::PROGRAM_LINKING_ERROR of type:" << type << "\n" << infoLog << "\n"
                "-----------------------------------------" << std::endl;
        }
    } else{
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success){
            glGetShaderInfoLog(shader, 1024, NULL, infoLog);
            std::cout << "ERROR::SHADER_COMPILATION_ERROR of type:" << type << "\n" << infoLog << "\n"
                "-----------------------------------------" << std::endl;
        }
    }
}

void Shader::setBool(const  std::string &name, bool value) const{
    glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
}
void Shader::setInt(const  std::string &name, int value) const{
    glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}
void Shader::setFloat(const  std::string &name, float value) const{
    glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}