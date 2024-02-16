#include "imgui.h"
#include "imgui-SFML.h"

#include "SFML/Graphics.hpp"
#include <box2d/box2d.h>

#include <string>
#include <vector>

#include "editor/Editor.h"
#include "EntityManager.h"

int main()
{
    sf::RenderWindow window(sf::VideoMode(1366, 768), "Gator Engine");
    ImGui::SFML::Init(window);
    sf::Clock deltaClock;

    // Initialize Editor
    Editor editor;

    // TESTING: Add a few entities to the EntityManager
    auto& entityManager = EntityManager::GetInstance();
    for (int i = 0; i < 20; i++) {
        auto& entity = entityManager.addEntity("Entity" + std::to_string(i));
        entity->cTransform = std::make_shared<CTransform>(sf::Vector2f(110 * i, 120 * i), sf::Vector2f(8*i, 7*i), 2*i);
        entity->cBBox = std::make_shared<CBBox>(sf::Vector2f(11 * i, 3 * i));
        entity->cName = std::make_shared<CName>("MyEntity " + std::to_string(i));
        entity->cShape = std::make_shared<CShape>("Rectangle", sf::Color(i*11, i*11/3, i*11/4, 255));
    }

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            // Process ImGui gui events
            ImGui::SFML::ProcessEvent(event);
            if (event.type == sf::Event::Closed)
                window.close();
        }

        entityManager.update();

        ImGui::SFML::Update(window, deltaClock.restart());
        editor.Draw();
        ImGui::ShowDemoWindow();

        window.clear(sf::Color(0, 0, 0));
        ImGui::SFML::Render(window);
        window.display();
    }

    ImGui::SFML::Shutdown();
    return 0;
}