#include "imgui.h"
#include "imgui-SFML.h"

#include "SFML/Graphics.hpp"
#include <box2d/box2d.h>

#include <iostream>
#include <string>
#include <vector>

#include "editor/Editor.h"
#include "EntityManager.h"
#include "GameEngine.h"
#include "Scene_Play.h"

int main()
{
    GameEngine& newGame = GameEngine::GetInstance();
    sf::Clock deltaClock;
    sf::RenderWindow& window = newGame.window();
    ImGui::SFML::Init(window);
    Editor editor;
    std::shared_ptr<Scene> scene = std::make_shared<Scene_Play>();
    newGame.ChangeScene("TestScene", scene);

    while (window.isOpen())
    {
        ImGui::SFML::Update(window, deltaClock.restart());
        editor.Draw();
        window.clear(sf::Color(0, 0, 0));
        ImGui::SFML::Render(window);
        newGame.update();
        window.display();
    }
    ImGui::SFML::Shutdown();
    return 0;
}