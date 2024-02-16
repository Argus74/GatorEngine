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
        entityManager.addEntity("Entity" + std::to_string(i));
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

        window.clear(sf::Color(0, 0, 0));
        ImGui::SFML::Render(window);
        window.display();
    }

    ImGui::SFML::Shutdown();
    return 0;
}