#include <glad/glad.h>
#include "Game.hpp"
#include <GLFW/glfw3.h>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl2.h>
#include <ctime>
#include <cstdlib>
#include <iostream>
#include <filesystem>
#if defined(__APPLE__)
#include <mach-o/dyld.h>
#elif defined(__linux__)
#include <unistd.h>
#include <limits.h>
#elif defined(_WIN32)
#include <windows.h>
#endif

// 构造函数
Game::Game()
    : winWidth(800), winHeight(600), window(nullptr),
      gameState(GameState::MainMenu), canContinue(false),
      totalEnemiesInit(10), totalEnemies(10),
      playerHP(3), spellCount(2), score(0),
      lastTime(0.0f), bulletCooldown(0.0f), xLast(false),
      playerX(400.0f), playerY(550.0f),
      player(400.0f, 550.0f, &winWidth, &winHeight),
      spell("Scripts/spell1.lua", &interpreter),
      spellBullets()
{
    scriptDir = getExecutableDir() + "/Scripts";
    scriptDirEnemy = scriptDir + "/enemy";
    // 移除自动创建目录逻辑，目录需手动创建
    enemyScripts = {
        getScriptFullPathEnemy("enemy1.lua"),
        getScriptFullPathEnemy("boss1.lua")
        // 可在此处硬编码更多敌机脚本
    };
    // 检查Lua脚本文件是否存在
    for (const auto& script : enemyScripts) {
        if (!std::filesystem::exists(script)) {
            std::cerr << "[Error] Enemy Lua script not found: " << script << std::endl;
        }
    }
    // 自动扫描Scripts目录下所有spell*.lua作为可选自机符卡脚本
    playerSpellScripts.clear();
    for (const auto& entry : std::filesystem::directory_iterator(scriptDir)) {
        if (entry.is_regular_file()) {
            std::string name = entry.path().filename().string();
            if (name.size() >= 5 && name.substr(0, 5) == "spell" && name.substr(name.size()-4) == ".lua") {
                playerSpellScripts.push_back(name);
            }
        }
    }
    if (playerSpellScripts.empty()) {
        playerSpellScripts.push_back("spell1.lua");
    }
    selectedSpellScriptIndex = 0;
    if (!std::filesystem::exists(getScriptFullPath(playerSpellScripts[selectedSpellScriptIndex]))) {
        std::cerr << "[Error] Player spell Lua script not found: " << getScriptFullPath(playerSpellScripts[selectedSpellScriptIndex]) << std::endl;
    }
}

// 析构函数
Game::~Game() {
    shutdownImGui();
    if (window) glfwDestroyWindow(window);
    glfwTerminate();
}

void Game::initWindowAndGL() {
    if (!glfwInit()) {
        std::cerr << "[Error] Failed to initialize GLFW!" << std::endl;
        std::exit(-1);
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    window = glfwCreateWindow(winWidth, winHeight, "Hanabi Simulator", NULL, NULL);
    if (!window) {
        std::cerr << "[Error] Failed to create GLFW window!" << std::endl;
        std::exit(-1);
    }
    glfwMakeContextCurrent(window);
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "[Error] Failed to initialize GLAD!" << std::endl;
        std::exit(-1);
    }
    glfwSetWindowUserPointer(window, this);
}

void Game::initImGui() {
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui::StyleColorsDark();
    std::string fontPath = "UI/res/fonts/NotoSansCJKsc-Regular.otf";
    if (!std::filesystem::exists(fontPath)) {
        std::cerr << "[Error] Font file not found: " << fontPath << std::endl;
        std::exit(-1);
    }
    io.Fonts->AddFontFromFileTTF(fontPath.c_str(), 20.0f, NULL, io.Fonts->GetGlyphRangesChineseFull());
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL2_Init();
}

