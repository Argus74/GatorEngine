#include "imgui.h"
#include "imgui-SFML.h"

#include "SFML/Graphics.hpp"
#include <box2d/box2d.h>
#include <iostream>

// #include <EntityManager.h>
// #include "glm.hpp"

#include "editor/Editor.h"

int main()
{
    sf::RenderWindow window(sf::VideoMode(1366, 768), "Gator Engine");
    // glm::bvec4 testVec(1, 0, 3, 4);
    ImGui::SFML::Init(window);
    sf::Clock deltaClock;

	// Update EntityManager (and all managed entities)
	// entityManager.update();

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
        // Update the ImGui visuals
        ImGui::SFML::Update(window, deltaClock.restart());
        ImGui::Begin("Red Square Size");
        ImGui::Text("Drag the slider below to control red box size and position");
        ImGui::SliderFloat("Length", &redBoxLength, 10, 300);
        ImGui::SliderFloat("Height", &redBoxHeight, 10, 300);

        // Draw ImGui windows of the Editor
        editor.Draw();

        // Update physics objects based on gui's
        dynamicBox.SetAsBox(redBoxLength / 2, redBoxHeight / 2); // Updating size
        // The position should only be updated if there is a change in the XPos/YPos slider
        if (ImGui::SliderFloat("XPos", &redBoxX, 0, 1366))
        {
            body->SetTransform(b2Vec2(redBoxX, body->GetPosition().y), body->GetAngle()); // Updating x position
            body->SetAwake(true);
        }

        if (ImGui::SliderFloat("YPos", &redBoxY, 0, 768))
        {
            body->SetTransform(b2Vec2(body->GetPosition().x, redBoxY), body->GetAngle()); // Updating y position
            body->SetAwake(true);
        }

		// Interact with your entities through ImGui 
		// ...

        ImGui::End();

		// Draw entities
		// Example for an entity that has a method to return their shape for SFML:
		// for (auto& entity : entityManager.getEntities()) {
		//     if (auto shapeComponent = entity->getShape()) {
		//         window.draw(*shapeComponent->getSFMLShape());
		//     }
		// }
        
        // Run a physics step at 1/60 of a second
        world.Step(timeStep, velocityIterations, positionIterations);
        // Update sprite positions and rotations and sizes based on physics objects
        boxShape.setPosition(body->GetPosition().x, window.getSize().y - body->GetPosition().y);
        boxShape.setRotation(body->GetAngle());
        boxShape.setSize(sf::Vector2f(redBoxLength, redBoxHeight));
        boxShape.setOrigin(redBoxLength / 2, redBoxHeight / 2);

        window.clear();
        window.draw(groundShape);
        window.draw(boxShape);
        ImGui::SFML::Render(window);
        window.display();
    }

    ImGui::SFML::Shutdown();
    return 0;
}