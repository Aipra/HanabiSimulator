#pragma once
class Bullet {
public:
    Bullet(float x, float y, float vx, float vy, float radius = 5.0f, bool isSpell = false);
    void update(float deltaTime);
    void draw();
    bool isOutOfWindow(int winWidth, int winHeight) const;
    float getX() const;
    float getY() const;
    float getRadius() const;
    bool isSpellCard() const;
private:
    float x, y, vx, vy, radius;
    bool spell;
};