#include "Texture.h"

void Texture2D::bind(){ // BindTexture 
    // 绑定纹理前先激活纹理单元
    glBindTexture(GL_TEXTURE_2D, ID); 
} 

Texture2D::Texture2D(GLint wrapS, GLint wrapT, GLint minFilter, GLint magFilter){
    glGenTextures(1, &ID);
    glBindTexture(GL_TEXTURE_2D, ID); 
    /* set the texture wrapping parameters 设置超出纹理坐标的环绕方式*/
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapS);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapT);
    /* set teh texture filtering paramenters 设置采样参数 */
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minFilter);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magFilter);
    glBindTexture(GL_TEXTURE_2D, 0);
}

Texture2D::Texture2D(){}

TextureCubemap::TextureCubemap(){
    glGenTextures(1, &ID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, ID);

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}
void TextureCubemap::bind(){
    glBindTexture(GL_TEXTURE_CUBE_MAP, ID);
}