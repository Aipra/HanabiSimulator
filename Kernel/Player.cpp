#include "Player.hpp"
#include <glad/glad.h>
#include <algorithm>

Player::Player(float x, float y, int* winWidth, int* winHeight)
    : x(x), y(y), winWidth(winWidth), winHeight(winHeight) {}

void Player::move(float dx, float dy) {
    x += dx;
    y += dy;
    // 边界判断
    if (x < 0) x = 0;
    if (x > *winWidth) x = *winWidth;
    if (y < 0) y = 0;
    if (y > *winHeight) y = *winHeight;
}

void Player::fireBullet() {
    bullets.emplace_back(x, y, 0.0f, -300.0f); // 向上
}

void Player::fireSpellCard() {
    bullets.emplace_back(x, y, 0.0f, -200.0f, 12.0f, true); // 大红弹
}

void Player::update(float deltaTime) {
    for (auto& b : bullets) b.update(deltaTime);
    bullets.erase(std::remove_if(bullets.begin(), bullets.end(),
        [this](const Bullet& b) { return b.isOutOfWindow(*winWidth, *winHeight); }), bullets.end());
}

void Player::draw() {
    // 画自机
    glColor3f(1.0f, 1.0f, 0.2f);
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(x, y);
    for (int i = 0; i <= 20; ++i) {
        float theta = 2.0f * 3.1415926f * float(i) / 20.0f;
        glVertex2f(x + cosf(theta) * 15.0f, y + sinf(theta) * 15.0f);
    }
    glEnd();
    // 画弹幕
    for (auto& b : bullets) b.draw();
}


float Player::getX() const { return x; }
float Player::getY() const { return y; }
std::vector<Bullet>& Player::getBullets() { return bullets; }