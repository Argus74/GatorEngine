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

/*
Testing function for spawning the mock player

std::shared_ptr<Entity> spawnPlayer(Vec2 originalPos)
{
    auto entity = EntityManager::GetInstance().addEntity("Player");

    entity->cTransform = std::make_shared<CTransform>(originalPos, Vec2(80, 70), 0);
    entity->cName = std::make_shared<CName>("Player");
    entity->cShape = std::make_shared<CShape>("Rectangle", sf::Color::Red);
    entity->cTransform->velocity = Vec2(5, 5);

    // TODO: initialize player input component
    return entity;
}
*/

int main()
{

    /*
        Testing code creating mock player:

        Vec2 originalPos = Vec2(400, 000);
        auto entity = spawnPlayer(originalPos);

        sf::RectangleShape rectangle(sf::Vector2f(entity->cTransform->scale.x, entity->cTransform->scale.y));
        rectangle.setPosition(entity->cTransform->position.x, entity->cTransform->position.y);

        rectangle.setFillColor(entity->cShape->color);
        Game newGame(entity);
    */

    Game newGame; // There's probably a better way, but could make Game a singleton OR have a ref to this game passed into editor to manipulate current frame
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
            /*

            Movement testing with mock user input:

            if (event.type == sf::Event::KeyPressed)
            {
                // Check which key was pressed
                if (event.key.code == sf::Keyboard::W)
                {
                    entity->cTransform->position.y -= entity->cTransform->velocity.y;
                }
                // Add more conditions for other keys if needed
                if (event.key.code == sf::Keyboard::A)
                {
                    entity->cTransform->position.x -= entity->cTransform->velocity.x;
                }
                if (event.key.code == sf::Keyboard::S)
                {
                    entity->cTransform->position.y += entity->cTransform->velocity.y;
                }
                if (event.key.code == sf::Keyboard::D)
                {
                    entity->cTransform->position.x += entity->cTransform->velocity.x;
                }
            }
            */
        }

        ImGui::SFML::Update(newGame.m_window, deltaClock.restart());
        editor.Draw();
        newGame.m_window.clear(sf::Color(0, 0, 0));
        ImGui::SFML::Render(newGame.m_window);

        newGame.update();

        newGame.m_window.display();
        /*
            Set position and draw mock player:

            rectangle.setPosition(entity->cTransform->position.x, entity->cTransform->position.y);
            newGame.m_window.draw(rectangle);
        */

        newGame.m_window.display(); // add this to sRender()?
    }
    ImGui::SFML::Shutdown();
    return 0;
}