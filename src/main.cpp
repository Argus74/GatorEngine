#include "imgui.h"
#include "imgui-SFML.h"

#include "SFML/Graphics.hpp"
#include <box2d/box2d.h>

#include <iostream>
#include <string>
#include <vector>

#include "editor/Editor.h"
#include "EntityManager.h"
#include "Game.h"


int main()
{
    Game newGame;
    newGame.update();
    sf::Clock deltaClock;
    ImGui::SFML::Init(newGame.m_window);
    Editor editor;
    while (newGame.m_window.isOpen())
    {
        sf::Event event;
        while (newGame.m_window.pollEvent(event))
		{
			ImGui::SFML::ProcessEvent(event);
			if (event.type == sf::Event::Closed)
				newGame.m_window.close();

            // Update the SFML's render window dimensions (to prevent scaling)
            if (event.type == sf::Event::Resized) {
                sf::FloatRect view(0, 0, event.size.width, event.size.height);
                newGame.m_window.setView(sf::View(view));
            }
		}

        ImGui::SFML::Update(newGame.m_window, deltaClock.restart());
        editor.Draw();
        newGame.m_window.clear(sf::Color(0, 0, 0));
        ImGui::SFML::Render(newGame.m_window);

        newGame.update();
        newGame.m_window.display();
    }
    ImGui::SFML::Shutdown();
    return 0;
}