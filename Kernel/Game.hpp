#pragma once
#include <vector>
#include <string>
#include <memory>
#include <GLFW/glfw3.h>
#include "Player.hpp"
#include "Bullet.hpp"
#include "SpellCard.hpp"
#include "DanmakuInterpreter.hpp"
#include "Enemy.hpp"
#include <filesystem>
#if defined(_WIN32)
#include <windows.h>
#endif

enum class GameState { MainMenu, Playing, Win, PlayerConfig, Pause, GameHelp };

class Game {
public:
    Game();
    ~Game();
    void run();
    int winWidth, winHeight;
private:
    static std::string getExecutableDir();
    std::string scriptDir;
    std::string scriptDirEnemy;
    std::string getScriptFullPath(const std::string& filename) const;
    std::string getScriptFullPathEnemy(const std::string& filename) const;
    void initWindowAndGL();
    void initImGui();
    void shutdownImGui();
    void resetGame();
    void processInput(float deltaTime);
    void updateGame(float deltaTime);
    void renderGame();
    void renderUI();
    void mainMenuUI();
    void victoryUI();
    void playerConfigUI();
    void pauseUI();
    void helpUI();
    GLFWwindow* window;
    GameState gameState;
    bool canContinue;
    int totalEnemiesInit;
    int totalEnemies;
    int playerHP;
    int spellCount;
    int score;
    float lastTime;
    float bulletCooldown;
    const float bulletInterval = 0.08f;
    bool xLast;
    bool pauseRequested = false;
    float playerX, playerY;
    Player player;
    DanmakuInterpreter interpreter;
    SpellCard spell;
    std::vector<Bullet> bullets;
    std::vector<Bullet> enemyBullets;
    std::vector<Bullet> spellBullets;
    std::vector<Enemy> enemies;
    std::vector<std::string> enemyScripts;
    std::vector<std::string> playerSpellScripts;
    int selectedSpellScriptIndex = 0;
}; 