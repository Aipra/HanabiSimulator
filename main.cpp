#include <iostream>
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include <cmath>

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
    
    // 获取主显示器
    GLFWmonitor* monitor = glfwGetPrimaryMonitor();
    const GLFWvidmode* mode = glfwGetVideoMode(monitor);
    
    // 计算窗口居中位置
    int windowX = (mode->width - 800) / 2;
    int windowY = (mode->height - 600) / 2;
    
    // 设置窗口位置
    glfwSetWindowPos(window, windowX, windowY);
    
    // 设置当前线程的上下文
    glfwMakeContextCurrent(window);
    // 开启垂直同步，保证60Hz
    glfwSwapInterval(1);
    //调用glfw函数之前，初始化glad
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }
    // 获取窗口的实际大小
    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    
    // 设置opengl渲染视口的大小
    glViewport(0, 0, width, height);
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
    const char *vertexShaderSource = "#version 330 core\n"
                                     "layout (location = 0) in vec3 aPos;\n"
                                     "uniform mat4 rotation;\n"
                                     "void main()\n"
                                     "{\n"
                                     "   gl_Position = rotation * vec4(aPos, 1.0);\n"
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

    // 获取旋转矩阵uniform的位置
    int rotationLoc = glGetUniformLocation(shaderProgram, "rotation");

    // 删除无用的子着色器程序
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    // 仅绘制三角形的边
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    // 旋转角度变量
    float angle = 0.0f;
    // 旋转速度（弧度/秒）
    float rotateSpeed = 2.0f; // 可调整转速
    float lastTime = glfwGetTime();

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

        // 计算帧间隔
        float currentTime = glfwGetTime();
        float deltaTime = currentTime - lastTime;
        lastTime = currentTime;

        // 检查键盘输入，控制旋转角度
        if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
            angle += rotateSpeed * deltaTime;
        }
        if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
            angle -= rotateSpeed * deltaTime;
        }

        // 构造旋转矩阵
        float rotation[16] = {
            cos(angle), -sin(angle), 0.0f, 0.0f,
            sin(angle),  cos(angle), 0.0f, 0.0f,
            0.0f,        0.0f,      1.0f, 0.0f,
            0.0f,        0.0f,      0.0f, 1.0f
        };
        glUseProgram(shaderProgram);
        glUniformMatrix4fv(rotationLoc, 1, GL_FALSE, rotation);
        // 设置绑定的顶点数组对象
        glBindVertexArray(VAO);
        // 绘制三角形
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