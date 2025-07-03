#include "SpellCard.hpp"

SpellCard::SpellCard(const std::string& scriptPath, DanmakuInterpreter* interpreter)
    : scriptPath(scriptPath), interpreter(interpreter) {
    interpreter->loadScript(scriptPath);
}

void SpellCard::activate() {
    interpreter->activate();
}

void SpellCard::update(float dt) {
    interpreter->update(dt);
}