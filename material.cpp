#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include "Shader.h"
#include "Texture.h"
#include "Camera.h"

using namespace std;

/* setting */
const unsigned int SCR_WIDTH = 1000;
const unsigned int SCR_HEIGHT = 1000;
Camera camera; // 全局相机

float vertices[] = {
    /*-----positoin----|------法向量--------*/
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
     0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 
     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 
     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 
    -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 

    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,

    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
     0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
     0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
     0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
     0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
     0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
};
float verticesPlane[] = {
    // first triangle
     0.5f,  0.5f, 0.0f, 1.0f, 1.0f,   // 右上
     0.5f, -0.5f, 0.0f, 1.0f, 0.0f,   // 右下
    -0.5f, -0.5f, 0.0f, 0.0f, 0.0f,   // 左下
    // second triangle
     0.5f,  0.5f, 0.0f, 1.0f, 1.0f,   // 右上
    -0.5f, -0.5f, 0.0f, 0.0f, 0.0f,   // 左下
    -0.5f,  0.5f, 0.0f, 0.0f, 1.0f    // 左上
};

void framebuffer_size_callback(GLFWwindow* window, int width, int height); // 窗口大小改变时回调该函数
void mouse_callback(GLFWwindow *window, double xpos, double ypos); // 光标移动回调
void processInput(GLFWwindow* window, Camera &camera, float deltaTime); // 处理输入
void myTransform(glm::vec3 translate, float angelX, float angelY, float angelZ, Shader &shader); // 变换
void HelpMarker(const char *title, const char* desc); // imGui 显示帮助

