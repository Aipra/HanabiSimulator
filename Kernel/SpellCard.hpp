#pragma once
#include <string>
#include "DanmakuInterpreter.hpp"

class SpellCard {
public:
    SpellCard(const std::string& scriptPath, DanmakuInterpreter* interpreter);
    void activate();
    void update(float dt);
private:
    std::string scriptPath;
    DanmakuInterpreter* interpreter;
};