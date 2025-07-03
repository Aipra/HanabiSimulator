#pragma once
#include <vector>
#include <memory>
#include "Bullet.hpp"

class Player {
public:
    Player(float x, float y, int* winWidth, int* winHeight);
    void move(float dx, float dy);
    void fireBullet();
    void fireSpellCard();
    void update(float deltaTime);
    void draw();
    float getX() const;
    float getY() const;
    std::vector<Bullet>& getBullets();
private:
    float x, y;
    int* winWidth;
    int* winHeight;
    std::vector<Bullet> bullets;
};