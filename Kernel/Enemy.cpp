#include "Enemy.hpp"
#include <glad/glad.h>
#include <cstdlib>
#include <ctime>

Enemy::Enemy(int winWidth, int winHeight, const std::string& luaScriptPath)
    : radius(24.0f), hp(10), dead(false) {
    // 随机x在窗口内，y在屏幕上方（窗口外）
    std::srand(std::time(nullptr));
    x = radius + std::rand() % (winWidth - 2 * (int)radius);
    y = -radius - (std::rand() % 100); // 出现在屏幕上方外部
    vx = (std::rand() % 201 - 100); // -100~100
    vy = 80.0f + std::rand() % 81;  // 80~160
    interpreter = DanmakuInterpreter();
    // 注册发射弹幕API
    interpreter.registerAPI(
        [this, &enemyBullets = *(new std::vector<Bullet>)](float bx, float by, float vx, float vy, int type) mutable {
            enemyBullets.emplace_back(bx, by, vx, vy, type == 1 ? 8.0f : 5.0f, type == 1, true);
        }, &x, &y
    );
    interpreter.loadScript(luaScriptPath);
    interpreter.activate();
}

void Enemy::update(float deltaTime, std::vector<Bullet>& enemyBullets, float playerX, float playerY) {
    if (dead) return;
    // 大范围游走
    x += vx * deltaTime;
    y += vy * deltaTime;
    // 碰到左右边界反弹
    if (x < radius) { x = radius; vx = -vx; }
    if (x > 800 - radius) { x = 800 - radius; vx = -vx; } // 800为窗口宽度，可用参数传递
    // 超出下边界判定死亡
    if (y > 600 + radius) dead = true; // 600为窗口高度，可用参数传递
    interpreter.registerAPI(
        [&enemyBullets](float bx, float by, float vx, float vy, int type) {
            enemyBullets.emplace_back(bx, by, vx, vy, type == 1 ? 8.0f : 5.0f, type == 1, true);
        }, &x, &y
    );
    interpreter.update(deltaTime);
}

void Enemy::draw() {
    if (dead) return;
    glColor3f(1.0f, 0.5f, 0.5f);
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(x, y);
    for (int i = 0; i <= 24; ++i) {
        float theta = 2.0f * 3.1415926f * float(i) / 24.0f;
        glVertex2f(x + cosf(theta) * radius, y + sinf(theta) * radius);
    }
    glEnd();
}

bool Enemy::isDead() const { return dead || hp <= 0; }
void Enemy::hit(bool isSpell) {
    if (dead) return;
    hp -= isSpell ? 5 : 1;
    if (hp <= 0) dead = true;
}
float Enemy::getX() const { return x; }
float Enemy::getY() const { return y; }
float Enemy::getRadius() const { return radius; }
void Enemy::setX(float nx) { x = nx; }
void Enemy::setY(float ny) { y = ny; }