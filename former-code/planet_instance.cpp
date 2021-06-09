#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include "Shader.h"
#include "Texture.h"
#include "Camera.h"
#include "Light.h"
#include "Model.h"
#include "Skybox.h"
#include "Framebuffer.h"

using namespace std;

/* setting */
const unsigned int SCR_WIDTH = 1300;
const unsigned int SCR_HEIGHT = 1000;
Camera camera(glm::vec3(0.0f, 0.0f, 5.0f)); // 全局相机

float vertices[] = {
    /*----positions----|------normals-------|--texture coords-*/
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
     0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,

    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,

    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
    -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,

    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f
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
glm::vec3 cubePositions[] = {
    glm::vec3( 0.0f,  0.0f,  0.0f),
    glm::vec3( 2.0f,  5.0f, -15.0f),
    glm::vec3(-1.5f, -2.2f, -2.5f),
    glm::vec3(-3.8f, -2.0f, -12.3f),
    glm::vec3( 2.4f, -0.4f, -3.5f),
    glm::vec3(-1.7f,  3.0f, -7.5f),
    glm::vec3( 1.3f, -2.0f, -2.5f),
    glm::vec3( 1.5f,  2.0f, -2.5f),
    glm::vec3( 1.5f,  0.2f, -1.5f),
    glm::vec3(-1.3f,  1.0f, -1.5f)
};
glm::vec3 pointLightPositions[] = {
    glm::vec3( 0.7f,  0.2f,  2.0f),
    glm::vec3( 2.3f, -3.3f, -4.0f),
    glm::vec3(-4.0f,  2.0f, -12.0f),
    glm::vec3( 0.0f,  0.0f, -3.0f)
};

void framebuffer_size_callback(GLFWwindow* window, int width, int height); // 窗口大小改变时回调该函数
void mouse_callback(GLFWwindow *window, double xpos, double ypos); // 光标移动回调
void processInput(GLFWwindow* window, Camera &camera, float deltaTime); // 处理输入
glm::mat4 myTransform(glm::vec3 translate, float angelX, float angelY, float angelZ, Shader &shader); // 变换
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
    /* GLOBAL OpenGL state */
    /*---------------------*/
    /* 开启深度测试, 之后在渲染循环中需清除深度缓冲(DEPTH_BUFFER_BIT) enable Z-buffer */
    glEnable(GL_DEPTH_TEST);

    /* build and compile shader */
    /*--------------------------*/
    Shader shaderModel("Shaders/lightingMap.vert", "Shaders/Light.frag");
    Shader shaderInstance("Shaders/instance.vert", "Shaders/Light.frag");

    /* model */
    /*-------*/
    Model planet("Resource/Model/planet/planet.obj");
    Model rock("Resource/Model/rock/rock.obj");

    /* texture */
    /*---------*/

    /* skybox */
    /*--------*/

    /* framebuffer */
    /*-------------*/

    /* set up vertex data and buffers and configure vertex attributes */
    /*----------------------------------------------------------------*/
    /* 设置位移 */
    unsigned int amount = 1000;
    float radius(30.0), offset(15);
    glm::mat4 *modelMatrices;
    modelMatrices = new glm::mat4[amount];
    srand(glfwGetTime());
    for (int i = 0; i < amount; i++){
        glm::mat4 model(1.0);
        // 1.位移，分布在半径为radius的圆上，位移范围是[-offset, offset]
        float angel = (float)i / (float)amount * 360.f;
        float displacement = (rand() % (int)(2 * offset * 100)) / 100 - offset;
        float x = cos(angel) * radius + displacement;
        displacement = (rand() % (int)(2 * offset * 100)) / 100 - offset;
        float y = displacement * 0.4f; // 高度y要比x和z小
        displacement = (rand() % (int)(2 * offset * 100)) / 100 - offset;
        float z = sin(angel) * radius + displacement;
        model = glm::translate(model, glm::vec3(x, y, z));
        // 2.缩放，在[0.05, 0.15]之间缩放
        float scale = (rand() % 10) / 100.f + 0.05;
        model = glm::scale(model, glm::vec3(scale));
        // 3.绕轴旋转任意角
        float rotAngel = rand() % 360;
        model = glm::rotate(model, rotAngel, glm::vec3(0.4f, 0.6f, 0.8f));
        // 4.添加到model矩阵中
        modelMatrices[i] = model;
    }
    // 设置实例化缓冲
    unsigned int instanceBuffer;
    glGenBuffers(1, &instanceBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, instanceBuffer);
    glBufferData(GL_ARRAY_BUFFER, amount * sizeof(glm::mat4), &modelMatrices[0], GL_STATIC_DRAW);
    for (int i = 0; i < rock.meshes.size(); i++){
        unsigned int VAO = rock.meshes[i].VAO;
        glBindVertexArray(VAO);
        // 顶点属性
        GLsizei vec4Size = sizeof(glm::vec4);
        // 顶点着色器输入为mat4，但着色器最大允许属性为vec4，故我们设置4个vec4
        glEnableVertexAttribArray(3);
        glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)0);
        glEnableVertexAttribArray(4);
        glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)(vec4Size));
        glEnableVertexAttribArray(5);
        glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)(2 * vec4Size));
        glEnableVertexAttribArray(6);
        glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)(3 * vec4Size));
        
        glVertexAttribDivisor(3, 1);
        glVertexAttribDivisor(4, 1);
        glVertexAttribDivisor(5, 1);
        glVertexAttribDivisor(6, 1);

        glBindVertexArray(0);
    }

    /* 设置变换 */
    glm::vec3 translate = glm::vec3(0.0f, 0.0f, 0.0f);

    /* directional light */
    DirLight dirLight(glm::vec3(0.04f), glm::vec3(0.9f), glm::vec3(0.5f), glm::vec3(-1.0f, 0.0f, 0.0f));

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


        glClearColor(0.05f, 0.05f, 0.06f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // 清除颜色缓冲&深度缓冲&模板测试缓冲

        /* Coordinate Settings 坐标系统设置 */
        /*--------------------------------*/
        glm::mat4 view = camera.view;       // 观察矩阵
        glm::mat4 projection = glm::mat4(1.0f); // 投影矩阵
        glm::mat4 model = glm::mat4(1.0f);      // 模型矩阵
        //model = glm::rotate(model, glm::radians(-55.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        // 初始视口在原点，要看见图形，相机要向+z移动，相当于世界坐标向-z移动
        projection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH/(float)SCR_HEIGHT, 0.1f, 400.0f); //45度透视投影

        /* draw planet */
        shaderModel.use();
        shaderModel.setMat4("model", model);
        shaderModel.setMat4("view", view);
        shaderModel.setMat4("projection", projection);
        shaderModel.setVec3("viewPosition", camera.cameraPos);
        shaderModel.setVec3("directionalLight.direction", dirLight.direction);
        shaderModel.setVec3("directionalLight.ambient", dirLight.ambient);
        shaderModel.setVec3("directionalLight.diffuse", dirLight.diffuse);
        shaderModel.setVec3("directionalLight.specular", dirLight.specular);
        shaderModel.setInt("material.shininess", 32);
        shaderModel.setInt("pointLightNum", 0);

        shaderModel.setBool("showDirLight", true);
        shaderModel.setBool("showPointLight", false);
        shaderModel.setBool("showSpotLight", false);
        // draw planet
        model = glm::translate(model, glm::vec3(7.f, 0, 0));
        shaderModel.setMat4("model", model);
        planet.Draw(shaderModel);
        // draw rock
        shaderInstance.use();
        shaderInstance.setMat4("view", view);
        shaderInstance.setMat4("projection", projection);
        shaderInstance.setVec3("viewPosition", camera.cameraPos);
        shaderInstance.setVec3("directionalLight.direction", dirLight.direction);
        shaderInstance.setVec3("directionalLight.ambient", dirLight.ambient);
        shaderInstance.setVec3("directionalLight.diffuse", dirLight.diffuse);
        shaderInstance.setVec3("directionalLight.specular", dirLight.specular);
        shaderInstance.setInt("material.shininess", 32);
        shaderInstance.setInt("pointLightNum", 0);

        shaderInstance.setBool("showDirLight", true);
        shaderInstance.setBool("showPointLight", false);
        shaderInstance.setBool("showSpotLight", false);
        for (int i = 0; i < rock.meshes.size(); i++){
            glBindVertexArray(rock.meshes[i].VAO);
            glDrawElementsInstanced(GL_TRIANGLES, rock.meshes[i].indices.size(), GL_UNSIGNED_INT, 0, amount);
        }

        /* draw imgui */
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        ImGui::Begin("Configure"); {
            ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "Press TAB to use mouse/camera!");
            ImGui::SameLine(); HelpMarker("Key", " ESC:exit\n W: forward\n A: left\n S: back\n D: right\n SPACE: up");
        } ImGui::End();
        ImGui::Begin("Debug");{
        } ImGui::End();

        ImGui::Render(); // rendering
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window); // 交换前后缓冲
        glfwPollEvents(); // poll events 回调事件
    }

    /* optional: 超出生存期，de-allocate所有资源 */

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

glm::mat4 myTransform(glm::vec3 move, float angelX, float angelY, float angelZ, Shader &shader){ // 对物体变换
    glm::mat4 translate = glm::mat4(1.0f);
    glm::mat4 rotate = glm::mat4(1.0f);
    translate = glm::translate(translate, move);
    rotate = glm::rotate(rotate, glm::radians(angelX), glm::vec3(1.0f, 0.0f, 0.0f));
    rotate = glm::rotate(rotate, glm::radians(angelY), glm::vec3(0.0f, 1.0f, 0.0f));
    rotate = glm::rotate(rotate, glm::radians(angelZ), glm::vec3(0.0f, 0.0f, 1.0f));
    /* 将变换矩阵传给着色器 */
    shader.setMat4("model", translate * rotate);
    return translate * rotate;
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