void Game::shutdownImGui() {
    ImGui_ImplOpenGL2_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

void Game::resetGame() {
    totalEnemies = totalEnemiesInit;
    enemies.clear();
    enemyBullets.clear();
    spellBullets.clear();
    bullets.clear();
    player.getBullets().clear();
    playerHP = 3;
    spellCount = 2;
    score = 0;
    playerX = winWidth / 2.0f;
    playerY = winHeight - 50.0f;
    // 重新new一个Lua虚拟机，保证符卡脚本切换生效
    interpreter = DanmakuInterpreter();
    spell = SpellCard(getScriptFullPath(playerSpellScripts[selectedSpellScriptIndex]), &interpreter);
    interpreter.registerAPI(
        [this](float bx, float by, float vx, float vy, int type) {
            spellBullets.emplace_back(bx, by, vx, vy, type == 1 ? 8.0f : 5.0f, true, false);
        },
        &playerX, &playerY
    );
}

void Game::run() {
    initWindowAndGL();
    initImGui();
    lastTime = glfwGetTime();
    while (!glfwWindowShouldClose(window)) {
        float now = glfwGetTime();
        float deltaTime = now - lastTime;
        lastTime = now;
        glfwPollEvents();
        ImGui_ImplOpenGL2_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        if (gameState == GameState::MainMenu) {
            mainMenuUI();
        } else if (gameState == GameState::PlayerConfig) {
            playerConfigUI();
        } else if (gameState == GameState::Pause) {
            renderGame();
            pauseUI();
        } else if (gameState == GameState::GameHelp) {
            helpUI();
        } else {
            processInput(deltaTime);
            updateGame(deltaTime);
            renderGame();
            renderUI();
        }
        ImGui::Render();
        ImGui_ImplOpenGL2_RenderDrawData(ImGui::GetDrawData());
        glfwSwapBuffers(window);
    }
}

void Game::processInput(float deltaTime) {
    float dx = 0, dy = 0;
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS && !pauseRequested) {
        pauseRequested = true;
        if (gameState == GameState::Playing) {
            gameState = GameState::Pause;
        }
    }
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_RELEASE) {
        pauseRequested = false;
    }
    if (gameState != GameState::Playing) return;
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) dx -= 300.0f * deltaTime;
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) dx += 300.0f * deltaTime;
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) dy -= 300.0f * deltaTime;
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) dy += 300.0f * deltaTime;
    player.move(dx, dy);
    bulletCooldown -= deltaTime;
    if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS) {
        if (bulletCooldown <= 0.0f) {
            player.fireBullet();
            bulletCooldown = bulletInterval;
        }
    } else {
        bulletCooldown = 0.0f;
    }
    bool xNow = glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS;
    if (xNow && !xLast && spellCount > 0) {
        enemyBullets.clear();      // 符卡瞬间消弹
        spell.activate();          // 触发Lua符卡
        spellCount--;
    }
    xLast = xNow;
}

void Game::updateGame(float deltaTime) {
    playerX = player.getX();
    playerY = player.getY();
    player.update(deltaTime);
    spell.update(deltaTime);
    for (auto& b : spellBullets) b.update(deltaTime);
    spellBullets.erase(std::remove_if(spellBullets.begin(), spellBullets.end(),
        [this](const Bullet& b) { return b.isOutOfWindow(winWidth, winHeight); }), spellBullets.end());
    for (auto& b : enemyBullets) b.update(deltaTime);
    enemyBullets.erase(std::remove_if(enemyBullets.begin(), enemyBullets.end(),
        [this](const Bullet& b) { return b.isOutOfWindow(winWidth, winHeight); }), enemyBullets.end());
    while (enemies.size() < 3 && totalEnemies > 0) {
        int idx = std::rand() % enemyScripts.size();
        enemies.emplace_back(winWidth, winHeight, enemyScripts[idx]);
        totalEnemies--;
    }
    for (auto& enemy : enemies) {
        enemy.update(deltaTime, enemyBullets, playerX, playerY);
    }
    for (auto& enemy : enemies) {
        if (enemy.isDead()) continue;
        for (auto& b : player.getBullets()) {
            float dx = b.getX() - enemy.getX();
            float dy = b.getY() - enemy.getY();
            float dist = sqrt(dx * dx + dy * dy);
            if (dist < b.getRadius() + enemy.getRadius()) {
                enemy.hit(b.isSpellCard());
                b = Bullet(-100, -100, 0, 0);
            }
        }
    }
    for (auto& enemy : enemies) {
        if (enemy.isDead()) continue;
        for (auto& b : spellBullets) {
            float dx = b.getX() - enemy.getX();
            float dy = b.getY() - enemy.getY();
            float dist = sqrt(dx * dx + dy * dy);
            if (dist < b.getRadius() + enemy.getRadius()) {
                enemy.hit(true); // 符卡弹幕
                b = Bullet(-100, -100, 0, 0);
            }
        }
    }
    enemies.erase(std::remove_if(enemies.begin(), enemies.end(), [](const Enemy& e){ return e.isDead(); }), enemies.end());
    if (enemies.empty() && totalEnemies == 0) {
        gameState = GameState::Win;
    }
}

