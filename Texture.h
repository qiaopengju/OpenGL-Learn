#ifndef TEXTURE_H
#define TEXTURE_H
#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#define STB_IMAGE_IMPLEMENTATION
#include "deps/stb_image.h"  // image depends lib

class Texture{
public:
    unsigned int ID;
    GLint wrapS, wrapT, minFilter, magFilter;
    float width, height;

    void use(){ // BindTexture 
        // 绑定纹理前先激活纹理单元
        glBindTexture(GL_TEXTURE_2D, ID); 
    } 
    Texture(std::string texturePath, GLint wrapS, GLint wrapT, GLint minFilter, GLint magFilter){
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
#endif