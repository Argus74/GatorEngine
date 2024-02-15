#include "imgui.h"
#include "imgui-SFML.h"

#include "SFML/Graphics.hpp"
#include <box2d/box2d.h>

#include <string>
#include <vector>

#include "editor/Editor.h"

int main()
{
    sf::RenderWindow window(sf::VideoMode(1366, 768), "Gator Engine");
    ImGui::SFML::Init(window);
    sf::Clock deltaClock;

    // Initialize Editor
    Editor editor;

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            ImGui::SFML::ProcessEvent(event);
            if (event.type == sf::Event::Closed)
                window.close();
        }

        ImGui::SFML::Update(window, deltaClock.restart());
        editor.Draw();

        window.clear(sf::Color(0, 0, 0));
        ImGui::SFML::Render(window);
        window.display();
    }

    ImGui::SFML::Shutdown();
    return 0;
}