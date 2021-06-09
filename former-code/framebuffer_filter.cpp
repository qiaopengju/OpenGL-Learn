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
const unsigned int SCR_WIDTH = 1000;
const unsigned int SCR_HEIGHT = 1000;
Camera camera(glm::vec3(0.0f, 1.0f, 5.0f)); // 全局相机

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
    Shader shaderSkyBox("Shaders/skybox.vert", "Shaders/skybox.frag");
    Shader shaderReflect("Shaders/lightingMap.vert", "Shaders/reflect.frag");
    Shader shaderRefract("Shaders/lightingMap.vert", "Shaders/refract.frag");
    Shader shaderTexture2D("Shaders/framebuffer.vert", "Shaders/framebuffer.frag");
    /* model */
    Model nanosuitModel("Resource/Model/nanosuit/nanosuit.obj");
    /* texture */

    /* skybox */
    Skybox skybox("Resource/Img/skybox/");
    /* framebuffer */
    Framebuffer frameBuffer;
    shaderTexture2D.use();
    shaderTexture2D.setInt("frameTexture", 0);

    /* set up vertex data and buffers and configure vertex attributes */
    /*----------------------------------------------------------------*/
    /* Skybox data set*/
    /*----------------*/
    shaderSkyBox.use();
    shaderSkyBox.setInt("skybox", 0);

    /* Cube data set */
    /*----------------*/
    unsigned int VBOCube, VAOColorCube, VAOLightCube;
    glGenBuffers(1, &VBOCube);
    glGenVertexArrays(1, &VAOColorCube);

    glBindVertexArray(VAOColorCube);
    glBindBuffer(GL_ARRAY_BUFFER, VBOCube);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    /* 设置顶点position属性 */
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glad_glEnableVertexAttribArray(0);
    /* 设置顶点所在面法向量属性 */
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glad_glEnableVertexAttribArray(1);
    /* 设置顶点贴图坐标属性 */
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glad_glEnableVertexAttribArray(2);
    /* lighting map texture */

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
    
    /* 设置变换 */
    //float angel[] = {0.0f, 25.0f, 0.0f};
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
    bool showColorCube = true;
    bool showModel = false;
    bool reverseColor = false;
    bool grayscale = false;
    bool blur = false;

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

        /* render in texture, which bind to framebuffer */
        glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer.ID);
        glEnable(GL_DEPTH_TEST);

        //glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // 清除颜色缓冲&深度缓冲&模板测试缓冲

        /* Coordinate Settings 坐标系统设置 */
        /*--------------------------------*/
        glm::mat4 view = camera.view;       // 观察矩阵
        glm::mat4 projection = glm::mat4(1.0f); // 投影矩阵
        glm::mat4 model = glm::mat4(1.0f);      // 模型矩阵
        //model = glm::rotate(model, glm::radians(-55.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        // 初始视口在原点，要看见图形，相机要向+z移动，相当于世界坐标向-z移动
        projection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH/(float)SCR_HEIGHT, 0.1f, 100.0f); //45度透视投影

        /* draw color cube */
        /*-----------------*/
        if (showColorCube){
            shaderReflect.use();
            model = glm::mat4(1.0);
            shaderReflect.setMat4("model", model);
            shaderReflect.setMat4("view", view);
            shaderReflect.setMat4("projection", projection);
            shaderReflect.setVec3("cameraPos", camera.cameraPos);
            shaderReflect.setInt("skybox", 0);
            glActiveTexture(GL_TEXTURE0);
            skybox.use();
            glBindVertexArray(VAOColorCube);
            glDrawArrays(GL_TRIANGLES, 0, 36);
            glBindVertexArray(0);
        }

        /* draw model */
        /*------------*/
        if (showModel){
            //shaderColorCube.use();
            shaderRefract.use();
            model = glm::mat4(1.0f);
            model = glm::translate(model, glm::vec3(0, 0.5f, 0.f));
            model = glm::scale(model, glm::vec3(0.1f));
            shaderRefract.setMat4("model", model);
            shaderRefract.setMat4("view", view);
            shaderRefract.setMat4("projection", projection);
            shaderRefract.setVec3("cameraPos", camera.cameraPos);
            shaderRefract.setInt("skybox", 0);
            glActiveTexture(GL_TEXTURE0);
            skybox.use();
            nanosuitModel.Draw(shaderRefract);
        }

        // draw skybox at last !
        glDepthFunc(GL_LEQUAL);
        shaderSkyBox.use();
        view = glm::mat4(glm::mat3(camera.getView()));  // remove translation form the view matrix
        shaderSkyBox.setMat4("view", view);
        shaderSkyBox.setMat4("projection", projection);
        skybox.Draw(shaderSkyBox);
        glDepthFunc(GL_LESS);

        // render in window
        //-----------------
        shaderTexture2D.use();
        shaderTexture2D.setFloat("width", SCR_WIDTH);
        shaderTexture2D.setFloat("height", SCR_HEIGHT);
        frameBuffer.Draw(shaderTexture2D);

        /* draw imgui */
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        ImGui::Begin("Configure"); {
            ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "Press TAB to use mouse/camera!");
            ImGui::SameLine(); HelpMarker("Key", " ESC:exit\n W: forward\n A: left\n S: back\n D: right\n SPACE: up");
            ImGui::BulletText("Show Objects");
            ImGui::Checkbox("Color Box", &showColorCube);
            ImGui::Checkbox("Model", &showModel);
            ImGui::BulletText("Filter");
            ImGui::Checkbox("Reverse Color", &reverseColor);
            ImGui::Checkbox("Gray scale", &grayscale);
            ImGui::Checkbox("Blur", &blur);
        } ImGui::End();
        ImGui::Begin("Debug");{
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