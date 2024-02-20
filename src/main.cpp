#include "imgui.h"
#include "imgui-SFML.h"

#include "SFML/Graphics.hpp"
#include <box2d/box2d.h>

#include <iostream>
#include <string>
#include <vector>

#include "editor/Editor.h"
#include "EntityManager.h"


int main()
{
    sf::RenderWindow window(sf::VideoMode(1366, 768), "Gator Engine");
    ImGui::SFML::Init(window);
    sf::Clock deltaClock;
    // Create world
    b2Vec2 gravity(0.0f, -10.0f);
    b2World world(gravity);
    // Create a body def for the ground
    b2BodyDef groundBodyDef;
    groundBodyDef.position.Set(500, 500);
    // Create a body using the def for the ground
    b2Body *groundBody = world.CreateBody(&groundBodyDef);
    // Create a polygon shape for the ground
    b2PolygonShape groundBox;
    groundBox.SetAsBox(50.0f, 50.0f);
    // Give the ground physical properties
    groundBody->CreateFixture(&groundBox, 0.0f);
    // Create the sprite for the ground
    sf::RectangleShape groundShape(sf::Vector2f(100.0f, 100.0f));
    groundShape.setPosition(500, window.getSize().y - groundBody->GetPosition().y);
    groundShape.setOrigin(sf::Vector2f(50, 50));
    groundShape.setFillColor(sf::Color::Blue);

    // Create a body def for the rigid body
    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;
    bodyDef.position.Set(500, 900);
    // Create a body using the def for the rigid body
    b2Body *body = world.CreateBody(&bodyDef);
    // Create a polygon shape for the rigid body
    b2PolygonShape dynamicBox;
    dynamicBox.SetAsBox(5, 5);
    // Give the rigid body physical properties
    b2FixtureDef fixtureDef;
    fixtureDef.shape = &dynamicBox;
    fixtureDef.density = 1.0f;
    fixtureDef.friction = 0.3f;
    body->CreateFixture(&fixtureDef);

    // Create the sprite for the rigid body
    sf::RectangleShape boxShape(sf::Vector2f(10, 10));
    boxShape.setPosition(500, window.getSize().y - body->GetPosition().y);
    boxShape.setRotation(body->GetAngle());
    boxShape.setOrigin(sf::Vector2f(5, 5));
    boxShape.setFillColor(sf::Color::Red);

    // Data for physics simulation steps
    float timeStep = 1.0f / 60.0f;
    int32 velocityIterations = 6;
    int32 positionIterations = 2;

    window.setFramerateLimit(60);
    // Attach
    float redBoxLength = 10;
    float redBoxHeight = 10;
    float redBoxX = 500;
    float redBoxY = 768;

    // Initialize Editor
    Editor editor;

    // TESTING: Add a few entities to the EntityManager
    auto &entityManager = EntityManager::GetInstance();
    for (int i = 0; i < 20; i++)
    {
        const auto &entity = entityManager.addEntity("Entity" + std::to_string(i));
        entity->cTransform = std::make_shared<CTransform>(sf::Vector2f(110 * i, 120 * i), sf::Vector2f(8 * i, 7 * i), 2 * i);
        entity->cBBox = std::make_shared<CBBox>(sf::Vector2f(11 * i, 3 * i));
        entity->cName = std::make_shared<CName>("MyEntity " + std::to_string(i));
        entity->cShape = std::make_shared<CShape>("Rectangle", sf::Color(i * 11, i * 11 / 3, i * 11 / 4, 255));
    }

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
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