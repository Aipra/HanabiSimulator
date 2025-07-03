#include <iostream>
#include <vector>
#include <memory>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "Kernel/Player.hpp"
#include "Kernel/Bullet.hpp"
#include "Kernel/SpellCard.hpp"
#include "Kernel/DanmakuInterpreter.hpp"

int winWidth = 800, winHeight = 600;

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    ::winWidth = width;
    ::winHeight = height;
    glViewport(0, 0, width, height);
}

int main() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    GLFWwindow* window = glfwCreateWindow(winWidth, winHeight, "Player Demo", NULL, NULL);
    glfwMakeContextCurrent(window);
    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    Player player(winWidth / 2.0f, winHeight - 50.0f, &winWidth, &winHeight);

    float lastTime = glfwGetTime();
    float bulletCooldown = 0.0f; // Z键连发冷却
    const float bulletInterval = 0.08f; // 连发间隔（秒）
    static bool xLast = false;

    float playerX = winWidth / 2.0f, playerY = winHeight - 50.0f;

    std::vector<Bullet> bullets;
    // 弹幕发射API
    auto emitFunc = [&](float x, float y, float vx, float vy, int type) {
        bullets.emplace_back(x, y, vx, vy, type == 1 ? 8.0f : 5.0f, type == 1);
    };
    DanmakuInterpreter interpreter;
    interpreter.registerAPI(emitFunc, &playerX, &playerY);
    SpellCard spell("/Users/luyao/Documents/HanabiSimulator/Scripts/spell3.lua", &interpreter);


    while (!glfwWindowShouldClose(window)) {
        float now = glfwGetTime();
        float deltaTime = now - lastTime;
        lastTime = now;

        glfwPollEvents();

        // 移动
        float speed = 300.0f;
        float dx = 0, dy = 0;
        if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) dx -= speed * deltaTime;
        if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) dx += speed * deltaTime;
        if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) dy -= speed * deltaTime;
        if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) dy += speed * deltaTime;
        player.move(dx, dy);

        // 同步playerX/playerY
        playerX = player.getX();
        playerY = player.getY();

        // Z键连发
        bulletCooldown -= deltaTime;
        if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS) {
            if (bulletCooldown <= 0.0f) {
                player.fireBullet();
                bulletCooldown = bulletInterval;
            }
        } else {
            bulletCooldown = 0.0f;
        }

        // X键单发
        // static bool xLast = false;
        bool xNow = glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS;
        if (xNow && !xLast) spell.activate();
        xLast = xNow;

        spell.update(deltaTime);
        player.update(deltaTime);

        // 每帧更新弹幕
        for (auto& b : bullets) b.update(deltaTime);
        bullets.erase(std::remove_if(bullets.begin(), bullets.end(),
            [](const Bullet& b) { return b.isOutOfWindow(winWidth, winHeight); }), bullets.end());

         // 渲染
        glClearColor(0.1f, 0.1f, 0.15f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glOrtho(0, winWidth, winHeight, 0, -1, 1); // 像素坐标
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();

        // 绘制弹幕
        for (auto& b : bullets) b.draw();

        player.draw();

        glfwSwapBuffers(window);
    }
    glfwTerminate();
    return 0;
}