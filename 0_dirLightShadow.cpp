#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include "Camera.h"
#include "Light.h"
#include "Skybox.h"
#include "ResourceManager.h"
#include "BasicObjectRender.h"

using namespace std;

/* setting */
const unsigned int SCR_WIDTH = 1000;
const unsigned int SCR_HEIGHT = 1000;
Camera camera(glm::vec3(0.0f, 1.0f, 5.0f)); // 全局相机

void framebuffer_size_callback(GLFWwindow* window, int width, int height); // 窗口大小改变时回调该函数
void mouse_callback(GLFWwindow *window, double xpos, double ypos); // 光标移动回调
void processInput(GLFWwindow* window, Camera &camera, float deltaTime); // 处理输入
void HelpMarker(const char *title, const char* desc); // imGui 显示帮助

int main(){
    /* glfw: initalize and cofigure */
    /*------------------------------*/
    glfwInit(); //init
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); //major version 3.3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); //minor version 3.3
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); //core profile
    glfwWindowHint(GLFW_SAMPLES, 4); // 多重采样，抗锯齿
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
    /* 开启深度测试, 之后在渲染循环中需清除深度缓冲(DEPTH_BUFFER_BIT) enable Z-buffer */
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_MULTISAMPLE);

    /* Resource manager */
    /* build and compile shader */
    /*--------------------------*/
    ResourceManager::LoadShader("Shaders/skybox.vert", "Shaders/skybox.frag", "skybox");
    ResourceManager::LoadShader("Shaders/0.Light.vert", "Shaders/0.Light.frag", "shadow");
    ResourceManager::LoadShader("Shaders/0.depthMap.vert", "Shaders/0.depthMap.frag", "depthMap");
    ResourceManager::LoadShader("Shaders/framebuffer.vert", "Shaders/framebuffer.frag", "debug");
    //ResourceManager::LoadShader("Shaders/showNormal.vert", "Shaders/showNormal.frag", "Shaders/showNormal.geom", "shadow");
    /* model */
    ResourceManager::LoadModel("Resource/Model/cube/cube.obj", "cube");
    /* texture */
    ResourceManager::LoadTexture2D("Resource/Img/woodfloor/color.jpg", "wood_color");
    ResourceManager::LoadTexture2D("Resource/Img/woodfloor/normal.jpg", "wood_normal");
    ResourceManager::LoadTexture2D("Resource/Img/metal/color.jpg", "plane_color");
    ResourceManager::LoadTexture2D("Resource/Img/metal/normal.jpg", "plane_normal");
    ResourceManager::LoadTexture2D("Resource/Img/write.png", "null");
    ResourceManager::LoadTextureCubemap("Resource/Img/skybox/", "skybox");

    /* set up vertex data and buffers and configure vertex attributes */
    /*----------------------------------------------------------------*/
    /* Skybox data set*/
    /*----------------*/
    Skybox skybox("skybox", ResourceManager::getTextureCubemap("skybox"));
    ResourceManager::getShader("skybox").use();
    ResourceManager::getShader("skybox").setInt("skybox", 0);

    /* 设置光照参数 */
    ResourceManager::getShader("shadow").use();
    ResourceManager::getShader("shadow").setBool("showDirLight", true);
    ResourceManager::getShader("shadow").setBool("showPointLight", false);
    ResourceManager::getShader("shadow").setBool("showSpotLight", false);
    ResourceManager::getShader("shadow").setInt("material.texture_diffuse1", 0);
    ResourceManager::getShader("shadow").setInt("material.texture_specular1", 1);
    ResourceManager::getShader("shadow").setInt("material.texture_normal1", 2);
    ResourceManager::getShader("shadow").setInt("depthMap", 3);
    //debug
    //ResourceManager::getShader("shadow").setFloat("MAGNITUDE", 0.2f);
    /* material */
    int shininess(32); // 物体反光度
    /* directional light */
    DirLight dirLight(glm::vec3(0.1f), glm::vec3(1.0f), glm::vec3(1.0f), glm::vec3(-0.6f, -1.7f, 1.0f));
    glm::vec3 direct = dirLight.direction;
    /* point light */
    int pointLightNum = 4;
    PointLight pointLight(glm::vec3(0.05f), glm::vec3(0.8f), glm::vec3(1.0f));
    glm::vec3 pointLightPositions[] = {
        glm::vec3(0.0f, 1.5f, 0.0),
        glm::vec3(2.0f, 0.0f, 1.0),
        glm::vec3(-1.0f, 0.0f, 2.0),
    };
    /* spot light */
    int spotLightNum = 1;
    SpotLight spotLight(glm::vec3(0), glm::vec3(1.0f), glm::vec3(1.0f), camera.cameraPos, camera.cameraFront);
    float cutOff(12.5f), outerCutOff(15.f);

    // framebuffer
    const float depthWidth(2000), depthHeight(2000);
    unsigned int depthMapFBO, depthTexture;
    glGenFramebuffers(1, &depthMapFBO);
    glGenTextures(1, &depthTexture);
    glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
    glBindTexture(GL_TEXTURE_2D, depthTexture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    GLfloat borderColor[] = {1.0, 1.0, 1.0};
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor); // 设置边界深度为1，所有超出边界的图形都不在阴影中
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, depthWidth, depthHeight, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthTexture, 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glBindTexture(GL_TEXTURE_2D, 0);

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
    bool showColorCube = true;
    bool showModel = false;
    bool depthMapDebug = true;

    CubeRender cube;

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
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT); // 清除颜色缓冲&深度缓冲&模板测试缓冲
        glEnable(GL_DEPTH_TEST);

        /* Coordinate Settings 坐标系统设置 */
        /*--------------------------------*/
        glm::mat4 view = camera.view;       // 观察矩阵
        glm::mat4 projection = glm::mat4(1.0f); // 投影矩阵
        glm::mat4 model = glm::mat4(1.0f);      // 模型矩阵
        // 初始视口在原点，要看见图形，相机要向+z移动，相当于世界坐标向-z移动
        projection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH/(float)SCR_HEIGHT, 0.1f, 100.0f); //45度透视投影

        /* depth framebuffer */
        /*-------------------*/
        /*-------------------*/
        glViewport(0, 0, depthWidth, depthHeight);
        glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
        glClear(GL_DEPTH_BUFFER_BIT); // 清除颜色缓冲&深度缓冲&模板测试缓冲
        glm::mat4 lightView = glm::lookAt(-direct, glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        //glm::mat4 depthMatrix = glm::lookAt(, dirLight.direction, glm::vec3(0.0, 1.0, 0.0));
        glm::mat4 lightProjection = glm::ortho(-10.0, 10.0, -10.0, 10.0, 0.1, 10.0);
        glm::mat4 depthMatrix = lightProjection * lightView;
        ResourceManager::getShader("depthMap").use();
        ResourceManager::getShader("depthMap").setMat4("depthMatrix", depthMatrix);

        /* draw plane */
        /*------------*/
        if (showPlane){
            ResourceManager::getShader("depthMap").use();
            model = glm::mat4(1.0f);
            model = glm::translate(model, glm::vec3(0.0f, -0.5f, 0.0f));
            model = glm::scale(model, glm::vec3(10.0f, 0.1f, 10.0f));
            ResourceManager::getShader("depthMap").setMat4("model", model);

            ResourceManager::getModel("cube").Draw(ResourceManager::getShader("depthMap"));
            //cube.Draw();
        }

        /* draw color cube */
        /*-----------------*/
        if (showColorCube){
            ResourceManager::getShader("depthMap").use();
            for (int i = 0; i < 3; i++){
                model = glm::mat4(1.0);
                model = glm::translate(model, pointLightPositions[i]);
                model = glm::scale(model, glm::vec3(0.5f));
                ResourceManager::getShader("depthMap").setMat4("model", model);
                ResourceManager::getModel("cube").Draw(ResourceManager::getShader("depthMap"));
            }
            //cube.Draw();
        }
        /* render as usual */
        /*-----------------*/
        /*-----------------*/
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT); // 清除颜色缓冲&深度缓冲&模板测试缓冲
        glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);

        static Spirit2DRender quat;
        ResourceManager::getShader("debug").use();
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, depthTexture);
        ResourceManager::getShader("debug").setInt("frameTexture", 0);
        ResourceManager::getShader("debug").setFloat("width", SCR_WIDTH);
        ResourceManager::getShader("debug").setFloat("height", SCR_HEIGHT);
        if (depthMapDebug){
            quat.Draw();
            glEnable(GL_DEPTH_TEST);
        }

        /* Set up Shader */
        ResourceManager::getShader("shadow").use();
        ResourceManager::getShader("shadow").setMat4("depthMatrix", depthMatrix);
        ResourceManager::getShader("shadow").setMat4("view", view);
        ResourceManager::getShader("shadow").setMat4("projection", projection);
        ResourceManager::getShader("shadow").setVec3("viewPosition", camera.cameraPos);
        // directional light
        //ResourceManager::getShader("shadow").setVec3("directionalLight.direction", dirLight.direction);
        ResourceManager::getShader("shadow").setVec3("directionalLight.direction", direct);
        ResourceManager::getShader("shadow").setVec3("directionalLight.ambient", dirLight.ambient);
        ResourceManager::getShader("shadow").setVec3("directionalLight.diffuse", dirLight.diffuse);
        ResourceManager::getShader("shadow").setVec3("directionalLight.specular", dirLight.specular);
        ResourceManager::getShader("shadow").setInt("pointLightNum", pointLightNum);

        /* draw plane */
        /*------------*/
        if (showPlane){
            ResourceManager::getShader("shadow").use();
            model = glm::mat4(1.0f);
            model = glm::translate(model, glm::vec3(0.0f, -0.5f, 0.0f));
            model = glm::scale(model, glm::vec3(10.0f, 0.1f, 10.0f));
            ResourceManager::getShader("shadow").setMat4("model", model);

            glActiveTexture(GL_TEXTURE0);
            ResourceManager::getTexture2D("plane_color").bind();
            glActiveTexture(GL_TEXTURE1);
            ResourceManager::getTexture2D("null").bind();
            glActiveTexture(GL_TEXTURE2);
            ResourceManager::getTexture2D("plane_normal").bind();
            ResourceManager::getShader("shadow").setInt("material.shininess", 5);
            glActiveTexture(GL_TEXTURE3);
            glBindTexture(GL_TEXTURE_2D, depthTexture);
            
            ResourceManager::getModel("cube").Draw(ResourceManager::getShader("shadow"));
            //cube.Draw();
        }

        /* draw color cube */
        /*-----------------*/
        if (showColorCube){
            ResourceManager::getShader("shadow").use();
            glActiveTexture(GL_TEXTURE0);
            ResourceManager::getTexture2D("wood_color").bind();
            glActiveTexture(GL_TEXTURE1);
            ResourceManager::getTexture2D("null").bind();
            glActiveTexture(GL_TEXTURE2);
            ResourceManager::getTexture2D("wood_normal").bind();
            ResourceManager::getShader("shadow").setInt("material.shininess", shininess);
            glActiveTexture(GL_TEXTURE3);
            glBindTexture(GL_TEXTURE_2D, depthTexture);

            for (int i = 0; i < 3; i++){
                model = glm::mat4(1.0);
                model = glm::translate(model, pointLightPositions[i]);
                model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
                ResourceManager::getShader("shadow").setMat4("model", model);
                ResourceManager::getModel("cube").Draw(ResourceManager::getShader("shadow"));
            }
            //cube.Draw();
        }

        // draw skybox at last !
        //glDepthFunc(GL_LEQUAL);
        //ResourceManager::getShader("skybox").use();
        //view = glm::mat4(glm::mat3(camera.getView()));  // remove translation form the view matrix
        //ResourceManager::getShader("skybox").setMat4("view", view);
        //ResourceManager::getShader("skybox").setMat4("projection", projection);
        //skybox.Draw(ResourceManager::getShader("skybox"));
        //glDepthFunc(GL_LESS);

        /* draw imgui */
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        ImGui::Begin("Configure"); {
            ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "Press TAB to use mouse/camera!");
            ImGui::SameLine(); HelpMarker("Key", " ESC:exit\n W: forward\n A: left\n S: back\n D: right\n SPACE: up");
            ImGui::BulletText("Show Objects");
            ImGui::Checkbox("Color Box", &showColorCube);
            ImGui::Checkbox("Plane", &showPlane);
            ImGui::Checkbox("Model", &showModel);
            ImGui::SliderFloat3("light direction", glm::value_ptr(direct), -5, 5);
        } ImGui::End();
        ImGui::Begin("Debug");{
            ImGui::Checkbox("depth map framebuffer", &depthMapDebug);
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