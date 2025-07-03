#include "Bullet.hpp"
#include <glad/glad.h>
#include <cmath>

Bullet::Bullet(float x, float y, float vx, float vy, float radius, bool isSpell)
    : x(x), y(y), vx(vx), vy(vy), radius(radius), spell(isSpell) {}

void Bullet::update(float deltaTime) {
    x += vx * deltaTime;
    y += vy * deltaTime;
}

void Bullet::draw() {
    // 画圆
    glBegin(GL_TRIANGLE_FAN);
    if (spell) {
        glColor3f(1.0f, 0.2f, 0.2f); // spell弹幕为红色
    } else {
        glColor3f(0.2f, 0.8f, 1.0f); // 普通弹幕为蓝色
    }
    glVertex2f(x, y);
    for (int i = 0; i <= 20; ++i) {
        float theta = 2.0f * 3.1415926f * float(i) / 20.0f;
        glVertex2f(x + cosf(theta) * radius, y + sinf(theta) * radius);
    }
    glEnd();
}

bool Bullet::isOutOfWindow(int winWidth, int winHeight) const {
    return x < -radius || x > winWidth + radius || y < -radius || y > winHeight + radius;
}

float Bullet::getX() const { return x; }
float Bullet::getY() const { return y; }
float Bullet::getRadius() const { return radius; }
bool Bullet::isSpellCard() const { return spell; }