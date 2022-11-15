#include <iostream>
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "Window/window.hpp"
#include "Logical/logical.hpp"

int main(int argc, char* argv[]);
// 注册一个回调函数，设置改变原视口的大小
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
// 处理按键输入
void processInput(GLFWwindow *window);
// 检查着色器编译是否出现问题
void compileError(unsigned int);
int main(int argc, char* argv[])
{
    // 初始化GLFW
    glfwInit();
    // 配置GLFW主版本号
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    // 配置GLFW次版本号
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    // 使用opengl核心模式
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    // 创建窗口对象
    GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    // 设置当前线程的上下文
    glfwMakeContextCurrent(window);
    //调用glfw函数之前，初始化glad
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }
    // 设置opengl渲染视口的大小
    glViewport(0, 0, 800, 600);
    // 注册函数，设置当调整大小的时候调用这个回调函数
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // 创建VAO
    unsigned int VAO;
    glGenVertexArrays(1, &VAO);
    // 绑定VAO
    glBindVertexArray(VAO);

    // 定义三角形的位置坐标
    float vertices[] = {
            0.5f, 0.5f, 0.0f,   // 右上角
            0.5f, -0.5f, 0.0f,  // 右下角
            -0.5f, -0.5f, 0.0f, // 左下角
            -0.5f, 0.5f, 0.0f   // 左上角
    };
    // 定义三角形采用的三个位置
    unsigned int indices[] = {
            // 注意索引从0开始!
            // 此例的索引(0,1,2,3)就是顶点数组vertices的下标，
            // 这样可以由下标代表顶点组合成矩形

            0, 1, 3, // 第一个三角形
            1, 2, 3  // 第二个三角形
    };
    // 创建元素缓冲对象
    unsigned int EBO;
    glGenBuffers(1, &EBO);
    // 绑定EBO到实际的缓冲对象上
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    // 将索引值复制到EBO对象上
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // 使用VBO管理在GPU上的顶点内存（类似于打包存储到GPU中，加快速度）
    unsigned int VBO;
    glGenBuffers(1, &VBO);
    // 将VBO绑定到实际的缓冲对象上，使得对缓冲的操作变为对VBO的操作，便于处理数据
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    // 将顶点复制到缓冲对象中
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    // 解析顶点数据
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // 设置着色器程序
    // 设置顶点着色器
    const char *vertexShaderSource = "#version 330 core\n" // 设置着色器版本
                                     "layout (location = 0) in vec3 aPos;\n" // 设置输入的格式
                                     "void main()\n"
                                     "{\n"
                                     "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n" // 设置输出（gl_Position）的值
                                     "}\0";
    // 创建顶点着色器
    unsigned int vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    // 输入源码并编译到顶点着色器
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    // 检测错误
    compileError(vertexShader);

    // 创建片段着色器
    const char* fragmentShaderSource="#version 330 core\n"
                                     "out vec4 FragColor;\n"
                                     "\n"
                                     "void main()\n"
                                     "{\n"
                                     "    FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
                                     "} ";
    unsigned int fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    compileError(fragmentShader);
    // 配置着色器程序，将上面创建的着色器链接进去
    unsigned int shaderProgram;
    shaderProgram = glCreateProgram();
    // 链接定义过的顶点着色器和片段着色器
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    // 删除无用的子着色器程序
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    // 仅绘制三角形的边
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    // 设置渲染循环，在点击退出之前一直绘制窗口
    while(!glfwWindowShouldClose(window))
    {

        // 检测是否有按键被按下(输入)
        processInput(window);
        // 检测窗口中的事件
        glfwPollEvents();
        // 设置清空屏幕使用的颜色

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        // 设置清空屏幕
        glClear(GL_COLOR_BUFFER_BIT);

        // 渲染
        // 设置要使用的着色器程序
        glUseProgram(shaderProgram);
        // 设置绑定的顶点数组对象
        glBindVertexArray(VAO);
        // 绘制三角形
//        glDrawArrays(GL_TRIANGLES, 0, 6);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
        // 绘制窗口中的每一个像素
        glfwSwapBuffers(window);
    }
    // 销毁涉及到的内存资源
    glfwTerminate();
    return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window)
{
    // ESC按键被按下会返回press否则返回release
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        // esc被按下将关闭GLFW
        glfwSetWindowShouldClose(window, true);
}

void compileError(unsigned int shader){
    int  success;
    char infoLog[512];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if(!success)
    {
        glGetShaderInfoLog(shader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
}