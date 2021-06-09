#ifndef TEXTURE_H
#define TEXTURE_H
#include <iostream>
#include <string>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#define STB_IMAGE_IMPLEMENTATION
#include "deps/stb_image.h"  // image depends lib

// OpenGL 图片坐标的原点在左上角
// OpenGL 2D texture，data的起始位置是左下角，所以要将图片上下翻转
// OpenGL cubeBox texture的起始位置在左上角，无需将图片上下翻转
class Texture2D{
public:
    unsigned int ID;
    GLint wrapS, wrapT, minFilter, magFilter;
    float width, height;

    void use(){ // BindTexture 
        // 绑定纹理前先激活纹理单元
        glBindTexture(GL_TEXTURE_2D, ID); 
    } 
    Texture2D(std::string texturePath, GLint wrapS, GLint wrapT, GLint minFilter, GLint magFilter){
        texturePath = std::string("../") + texturePath;
        glGenTextures(1, &ID);
        glBindTexture(GL_TEXTURE_2D, ID); 
        /* set the texture wrapping parameters 设置超出纹理坐标的环绕方式*/
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapS);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapT);
        /* set teh texture filtering paramenters 设置采样参数 */
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minFilter);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magFilter);
        /* load texture */
        int width, height, nrChannels;
        stbi_set_flip_vertically_on_load(true);
        unsigned char *data = stbi_load(texturePath.c_str(), &width, &height, &nrChannels, 0);
        std::size_t pointPos = texturePath.find_last_of(".");
        std::string fileType = texturePath.substr(pointPos);
        if (data){
            if (fileType == ".jpeg" || fileType == ".jpg"){
                glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
                glGenerateMipmap(GL_TEXTURE_2D);
            } else if (fileType == ".png"){
                glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
                glGenerateMipmap(GL_TEXTURE_2D);
            }
        } else{
            std::cout << "FAILED TO LOAD TEXTURE:" << texturePath << std::endl;
        }
        stbi_image_free(data);
        glBindTexture(GL_TEXTURE_2D, 0);
    }
};

std::string faces[] = {"right.jpg", "left.jpg", "top.jpg", "bottom.jpg", "front.jpg", "back.jpg"};
class TextureCubemap{
public:
    unsigned int ID;

    TextureCubemap(std::string path){
        path = std::string("../") + path;
        glGenTextures(1, &ID);
        glBindTexture(GL_TEXTURE_CUBE_MAP, ID);

        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
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
    }
    void use(){
        glBindTexture(GL_TEXTURE_CUBE_MAP, ID);
    }
};
#endif