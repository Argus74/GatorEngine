#include "imgui.h"
#include "imgui-SFML.h"

#include "SFML/Graphics.hpp"
#include <box2d/box2d.h>
#include <iostream>

void DrawPropertyWindow();

int main()
{
    // Initialize
    sf::RenderWindow window(sf::VideoMode(1366, 768), "Gator Engine");
    window.setFramerateLimit(60);
    ImGui::SFML::Init(window);
    sf::Clock deltaClock;

    while (window.isOpen())
    {
        // Process user events
        sf::Event event;
        while (window.pollEvent(event))
        {
            ImGui::SFML::ProcessEvent(window, event);
            if (event.type == sf::Event::Closed)
                window.close();
        }

        // Draw GUIs
        ImGui::SFML::Update(window, deltaClock.restart());
        DrawPropertyWindow();

        // Render updated window
        window.clear();
        ImGui::SFML::Render(window);
        window.display();
    }

    // Clean up
    ImGui::SFML::Shutdown();
    return 0;
}

void DrawPropertyWindow()
{
	ImGui::Begin("Properties");
	ImGui::Text("Placeholder for properties stuff");
	ImGui::End();
}