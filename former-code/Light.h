#ifndef LIGHT_H
#define LIGHT_H

#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Light{
public:
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
    void setAmbient(glm::vec3 _ambient){ this->ambient = _ambient; }
    void setDiffuse(glm::vec3 _diffuse){ this->diffuse = _diffuse; }
    void setspecular(glm::vec3 _specular){ this->specular= _specular; }
    void setLight(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular){
        this->ambient = ambient;
        this->diffuse = diffuse;
        this->specular = specular;
    }
};

class DirLight: public Light{
public:
    glm::vec3 direction; 
    void setDirection(glm::vec3 _dir){ this->direction = _dir; }
    DirLight(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, glm::vec3 direction){
        setLight(ambient, diffuse, specular);
        this->direction = direction;
    }
};
class PointLight: public Light{
public:
    glm::vec3 position;
    // 光线衰减参数
    float constant;
    float linear;
    float quadratic;

    PointLight(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular){
        position = glm::vec3(0.f);
        setLight(ambient, diffuse, specular);
        constant = 1.0f;
        linear = 0.09f;
        quadratic = 0.032f;
    }
    PointLight(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, glm::vec3 position){
        setLight(ambient, diffuse, specular);
        this->position = position;
        constant = 1.0f;
        linear = 0.09f;
        quadratic = 0.032f;
    }
    PointLight(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, glm::vec3 position, 
        float constant, float linear, float quadratic){
        setLight(ambient, diffuse, specular);
        this->position = position;
        this->constant = constant;
        this->quadratic = quadratic;
    }
};
class SpotLight: public Light{
public:
    glm::vec3 position;
    glm::vec3 direction;
    float cutOff;       // 聚光内圆
    float outerCutOff;  // 聚光外圆
    // 光线衰减参数
    float constant;
    float linear;
    float quadratic;

    SpotLight(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, glm::vec3 position, glm::vec3 direction){
        setLight(ambient, diffuse, specular);
        this->position = position;
        this->direction = direction;
        cutOff = glm::cos(glm::radians(12.5f));
        outerCutOff = glm::cos(glm::radians(15.0f));
        // defautl distant: 100
        constant = 1.0f;
        linear = 0.045f;
        quadratic = 0.0075f;
    }
    SpotLight(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, glm::vec3 position, glm::vec3 direction, float cutOff, float outerCutOff){
        setLight(ambient, diffuse, specular);
        this->position = position;
        this->direction = direction;
        this->cutOff = glm::cos(glm::radians(cutOff));
        this->outerCutOff = glm::cos(glm::radians(outerCutOff));
        constant = 1.0f;
        linear = 0.045f;
        quadratic = 0.0075f;
    }
};
#endif