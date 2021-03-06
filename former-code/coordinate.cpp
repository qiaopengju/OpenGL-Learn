#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include "deps/stb_image.h"  // image depends lib
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include "Shader.h"
#include "Camera.h"

using namespace std;

/* setting */
const unsigned int SCR_WIDTH = 1000;
const unsigned int SCR_HEIGHT = 1000;
float vertices[] = {
    //----position-----|--texture--
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
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

void framebuffer_size_callback(GLFWwindow* window, int width, int height); // ????????????????????????????????????
void processInput(GLFWwindow* window); // ????????????
void loadJpeg(const char* filename); // ??????Jpeg, color: RGB
void loadPNG(const char* filename); // ??????png, color: RGBA
void myTransform(glm::vec3 move, float angel, Shader &shader); // ???????????????

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
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback); //????????????????????????????????????

    /* glad: load all OpenGL function pointers */
    /*-----------------------------------------*/
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){ //GLAD
        cout << "Failed to initialize GLAD" << endl;
        return -1;
    }

    /* build and compile shader */
    /*--------------------------*/
    Shader shader("Shaders/3DTexture.vert", "Shaders/2DTextureNoColor.frag");
    shader.use();

    /* Coordinate Settings ?????????????????? */
    /*--------------------------------*/
    glm::mat4 model = glm::mat4(1.0f);      // ????????????
    glm::mat4 view = glm::mat4(1.0f);       // ????????????
    glm::mat4 projection = glm::mat4(1.0f); // ????????????
    //model = glm::rotate(model, glm::radians(-55.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    // ??????????????????????????????????????????????????????+z?????????????????????????????????-z??????
    view = glm::translate(view, glm::vec3(0.0f, 0.0f, -5.0f));
    projection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH/(float)SCR_HEIGHT, 0.1f, 100.0f); //45???????????????
    shader.setMat4("model", model);
    shader.setMat4("view", view);
    shader.setMat4("projection", projection);
    /* ??????????????????, ?????????????????????????????????????????????(DEPTH_BUFFER_BIT) enable Z-buffer */
    glEnable(GL_DEPTH_TEST);

    /* set up vertex data and buffers and configure vertex attributes */
    /*----------------------------------------------------------------*/
    unsigned int VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    //glGenBuffers(1, &EBO);
    /* bind the Vertex Array Object first, then bind and set vertex buffers, and then configure vertex attrtibutes */
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    //glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    /* ????????????position?????? */
    //glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glad_glEnableVertexAttribArray(0);
    /* ????????????texture?????? */
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glad_glEnableVertexAttribArray(1);

    /* load and create texture */
    /*-------------------------*/
    unsigned int texture1, texture2;
    /* texuter1 */
    glGenTextures(1, &texture1);
    glBindTexture(GL_TEXTURE_2D, texture1);
    /* set the texture wrapping parameters ???????????????????????????????????????*/
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    /* set teh texture filtering paramenters ?????????????????? */
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    /* load image */
    loadJpeg("../Resource/Img/container.jpeg");

    /* texture 2 */
    glGenTextures(1, &texture2);
    glBindTexture(GL_TEXTURE_2D, texture2);
    /* set the texture wrapping parameters ???????????????????????????????????????*/
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    /* set teh texture filtering paramenters ?????????????????? */
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    /* load image */
    loadPNG("../Resource/Img/awesomeface.png");

    /* ?????????????????? */
    shader.use();
    shader.setInt("texture1", 0); //uniform sampler2D?????????, texture1?????????0???????????????
    shader.setInt("texture2", 1);

    /* ???????????? */
    float angel(0);
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

    /* Render Loop ???????????? */
    /*---------------------*/
    while(!glfwWindowShouldClose(window)){
        /* input */
        processInput(window); 

        /* render */
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // ??????????????????&????????????

        /* draw triangle */
        shader.use();
        glActiveTexture(GL_TEXTURE0); //??????texture?????????????????????
        glBindTexture(GL_TEXTURE_2D, texture1);
        glActiveTexture(GL_TEXTURE1); //??????texture?????????????????????
        glBindTexture(GL_TEXTURE_2D, texture2);
        glBindVertexArray(VAO);
        //glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        for (int i = 0; i < 10; i++){
            myTransform(cubePositions[i]+translate, angel * 20 * (i+1), shader);
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        /* draw imgui */
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        ImGui::Begin("Setting"); {
            ImGui::Text("Rotate");
            ImGui::SliderFloat("Rotate angel", &angel, -18.0f, 18.0f);
            ImGui::Text("Translate");
            ImGui::SliderFloat("X", &translate.x, -1.0f, 1.0f);
            ImGui::SliderFloat("Y", &translate.y, -1.0f, 1.0f);
            ImGui::SliderFloat("Z", &translate.z, -5.0f, 5.0f);
        } ImGui::End();

        ImGui::Render(); // rendering
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window); // ??????????????????
        glfwPollEvents(); // poll events ????????????
    }

    /* optional: ??????????????????de-allocate???????????? */
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    /* glfw: release resource */
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate(); 
    return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height){ //????????????????????????????????????
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window){
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS){
        glfwSetWindowShouldClose(window, true);
    }
}

void loadJpeg(const char* filename){ // ????????????
    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true); // ????????????????????????
    unsigned char *data = stbi_load(filename, &width, &height, &nrChannels, 0);
    if (data){
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data); // *RGB
        glGenerateMipmap(GL_TEXTURE_2D); // ??????????????????
    } else{
        std::cout << "FAILED TO LOAD TEXTURE\n";
    }
    stbi_image_free(data); // ??????????????????
}

void loadPNG(const char* filename){ // ????????????
    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true); // ????????????????????????
    unsigned char *data = stbi_load(filename, &width, &height, &nrChannels, 0);
    if (data){
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data); // *RGBA
        glGenerateMipmap(GL_TEXTURE_2D); // ??????????????????
    } else{
        std::cout << "FAILED TO LOAD TEXTURE\n";
    }
    stbi_image_free(data); // ??????????????????
}

void myTransform(glm::vec3 move, float angel, Shader &shader){ // ???????????????
    glm::mat4 translate = glm::mat4(1.0f);
    glm::mat4 rotate = glm::mat4(1.0f);
    translate = glm::translate(translate, move);
    rotate = glm::rotate(rotate, glm::radians(angel), glm::vec3(1.0f, 0.5f, 0.2f));
    /* ?????????????????????????????? */
    shader.setMat4("model", translate * rotate);
}