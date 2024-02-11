#include "imgui.h"
#include "imgui-SFML.h"

#include "SFML/Graphics.hpp"
#include <box2d/box2d.h>

#include <string>
#include <vector>

void DrawFileBar()
{
    if (ImGui::BeginMainMenuBar())
    {
        if (ImGui::BeginMenu("File"))
        {
            ImGui::MenuItem("Open");
            ImGui::MenuItem("Save");
            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("..."))
        {
            ImGui::MenuItem("Stuff Here");
            ImGui::EndMenu();
        }
        ImGui::EndMainMenuBar();
    }
}

void DrawTabBar(sf::RenderWindow &window, std::vector<sf::Texture> &textures)
{
    ImGui::SetNextWindowSize(ImVec2(window.getSize().x, 150));
    ImGui::SetNextWindowPos(ImVec2(0, 20));
    ImGui::Begin("My Tabs", NULL, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize);
    if (ImGui::BeginTabBar("MainTabs"))
    {
        if (ImGui::BeginTabItem("Sprites"))
        {
            // Select button
            ImGui::SetCursorPos(ImVec2(window.getSize().x / 20, 50));
            if (ImGui::ImageButton("Select Button", textures[0], sf::Vector2f(60, 60)))
            {
                // do stuff if button clicked
            }
            ImGui::SetCursorPos(ImVec2(window.getSize().x / 20 + 13, 118));
            ImGui::Text("Select");

            // Sprite button
            ImGui::SetCursorPos(ImVec2(window.getSize().x / 3, 50));
            if (ImGui::ImageButton("Sprite Button", textures[1], sf::Vector2f(60, 60)))
            {
                // do stuff if button clicked
            }
            ImGui::SetCursorPos(ImVec2(window.getSize().x / 3 + 13, 118));
            ImGui::Text("Sprite");

            // Game Object button
            ImGui::SetCursorPos(ImVec2(window.getSize().x / 3 + 90, 50));
            if (ImGui::ImageButton("Game Object Button", textures[2], sf::Vector2f(60, 60)))
            {
                // do stuff if button clicked
            }
            ImGui::SetCursorPos(ImVec2(window.getSize().x / 3 + 86, 118));
            ImGui::Text("Game Object");
            ImGui::EndTabItem();
        }

        if (ImGui::BeginTabItem("Testing"))
        {
            // additional UI elements specific to testing
            ImGui::EndTabItem();
        }

        ImGui::EndTabBar();
    }
    ImGui::End();
}

int main()
{
    sf::RenderWindow window(sf::VideoMode(1366, 768), "Gator Engine");
    ImGui::SFML::Init(window);
    sf::Clock deltaClock;

    sf::Texture selectIcon;
    selectIcon.loadFromFile("../assets/SelectIcon.png");

    sf::Texture spriteIcon;
    spriteIcon.loadFromFile("../assets/SpriteIcon.png");

    sf::Texture gameObjectIcon;
    gameObjectIcon.loadFromFile("../assets/GameObjectIcon.png");

    std::vector<sf::Texture> icons = {selectIcon, spriteIcon, gameObjectIcon};

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

        DrawFileBar();
        DrawTabBar(window, icons);

        window.clear(sf::Color(0, 0, 0));
        ImGui::SFML::Render(window);
        window.display();
    }

    ImGui::SFML::Shutdown();
    return 0;
}