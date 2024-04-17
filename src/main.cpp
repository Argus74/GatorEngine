#include "imgui.h"
#include "imgui-SFML.h"

#include "SFML/Graphics.hpp"
#include <box2d/box2d.h>

#include "nfd.h"

#include <iostream>
#include <string>
#include <vector>

#include "editor/Editor.h"
#include "EntityManager.h"
#include "GameEngine.h"
#include "Scene_Play.h"
#include "lua_interpreter/LuaState.h"

int main()
{

    NFD_Init();
    
    GameEngine& newGame = GameEngine::GetInstance();
    sf::Clock deltaClock;
    sf::RenderWindow& window = newGame.window();
    ImGui::SFML::Init(window);
    Editor editor;
    std::shared_ptr<Scene> scene = std::make_shared<Scene_Play>();
    newGame.ChangeScene("TestScene", scene);
    //LuaState newLuaState("script.lua");
    
    while (window.isOpen())
    {
        newGame.update();
        ImGui::SFML::Update(window, deltaClock.restart());
        editor.Draw();
        ImGui::SFML::Render(window);
        //newLuaState.Update();
        window.display();
    }
    ImGui::SFML::Shutdown();
    NFD_Quit;
    return 0;
}
