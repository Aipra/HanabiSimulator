#pragma once
#include <sol/sol.hpp>
#include <string>
#include <functional>

class DanmakuInterpreter {
public:
    DanmakuInterpreter();
    void registerAPI(const std::function<void(float, float, float, float, int)>& emitFunc, float* playerX, float* playerY);
    void loadScript(const std::string& path);
    void activate();
    void update(float dt);
private:
    sol::state lua;
};