int main(){
    /* glfw: initalize and cofigure */
    /*------------------------------*/
    glfwInit(); //init
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); //major version 3.3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); //minor version 3.3
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); //core profile
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); //Mac OS X
#endif

    /* glfw window creation */
    /*----------------------*/
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL); //create a window
    if (window == NULL){
        cout << "Failed to create GLFW window" << endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback); //注册窗口大小改变回调函数
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED); // 初始禁用光标
    glfwSetCursorPosCallback(window, mouse_callback);   // 光标位置回调

    /* glad: load all OpenGL function pointers */
    /*-----------------------------------------*/
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){ //GLAD
        cout << "Failed to initialize GLAD" << endl;
        return -1;
    }
    /* 开启深度测试, 之后在渲染循环中需清除深度缓冲(DEPTH_BUFFER_BIT) enable Z-buffer */
    glEnable(GL_DEPTH_TEST);

    /* build and compile shader */
    /*--------------------------*/
    Shader shaderLightCube("Shaders/lightCube.vert", "Shaders/lightCube.frag");
    Shader shaderColorCube("Shaders/phongLighting.vert", "Shaders/material.frag");
    Shader shaderPlane("Shaders/3DTexture.vert", "Shaders/plane.frag");

    /* set up vertex data and buffers and configure vertex attributes */
    /*----------------------------------------------------------------*/
    /* Cube data set */
    /*----------------*/
    unsigned int VBOCube, VAOColorCube, VAOLightCube;
    glGenBuffers(1, &VBOCube);
    glGenVertexArrays(1, &VAOColorCube);

    glBindVertexArray(VAOColorCube);
    glBindBuffer(GL_ARRAY_BUFFER, VBOCube);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    /* 设置顶点position属性 */
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glad_glEnableVertexAttribArray(0);
    /* 设置顶点所在面法向量属性 */
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glad_glEnableVertexAttribArray(1);

    glGenVertexArrays(1, &VAOLightCube);
    glBindVertexArray(VAOLightCube);
    /* 在Attribute Pointer前需要Bind VBOCube */
    glBindBuffer(GL_ARRAY_BUFFER, VBOCube);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glad_glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glad_glEnableVertexAttribArray(1);

    /* Plane data set */
    /*-----------------*/
    unsigned int VBOPlane, VAOPlane;
    glGenBuffers(1, &VBOPlane);
    glGenVertexArrays(1, &VAOPlane);

    glBindVertexArray(VAOPlane);
    glBindBuffer(GL_ARRAY_BUFFER, VBOPlane);
    glBufferData(GL_ARRAY_BUFFER, sizeof(verticesPlane), verticesPlane, GL_STATIC_DRAW);
    /* buffer attribute */
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float))); // important： OFFSET需要是sizeof(float)
    glEnableVertexAttribArray(1);
    /* Plane texture set */
    Texture texturePlane("Resource/Img/square.png", GL_REPEAT, GL_REPEAT, GL_LINEAR, GL_LINEAR);
    shaderPlane.use();
    shaderPlane.setInt("texturePlane", 0);
    
    /* 设置变换 */
    float angel[] = {0.0f, 25.0f, 0.0f};
    glm::vec3 translate = glm::vec3(0.0f, 0.0f, 0.0f);

    /* 设置光照参数 */
    float ambientMaterial[] = {1.0f, 0.5f, 0.31f};
    float diffuseMaterial[] = {1.0f, 0.5f, 0.31f};
    float specularMaterial[] = {0.5f, 0.5f, 0.5f};
    float ambientLight[] = {0.2f, 0.2f, 0.2f};
    float diffuseLight[] = {0.5f, 0.5f, 0.5f};
    float specularLight[] = {1.0f, 1.0f, 1.0f};
    int shininess(32); // 物体反光度

    float colorLight[3] = {1.0f, 1.0f, 1.0f};
    glm::vec3 lightPosition = glm::vec3(-0.6f, 1.45f, 0.7f);

    /* Imgui Setting */
    /*---------------*/
    // Setup imgui context
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    // Setup style
    ImGui::StyleColorsDark();
    // Setup backends
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 150");
    // Set imgui status
    //bool show_window = true;
    bool showPlane = true;

    /* Render Loop 渲染循环 */
    /*---------------------*/
    while(!glfwWindowShouldClose(window)){
        static float deltaTime = 0.0f;  // 当前帧与上一帧的时间差
        static float lastFrame = 0.0f;   // 上一帧时间
        float currentTime = glfwGetTime();
        deltaTime = currentTime - lastFrame;
        lastFrame = currentTime;
        /* input */
        processInput(window, camera, deltaTime); 

        /* render */
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // 清除颜色缓冲&深度缓冲

        /* Coordinate Settings 坐标系统设置 */
        /*--------------------------------*/
        glm::mat4 view = camera.view;       // 观察矩阵
        glm::mat4 projection = glm::mat4(1.0f); // 投影矩阵
        glm::mat4 model = glm::mat4(1.0f);      // 模型矩阵
        //model = glm::rotate(model, glm::radians(-55.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        // 初始视口在原点，要看见图形，相机要向+z移动，相当于世界坐标向-z移动
        projection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH/(float)SCR_HEIGHT, 0.1f, 100.0f); //45度透视投影

        /* draw color cube*/
        /* Be sure use shader before */
        shaderColorCube.use();
        shaderColorCube.setMat4("model", model);
        shaderColorCube.setMat4("view", view);
        shaderColorCube.setMat4("projection", projection);
        /* light set */
        //shaderColorCube.setVec3("colorObject", 1.0f, 0.5f, 0.31f);
        shaderColorCube.setVec3("colorLight", colorLight[0], colorLight[1], colorLight[2]);
        shaderColorCube.setVec3("viewPosition", camera.cameraPos);
        shaderColorCube.setVec3("lightPosition", lightPosition);
        shaderColorCube.setVec3("material.ambient", ambientMaterial[0], ambientMaterial[1], ambientMaterial[2]);
        shaderColorCube.setVec3("material.diffuse", diffuseMaterial[0], diffuseMaterial[1], diffuseMaterial[2]);
        shaderColorCube.setVec3("material.specular", specularMaterial[0],specularMaterial[1],specularMaterial[2]);
        shaderColorCube.setInt("material.shininess", shininess);
        shaderColorCube.setVec3("light.ambient", ambientLight[0], ambientLight[1], ambientLight[2]);
        shaderColorCube.setVec3("light.diffuse", diffuseLight[0], diffuseLight[1], diffuseLight[2]);
        shaderColorCube.setVec3("light.specular", specularLight[0], specularLight[1], specularLight[2]);

        glBindVertexArray(VAOColorCube);
        myTransform(translate, angel[0], angel[1], angel[2], shaderColorCube);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        /* draw Lighting cube */
        shaderLightCube.use();
        shaderLightCube.setMat4("view", view);
        shaderLightCube.setMat4("projection", projection);

        model = glm::mat4(1.0f);
        model = glm::translate(model, lightPosition);
        model = glm::rotate(model, glm::radians(25.f), glm::vec3(0.1f, 1.0f, 0.0f));
        model = glm::scale(model, glm::vec3(0.1f));
        shaderLightCube.setMat4("model", model);
        shaderLightCube.setVec3("colorLight", colorLight[0], colorLight[1], colorLight[2]);
        //glBindVertexArray(VAOLightCube);
        glBindVertexArray(VAOLightCube);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        /* draw plane */
        //texturePlane.use();
        if (showPlane){
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, texturePlane.ID);
            shaderPlane.use();
            shaderPlane.setMat4("view", view);
            shaderPlane.setMat4("projection", projection);
            model = glm::mat4(1.0f);
            model = glm::translate(model, glm::vec3(0.0f, -0.5f, 0.0f));
            model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
            model = glm::scale(model, glm::vec3(7.0f));
            shaderPlane.setMat4("model", model);
            
            glBindVertexArray(VAOPlane);
            glDrawArrays(GL_TRIANGLES, 0, 6);
        }

        /* draw imgui */
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        ImGui::Begin("Configure"); {
            ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "Press TAB to use mouse/camera!");
            ImGui::SameLine(); HelpMarker("Key", " ESC:exit\n W: forward\n A: left\n S: back\n D: right\n SPACE: up");
            ImGui::SameLine(); ImGui::Checkbox("show plane", &showPlane);
            ImGui::BulletText("Lamp Setting");
            ImGui::ColorEdit3("Lamp color", colorLight);
            ImGui::SliderFloat3("Lamp Position", glm::value_ptr(lightPosition), -2.0f, 2.0f);

            ImGui::BulletText("Cube Material");
            ImGui::SliderFloat3("Ambient ", ambientMaterial, 0.0f, 1.0f);
            ImGui::SliderFloat3("Diffuse ", diffuseMaterial, 0.0f, 1.0f);
            ImGui::SliderFloat3("Specular ", specularMaterial, 0.0f, 1.0f);
            ImGui::SliderInt("Shininess", &shininess, 0, 256);
            ImGui::BulletText("Light Attribute");
            ImGui::SliderFloat3("Ambient", ambientLight, 0.0f, 1.0f);
            ImGui::SliderFloat3("Diffuse", diffuseLight, 0.0f, 1.0f);
            ImGui::SliderFloat3("Specular", specularLight, 0.0f, 1.0f);
            /*static const char *items[] = {"2", "4", "8", "16", "32", "64", "128", "256"};
            static int item_current = 4;
            shininess = pow((double)2, (double)(item_current+1)); 
            ImGui::ListBox("Shininess", &item_current, items, IM_ARRAYSIZE(items), 4);*/
        //shaderColorCube.setInt("shininess", shininess);
        } ImGui::End();

        ImGui::Render(); // rendering
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window); // 交换前后缓冲
        glfwPollEvents(); // poll events 回调事件
    }

    /* optional: 超出生存期，de-allocate所有资源 */
    glDeleteBuffers(1, &VBOCube);
    glDeleteVertexArrays(1, &VAOColorCube);
    glDeleteVertexArrays(1, &VAOLightCube);
    /* glfw: release resource */
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate(); 
    return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height){ //窗口大小改变时回调该函数
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window, Camera &camera, float deltaTime){
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS){ // 退出
        glfwSetWindowShouldClose(window, true);
    }
    /* camera input */
    // 帧数越低，相机速度越快，保证相机移动速率平稳
    float cameraSpeed = 2.5f * deltaTime; // adjust accordingly
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS){ // forward
        camera.cameraPos += cameraSpeed * camera.cameraFront;
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS){ // back
        camera.cameraPos -= cameraSpeed * camera.cameraFront;
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS){ // left
        camera.cameraPos -= glm::normalize(glm::cross(camera.cameraFront, camera.cameraUp)) * cameraSpeed;
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS){ // right
        camera.cameraPos += glm::normalize(glm::cross(camera.cameraFront, camera.cameraUp)) * cameraSpeed;
    }
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS){ // up
        camera.cameraPos += glm::vec3(0.0f, 1.0f, 0.0f) * cameraSpeed;
    }
    static bool tabPressFirst = true; // 设置触发器，按下Tab只有第一下有用
    if (tabPressFirst && glfwGetKey(window, GLFW_KEY_TAB) == GLFW_PRESS){ // 按下Tab，并且检查触发器
        if(camera.enableMouse){ // 之前是启用鼠标
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL); // 开启光标
        } else{ // 之前禁用鼠标
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED); // 取消光标
        }
        camera.enableMouse = !camera.enableMouse;
        tabPressFirst = false;
    }
    if (!tabPressFirst && glfwGetKey(window, GLFW_KEY_TAB) == GLFW_RELEASE){ // 释放Tab键，重置触发器
        tabPressFirst = true;
    }
    camera.updateView();
}

