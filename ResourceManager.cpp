#include "ResourceManager.h"
#define STB_IMAGE_IMPLEMENTATION
#include "deps/stb_image.h"  // image depends lib

std::map<std::string, Texture2D> ResourceManager::Texture2Ds;
std::map<std::string, TextureCubemap> ResourceManager::TextureCubemaps;
std::map<std::string, Shader> ResourceManager:: Shaders;
std::map<std::string, Model> ResourceManager::Models;

ResourceManager::ResourceManager() {}

Texture2D ResourceManager::LoadTexture2D(const std::string &path, std::string name, GLint wrapS, GLint wrapT, 
        GLint minFilter, GLint magFilter){
    Texture2Ds[name] = loadTexture2DFromFile(path, wrapS, wrapS, minFilter, magFilter);
    return Texture2Ds[name];
}

TextureCubemap ResourceManager::LoadTextureCubemap(const std::string &path, std::string name){
    TextureCubemaps[name] = loadTextureCubemapFromFile(path);
    return TextureCubemaps[name];
}

Model ResourceManager::LoadModel(const std::string &path, std::string name){
    Models[name] = Model(path);
    return Models[name];
}

Shader ResourceManager::LoadShader(const std::string &vShaderFile, const std::string &fShaderFile, std::string name){
    Shaders[name] = Shader(vShaderFile.c_str(), fShaderFile.c_str());
    return Shaders[name];
}

Shader ResourceManager::LoadShader(const std::string &vShaderFile, const std::string &fShaderFile, const std::string &gShaderFile, std::string name){
    Shaders[name] = Shader(vShaderFile.c_str(), fShaderFile.c_str(), gShaderFile.c_str());
    return Shaders[name];
}

Texture2D &ResourceManager::getTexture2D(const std::string &name){
    if (Texture2Ds.find(name) == Texture2Ds.end()){
        std::cout << "RESOURCE_MANAGER::GET_TEXTURE2D_FAILED::" << name << std::endl;
    }
    return Texture2Ds[name]; 
}

TextureCubemap &ResourceManager::getTextureCubemap(const std::string &name){
    if (TextureCubemaps.find(name) == TextureCubemaps.end()){
        std::cout << "RESOURCE_MANAGER::GET_TEXTURE_CUBE_MAP_FAILED::" << name << std::endl;
    }
    return TextureCubemaps[name];
}

Model &ResourceManager::getModel(const std::string &name){
    if (Models.find(name) == Models.end()){
        std::cout << "RESOURCE_MANAGER::GET_MODEL_FAILED::" << name << std::endl;
    }
    return Models[name];
}

Shader &ResourceManager::getShader(const std::string &name){
    if (Shaders.find(name) == Shaders.end()){
        std::cout << "RESOURCE_MANAGER::GET_SHADER_FAILED::" << name << std::endl;
    }
    return Shaders[name];
}

Texture2D ResourceManager::loadTexture2DFromFile(const std::string &path, GLint wrapS, GLint wrapT, GLint minFilter, GLint magFilter){
    Texture2D texture(wrapS, wrapT, minFilter, magFilter);
    texture.bind();
    /* load texture */
    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true);
    unsigned char *data = stbi_load(path.c_str(), &width, &height, &nrChannels, 0);
    if (data){
        if (nrChannels == 3) {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        } else if (nrChannels == 4){
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        }
        glGenerateMipmap(GL_TEXTURE_2D);
    } else{
        std::cout << "FAILED TO LOAD TEXTURE:" << path << std::endl;
    }
    stbi_image_free(data);
    glBindTexture(GL_TEXTURE_2D, 0);

    return texture;
}

TextureCubemap ResourceManager::loadTextureCubemapFromFile(const std::string &path){
    std::string faces[] = {"right.jpg", "left.jpg", "top.jpg", "bottom.jpg", "front.jpg", "back.jpg"};
    TextureCubemap texture;
    texture.bind();
    /* load texture */
    int width, height, nrChannels;
    for (int i = 0; i < 6; i++){ // 遍历6个面的贴图
        stbi_set_flip_vertically_on_load(false);
        unsigned char *data = stbi_load((path + faces[i]).c_str(), &width, &height, &nrChannels, 0);
        if (data){
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
                0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        } else{
            std::cout << "Cubemap texture failed to load at path: " << path << faces[i] << std::endl;
        }
        stbi_image_free(data);
    }
    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

    return texture;
}