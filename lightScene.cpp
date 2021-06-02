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

using namespace std;

/* setting */
const unsigned int SCR_WIDTH = 1000;
const unsigned int SCR_HEIGHT = 1000;
float vertices[] = {
    -0.5f, -0.5f, -0.5f,
     0.5f, -0.5f, -0.5f,
     0.5f,  0.5f, -0.5f,
     0.5f,  0.5f, -0.5f,
    -0.5f,  0.5f, -0.5f,
    -0.5f, -0.5f, -0.5f,

    -0.5f, -0.5f,  0.5f,
     0.5f, -0.5f,  0.5f,
     0.5f,  0.5f,  0.5f,
     0.5f,  0.5f,  0.5f,
    -0.5f,  0.5f,  0.5f,
    -0.5f, -0.5f,  0.5f,

    -0.5f,  0.5f,  0.5f,
    -0.5f,  0.5f, -0.5f,
    -0.5f, -0.5f, -0.5f,
    -0.5f, -0.5f, -0.5f,
    -0.5f, -0.5f,  0.5f,
    -0.5f,  0.5f,  0.5f,

     0.5f,  0.5f,  0.5f,
     0.5f,  0.5f, -0.5f,
     0.5f, -0.5f, -0.5f,
     0.5f, -0.5f, -0.5f,
     0.5f, -0.5f,  0.5f,
     0.5f,  0.5f,  0.5f,

    -0.5f, -0.5f, -0.5f,
     0.5f, -0.5f, -0.5f,
     0.5f, -0.5f,  0.5f,
     0.5f, -0.5f,  0.5f,
    -0.5f, -0.5f,  0.5f,
    -0.5f, -0.5f, -0.5f,

    -0.5f,  0.5f, -0.5f,
     0.5f,  0.5f, -0.5f,
     0.5f,  0.5f,  0.5f,
     0.5f,  0.5f,  0.5f,
    -0.5f,  0.5f,  0.5f,
    -0.5f,  0.5f, -0.5f
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
void processInput(GLFWwindow* window); // 处理输入
void myTransform(glm::vec3 translate, float angelX, float angelY, float angelZ, Shader &shader); // 变换

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
    Shader shaderColorCube("Shaders/1.color.vert", "Shaders/1.color.frag");
    Shader shaderPlane("Shaders/3DTexture.vert", "Shaders/plane.frag");

    /* set up vertex data and buffers and configure vertex attributes */
    /*----------------------------------------------------------------*/
    /* Cube data set */
    /*----------------*/
    unsigned int VBOCub, VAOColorCube, VAOLightCube;
    glGenBuffers(1, &VBOCub);
    glGenVertexArrays(1, &VAOColorCube);

    glBindVertexArray(VAOColorCube);
    glBindBuffer(GL_ARRAY_BUFFER, VBOCub);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    /* 设置顶点position属性 */
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glad_glEnableVertexAttribArray(0);

    glGenVertexArrays(1, &VAOLightCube);
    glBindVertexArray(VAOLightCube);
    /* 在Attribute Pointer前需要Bind VBOCub */
    glBindBuffer(GL_ARRAY_BUFFER, VBOCub);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glad_glEnableVertexAttribArray(0);

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
    float angel[] = {0.0f, -25.0f, 0.0f};
    float colorLight[3] = {1.0f, 1.0f, 1.0f};
    glm::vec3 translate = glm::vec3(0.0f, 0.0f, 0.0f);

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

    /* Render Loop 渲染循环 */
    /*---------------------*/
    while(!glfwWindowShouldClose(window)){
        /* input */
        processInput(window); 

        /* render */
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // 清除颜色缓冲&深度缓冲

        /* Coordinate Settings 坐标系统设置 */
        /*--------------------------------*/
        glm::mat4 view = glm::mat4(1.0f);       // 观察矩阵
        glm::mat4 projection = glm::mat4(1.0f); // 投影矩阵
        glm::mat4 model = glm::mat4(1.0f);      // 模型矩阵
        //model = glm::rotate(model, glm::radians(-55.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        // 初始视口在原点，要看见图形，相机要向+z移动，相当于世界坐标向-z移动
        view = glm::translate(view, glm::vec3(0.0f, -1.0f, -7.0f));
        projection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH/(float)SCR_HEIGHT, 0.1f, 100.0f); //45度透视投影

        /* draw color cube*/
        /* Be sure use shader before */
        shaderColorCube.use();
        shaderColorCube.setMat4("model", model);
        shaderColorCube.setMat4("view", view);
        shaderColorCube.setMat4("projection", projection);
        shaderColorCube.setVec3("colorObject", 1.0f, 0.5f, 0.31f);
        shaderColorCube.setVec3("colorLight", colorLight[0], colorLight[1], colorLight[2]);

        glBindVertexArray(VAOColorCube);
        myTransform(translate, angel[0], angel[1], angel[2], shaderColorCube);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        /* draw Lighting cube */
        shaderLightCube.use();
        shaderLightCube.setMat4("view", view);
        shaderLightCube.setMat4("projection", projection);

        static glm::vec3 lightPosition = glm::vec3(1.2f, 1.0f, 2.0f);
        model = glm::mat4(1.0f);
        model = glm::translate(model, lightPosition);
        model = glm::rotate(model, glm::radians(-25.f), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::scale(model, glm::vec3(0.2f));
        shaderLightCube.setMat4("model", model);
        shaderLightCube.setVec3("colorLight", colorLight[0], colorLight[1], colorLight[2]);
        //glBindVertexArray(VAOLightCube);
        glBindVertexArray(VAOLightCube);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        /* draw plane */
        //texturePlane.use();
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

        /* draw imgui */
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        ImGui::Begin("Configure"); {
            ImGui::ColorEdit3("Light color", colorLight);
            ImGui::Text("Rotate");
            ImGui::SliderFloat3("Rotate angel", angel, -90.0f, 90.0f);
            ImGui::Text("Translate");
            ImGui::SliderFloat3("Move color cube", value_ptr(translate), -1.5f, 1.5f);
            ImGui::SliderFloat3("Move light cube", value_ptr(lightPosition), -3.0f, 3.0f);
        } ImGui::End();

        ImGui::Render(); // rendering
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window); // 交换前后缓冲
        glfwPollEvents(); // poll events 回调事件
    }

    /* optional: 超出生存期，de-allocate所有资源 */
    glDeleteBuffers(1, &VBOCub);
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

void processInput(GLFWwindow* window){
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS){
        glfwSetWindowShouldClose(window, true);
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