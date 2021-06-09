#ifndef RESOURCE_MANAGER_H
#define RESOURCE_MANAGER_H
#include <iostream>
#include <sstream>
#include <string>
#include <map>

#include "Texture.h"
#include "Shader.h"
#include "Model.h"

class ResourceManager{
public:
    std::map<std::string, Texture> textures;
    std::map<std::string, Shader> shaders;
    std::map<std::string, Model> models;
private:
};

#endif