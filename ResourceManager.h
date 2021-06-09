#ifndef RESOURCE_MANAGER_H
#define RESOURCE_MANAGER_H
#include <iostream>
#include <sstream>
#include <string>
#include <map>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Texture.h"
#include "Shader.h"
#include "Model.h"

class ResourceManager{
public:
    ResourceManager();
    static Texture2D &getTexture2D(const std::string &name);
    static TextureCubemap &getTextureCubemap(const std::string &name);
    static Model &getModel(const std::string &name);
    static Shader &getShader(const std::string &name);

    static Texture2D LoadTexture2D(const std::string &path, std::string name, GLint wrapS = GL_REPEAT, GLint wrapT = GL_REPEAT, GLint minFilter = GL_LINEAR, GLint magFilter = GL_LINEAR);
    static TextureCubemap LoadTextureCubemap(const std::string &path, std::string name);
    static Model LoadModel(const std::string &path, std::string name);
    static Shader LoadShader(const std::string &vShaderFile, const std::string &fShaderFile, const std::string &gShaderFile, std::string name);
    static Shader LoadShader(const std::string &vShaderFile, const std::string &fShaderFile, std::string name);
private:
    static std::map<std::string, Texture2D> Texture2Ds;
    static std::map<std::string, TextureCubemap> TextureCubemaps;
    static std::map<std::string, Shader> Shaders;
    static std::map<std::string, Model> Models;

    static Texture2D loadTexture2DFromFile(const std::string &path, GLint wrapS, GLint wrapT, GLint minFilter, GLint magFilter);
    static TextureCubemap loadTextureCubemapFromFile(const std::string &path);
};

#endif