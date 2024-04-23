#include <iostream>
#include <string>
#include <vector>

#include "SFML/Graphics.hpp"
#include "box2d/box2d.h"
#include "imgui-SFML.h"
#include "imgui.h"
#include "nfd.h"

#include "EntityManager.h"
#include "GameEngine.h"
#include "editor/Editor.h"
#include "lua_interpreter/LuaState.h"
#include "util/Scene.h"
#include "util/Serializable.h"

int main() {
    GameEngine& newGame = GameEngine::GetInstance();
    sf::Clock deltaClock;
    sf::RenderWindow& window = newGame.window();
    ImGui::SFML::Init(window);
    Editor editor;
    newGame.changeScene(newGame.currentScenePath());
    //LuaState newLuaState("script.lua");

    while (window.isOpen()) {
        newGame.update();
        ImGui::SFML::Update(window, deltaClock.restart());
        editor.Draw();
        ImGui::SFML::Render(window);
        //newLuaState.Update();
        window.display();
    }
    ImGui::SFML::Shutdown();

    return 0;
}