void mouse_callback(GLFWwindow *window, double xpos, double ypos){ // 光标移动回调
    static bool firstMouse = true;
    static float lastX(SCR_WIDTH / 2), lastY(SCR_HEIGHT / 2); // 上一次光标位置
    static float pitch(0.0f), yaw(-90.0f); // 俯仰角，偏航角
    if (firstMouse) { // 光标第一次进入窗口
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }
    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // 光标的y是从下到上减少的，故用 lastY - ypos
    lastX = xpos;
    lastY = ypos;

    static float sensitivity = 0.05f; // 鼠标灵敏度
    if (camera.enableMouse){
        xoffset *= sensitivity;
        yoffset *= sensitivity;
        yaw += xoffset;
        pitch += yoffset;
        // 设置俯仰角范围，我们设置为89度，因为90度会翻转相机
        if (pitch > 89.0f) pitch = 89.0f;
        if (pitch < -89.0f) pitch = -89.0f;
        glm::vec3 front;
        front.x = cos(glm::radians(pitch)) * cos(glm::radians(yaw));
        front.y = sin(glm::radians(pitch));
        front.z = cos(glm::radians(pitch)) * sin(glm::radians(yaw));
        camera.cameraFront = glm::normalize(front);
        camera.updateView();
    }
}

void myTransform(glm::vec3 move, float angelX, float angelY, float angelZ, Shader &shader){ // 对物体变换
    glm::mat4 translate = glm::mat4(1.0f);
    glm::mat4 rotate = glm::mat4(1.0f);
    translate = glm::translate(translate, move);
    rotate = glm::rotate(rotate, glm::radians(angelX), glm::vec3(1.0f, 0.0f, 0.0f));
    rotate = glm::rotate(rotate, glm::radians(angelY), glm::vec3(0.0f, 1.0f, 0.0f));
    rotate = glm::rotate(rotate, glm::radians(angelZ), glm::vec3(0.0f, 0.0f, 1.0f));
    /* 将变换矩阵传给着色器 */
    shader.setMat4("model", translate * rotate);
}

void HelpMarker(const char* title, const char* desc){ // imGui 显示帮助
    ImGui::TextDisabled("(?) %s", title);
    if (ImGui::IsItemHovered())
    {
        ImGui::BeginTooltip();
        ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
        ImGui::TextUnformatted(desc);
        ImGui::PopTextWrapPos();
        ImGui::EndTooltip();
    }
}