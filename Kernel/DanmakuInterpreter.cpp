#include "DanmakuInterpreter.hpp"

DanmakuInterpreter::DanmakuInterpreter() {
    lua.open_libraries(sol::lib::base, sol::lib::math);
}

void DanmakuInterpreter::registerAPI(const std::function<void(float, float, float, float, int)>& emitFunc, float* playerX, float* playerY) {
    lua.set_function("emit_bullet", emitFunc);
    lua.set_function("get_player_pos", [playerX, playerY]() {
        return std::make_tuple(*playerX, *playerY);
    });
}

void DanmakuInterpreter::loadScript(const std::string& path) {
    lua.script_file(path);
}

void DanmakuInterpreter::activate() {
    if (lua["activate"].valid())
        lua["activate"]();
    else
        std::cout << "Lua activate() not found!" << std::endl;
}

void DanmakuInterpreter::update(float dt) {
    if (lua["update"].valid())
        lua["update"](dt);
}