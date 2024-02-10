#include "imgui.h"
#include "imgui-SFML.h"
#include "SFML/Graphics.hpp"
#include "box2d/box2d.h"

int main() {
    // Initialize SFML window
    sf::RenderWindow window(sf::VideoMode(1366, 768), "Gator Engine");
    window.setFramerateLimit(60);

    // Initialize ImGui
    ImGui::SFML::Init(window);
    sf::Clock deltaClock;

    while (window.isOpen()) {
        // Process user events
        sf::Event event;
        while (window.pollEvent(event)) {
            ImGui::SFML::ProcessEvent(window, event);
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }

        // Draw GUIs
        ImGui::SFML::Update(window, deltaClock.restart());
        
        // Render updated window
        window.clear();
        ImGui::SFML::Render(window);
        window.display();
    }

    // Clean up
    ImGui::SFML::Shutdown();
    return 0;
}