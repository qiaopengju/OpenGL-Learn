#ifndef TEXTURE_H
#define TEXTURE_H
#include <iostream>
#include <string>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
// OpenGL 图片坐标的原点在左上角
// OpenGL 2D texture，data的起始位置是左下角，所以要将图片上下翻转
// OpenGL cubeBox texture的起始位置在左上角，无需将图片上下翻转
class Texture2D{
public:
    unsigned int ID;
    GLint wrapS, wrapT, minFilter, magFilter;
    float width, height;

    void bind(); // BindTexture 
    Texture2D(GLint wrapS, GLint wrapT, GLint minFilter, GLint magFilter);
    Texture2D();
};

class TextureCubemap{
public:
    unsigned int ID;

    TextureCubemap();
    void bind();
};
#endif