void Game::renderGame() {
    glClearColor(0.1f, 0.1f, 0.15f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, winWidth, winHeight, 0, -1, 1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    for (auto& b : player.getBullets()) b.draw();
    for (auto& b : spellBullets) b.draw();
    for (auto& b : enemyBullets) b.draw();
    player.draw();
    for (auto& enemy : enemies) {
        if (!enemy.isDead()) enemy.draw();
    }
}

void Game::renderUI() {
    // FPS窗口右下角浮动
    ImGui::SetNextWindowPos(ImVec2(winWidth-120, winHeight-60), ImGuiCond_Always);
    ImGui::SetNextWindowBgAlpha(0.25f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 10.0f);
    ImGui::Begin("FPS", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoSavedSettings);
    ImGui::Text("FPS: %.1f", ImGui::GetIO().Framerate);
    ImGui::End();
    ImGui::PopStyleVar();
    // 游戏信息栏右上角浮动
    int enemyCount = totalEnemies + enemies.size();
    float infoX = winWidth * 2.0f / 3.0f + 10;
    ImGui::SetNextWindowPos(ImVec2(winWidth-260, 20), ImGuiCond_Always);
    ImGui::SetNextWindowSize(ImVec2(240, 160), ImGuiCond_Always);
    ImGui::SetNextWindowBgAlpha(0.35f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 12.0f);
    ImGui::Begin("游戏信息", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoSavedSettings);
    ImGui::TextColored(ImVec4(0.9f,0.7f,0.2f,1.0f), "剩余敌人: %d", enemyCount);
    ImGui::TextColored(ImVec4(0.2f,0.8f,0.2f,1.0f), "生命值: %d", playerHP);
    ImGui::TextColored(ImVec4(0.2f,0.6f,1.0f,1.0f), "符卡: %d", spellCount);
    ImGui::TextColored(ImVec4(1.0f,0.5f,0.2f,1.0f), "分数: %d", score);
    ImGui::End();
    ImGui::PopStyleVar();
    if (gameState == GameState::Win) {
        victoryUI();
    }
}

void Game::mainMenuUI() {
    glClearColor(0.1f, 0.1f, 0.15f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, winWidth, winHeight, 0, -1, 1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    ImGui::SetNextWindowPos(ImVec2(winWidth/2-200, winHeight/2-220), ImGuiCond_Always);
    ImGui::SetNextWindowSize(ImVec2(400, 440), ImGuiCond_Always);
    ImGui::SetNextWindowBgAlpha(0.38f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 16.0f);
    ImGui::Begin("主菜单", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoSavedSettings);
    ImGui::Spacing(); ImGui::Spacing();
    ImGui::SetCursorPosX(120);
    ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[0]);
    ImGui::TextColored(ImVec4(1,0.6f,0.2f,1), "Hanabi Simulator");
    ImGui::PopFont();
    ImGui::Spacing(); ImGui::Spacing();
    ImGui::SetCursorPosX(100);
    if (ImGui::Button("开始游戏", ImVec2(200, 0))) {
        gameState = GameState::Playing;
        resetGame();
        canContinue = true;
    }
    ImGui::SetCursorPosX(100);
    if (ImGui::Button("继续游戏", ImVec2(200, 0))) {
        if (canContinue) gameState = GameState::Playing;
    }
    ImGui::SetCursorPosX(100);
    if (ImGui::Button("自机配置", ImVec2(200, 0))) {
        gameState = GameState::PlayerConfig;
    }
    ImGui::SetCursorPosX(100);
    if (ImGui::Button("游戏录像", ImVec2(200, 0))) {
        // 预留
    }
    ImGui::SetCursorPosX(100);
    if (ImGui::Button("游戏说明", ImVec2(200, 0))) {
        gameState = GameState::GameHelp;
    }
    ImGui::SetCursorPosX(100);
    if (ImGui::Button("退出游戏", ImVec2(200, 0))) {
        glfwSetWindowShouldClose(window, 1);
    }
    ImGui::End();
    ImGui::PopStyleVar();
}

void Game::playerConfigUI() {
    glClearColor(0.1f, 0.1f, 0.15f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, winWidth, winHeight, 0, -1, 1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    static bool showSaved = false;
    static bool needRefresh = true;
    // 刷新脚本列表的lambda
    auto refreshScripts = [this]() {
        playerSpellScripts.clear();
        for (const auto& entry : std::filesystem::directory_iterator(scriptDir)) {
            if (entry.is_regular_file()) {
                std::string name = entry.path().filename().string();
                if (name.size() > 4 && name.substr(name.size()-4) == ".lua") {
                    playerSpellScripts.push_back(name);
                }
            }
        }
        if (playerSpellScripts.empty()) {
            playerSpellScripts.push_back("spell1.lua");
        }
        if (selectedSpellScriptIndex >= playerSpellScripts.size()) selectedSpellScriptIndex = 0;
    };
    if (needRefresh) {
        refreshScripts();
        needRefresh = false;
    }
    ImGui::SetNextWindowPos(ImVec2(winWidth/2-220, winHeight/2-180), ImGuiCond_Always);
    ImGui::SetNextWindowSize(ImVec2(440, 360), ImGuiCond_Always);
    ImGui::SetNextWindowBgAlpha(0.40f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 18.0f);
    ImGui::Begin("自机配置", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoSavedSettings);
    ImGui::Spacing();
    ImGui::SetCursorPosX(120);
    ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[0]);
    ImGui::TextColored(ImVec4(0.2f,0.8f,1.0f,1.0f), "自机符卡脚本绑定");
    ImGui::PopFont();
    ImGui::Spacing();
    ImGui::SetCursorPosX(60);
    ImGui::TextWrapped("请选择你想要绑定的Lua符卡脚本，点击保存后生效。可在Scripts目录添加/删除脚本，点击刷新实时更新列表。");
    ImGui::Spacing();
    ImGui::SetCursorPosX(60);
    if (ImGui::Button("刷新", ImVec2(90, 0))) {
        refreshScripts();
    }
    ImGui::SameLine();
    ImGui::SetCursorPosX(170);
    ImGui::TextColored(ImVec4(0.9f,0.7f,0.2f,1.0f), "当前脚本:");
    ImGui::SameLine();
    ImGui::PushItemWidth(180);
    if (ImGui::BeginCombo("##符卡脚本", playerSpellScripts[selectedSpellScriptIndex].c_str())) {
        for (int i = 0; i < playerSpellScripts.size(); ++i) {
            bool is_selected = (selectedSpellScriptIndex == i);
            if (ImGui::Selectable(playerSpellScripts[i].c_str(), is_selected)) {
                selectedSpellScriptIndex = i;
            }
            if (is_selected) ImGui::SetItemDefaultFocus();
        }
        ImGui::EndCombo();
    }
    ImGui::PopItemWidth();
    ImGui::Spacing();
    ImGui::Spacing();
    ImGui::SetCursorPosX(120);
    if (ImGui::Button("保存", ImVec2(200, 0))) {
        // 立即应用当前选择的符卡脚本
        interpreter = DanmakuInterpreter();
        spell = SpellCard(getScriptFullPath(playerSpellScripts[selectedSpellScriptIndex]), &interpreter);
        interpreter.registerAPI(
            [this](float bx, float by, float vx, float vy, int type) {
                spellBullets.emplace_back(bx, by, vx, vy, type == 1 ? 8.0f : 5.0f, true, false);
            },
            &playerX, &playerY
        );
        showSaved = true;
    }
    if (showSaved) {
        ImGui::OpenPopup("保存成功");
    }
    if (ImGui::BeginPopupModal("保存成功", NULL, ImGuiWindowFlags_AlwaysAutoResize)) {
        ImGui::Text("符卡脚本已成功绑定！");
        if (ImGui::Button("确定", ImVec2(120, 0))) {
            showSaved = false;
            ImGui::CloseCurrentPopup();
        }
        ImGui::EndPopup();
    }
    ImGui::Spacing();
    ImGui::SetCursorPosX(120);
    if (ImGui::Button("返回主菜单", ImVec2(200, 0))) {
        gameState = GameState::MainMenu;
        needRefresh = true;
    }
    ImGui::End();
    ImGui::PopStyleVar();
}

void Game::victoryUI() {
    ImGui::OpenPopup("胜利");
    if (ImGui::BeginPopupModal("胜利", NULL, ImGuiWindowFlags_AlwaysAutoResize)) {
        ImGui::Text("恭喜你，游戏胜利！");
        if (ImGui::Button("返回主菜单")) {
            gameState = GameState::MainMenu;
            ImGui::CloseCurrentPopup();
            resetGame();
            canContinue = false;
        }
        ImGui::SameLine();
        if (ImGui::Button("重开游戏")) {
            gameState = GameState::Playing;
            ImGui::CloseCurrentPopup();
            resetGame();
            canContinue = true;
        }
        ImGui::EndPopup();
    }
}

void Game::pauseUI() {
    ImGui::SetNextWindowPos(ImVec2(winWidth/2-140, winHeight/2-100), ImGuiCond_Always);
    ImGui::SetNextWindowSize(ImVec2(280, 200), ImGuiCond_Always);
    ImGui::SetNextWindowBgAlpha(0.38f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 16.0f);
    ImGui::Begin("暂停", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoSavedSettings);
    ImGui::Spacing();
    ImGui::SetCursorPosX(90);
    ImGui::TextColored(ImVec4(1,0.6f,0.2f,1), "游戏已暂停");
    ImGui::Spacing();
    ImGui::SetCursorPosX(40);
    if (ImGui::Button("继续游戏", ImVec2(200, 0))) {
        gameState = GameState::Playing;
    }
    ImGui::SetCursorPosX(40);
    if (ImGui::Button("返回主菜单", ImVec2(200, 0))) {
        gameState = GameState::MainMenu;
        resetGame();
        canContinue = false;
    }
    ImGui::End();
    ImGui::PopStyleVar();
}

void Game::helpUI() {
    glClearColor(0.1f, 0.1f, 0.15f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, winWidth, winHeight, 0, -1, 1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    ImGui::SetNextWindowPos(ImVec2(winWidth/2-260, winHeight/2-220), ImGuiCond_Always);
    ImGui::SetNextWindowSize(ImVec2(520, 440), ImGuiCond_Always);
    ImGui::SetNextWindowBgAlpha(0.38f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 16.0f);
    ImGui::Begin("游戏说明", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoSavedSettings);
    ImGui::SetCursorPosX(120);
    ImGui::TextColored(ImVec4(0.2f,0.8f,1.0f,1.0f), "Hanabi Simulator 游戏说明");
    ImGui::Spacing();
    ImGui::TextWrapped(
        "1. 方向键/WASD控制自机移动\n"
        "2. Z键发射普通弹幕，击破敌人\n"
        "3. X键释放符卡（消弹+自定义弹幕，需有符卡次数）\n"
        "4. ESC键暂停游戏，可继续或返回主菜单\n"
        "5. 主菜单可配置自机符卡脚本，体验不同弹幕花样\n"
        "6. 游戏胜利后可重开或返回主菜单\n"
        "7. 支持Lua脚本自定义符卡弹幕，脚本位于Scripts目录\n"
        "8. 敌机弹幕脚本使用Scripts/enemy/下Lua脚本\n"
        "9. 支持Windows/Linux/MacOS，资源路径基于可执行文件目录\n"
        "10. 欢迎自定义脚本和玩法！\n\n"
        "祝你游戏愉快！"
    );
    ImGui::Spacing();
    ImGui::SetCursorPosX(160);
    if (ImGui::Button("返回主菜单", ImVec2(200, 0))) {
        gameState = GameState::MainMenu;
    }
    ImGui::End();
    ImGui::PopStyleVar();
}

std::string Game::getExecutableDir() {
    char buf[PATH_MAX];
#if defined(__APPLE__)
    uint32_t size = sizeof(buf);
    if (_NSGetExecutablePath(buf, &size) == 0) {
        return std::filesystem::path(buf).parent_path().string();
    }
#elif defined(__linux__)
    ssize_t count = readlink("/proc/self/exe", buf, PATH_MAX);
    if (count != -1) {
        return std::filesystem::path(std::string(buf, count)).parent_path().string();
    }
#elif defined(_WIN32)
    DWORD size = GetModuleFileNameA(NULL, buf, PATH_MAX);
    if (size > 0 && size < PATH_MAX) {
        return std::filesystem::path(std::string(buf, size)).parent_path().string();
    }
#endif
    return std::filesystem::current_path().string();
}

std::string Game::getScriptFullPath(const std::string& filename) const {
    return scriptDir + std::string("/") + filename;
}

std::string Game::getScriptFullPathEnemy(const std::string& filename) const {
    return scriptDirEnemy + std::string("/") + filename;
} 