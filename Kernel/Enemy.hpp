#pragma once
#include <vector>
#include <memory>
#include "Bullet.hpp"
#include "DanmakuInterpreter.hpp"

class Enemy {
public:
    Enemy(int winWidth, int winHeight, const std::string& luaScriptPath);
    void update(float deltaTime, std::vector<Bullet>& enemyBullets, float playerX, float playerY);
    void draw();
    bool isDead() const;
    void hit(bool isSpell); // 被击中时调用
    float getX() const;
    float getY() const;
    float getRadius() const;
    void setX(float nx);
    void setY(float ny);
private:
    float x, y;
    float vx, vy;
    float radius;
    int hp;
    DanmakuInterpreter interpreter;
    bool dead;
};