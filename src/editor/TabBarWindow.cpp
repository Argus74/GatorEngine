#include "TabBarWindow.h"
#include "Editor.h"

#include "imgui-SFML.h"

#include "../AssetManager.h"

TabBarWindow::TabBarWindow()
{
    name_ = "My Tabs";
    window_flags_ |= ImGuiWindowFlags_NoTitleBar;

    // Load icon refs here for (possibly, hopefully) easier access
    auto &assetManager = AssetManager::GetInstance();
    assetManager.AddTexture("SelectIcon", "assets/SelectIcon.png");
    assetManager.AddTexture("SpriteIcon", "assets/SpriteIcon.png");
    assetManager.AddTexture("GameObjectIcon", "assets/GameObjectIcon.png");
    icons.push_back(assetManager.GetTexture("SelectIcon"));
    icons.push_back(assetManager.GetTexture("SpriteIcon"));
    icons.push_back(assetManager.GetTexture("GameObjectIcon"));
}

void TabBarWindow::SetPosition()
{
    // TODO: Play around with this to see if it's the best spot
    // Position the window right below the file bar (20px tall)
    ImGui::SetNextWindowPos(ImVec2(0, 20));
    // Make it 20% of the full window height
    ImGui::SetNextWindowSize(ImVec2(ImGui::GetMainViewport()->Size.x,
                                    ImGui::GetMainViewport()->Size.y * 0.20));
}

void TabBarWindow::DrawFrames()
{
    if (ImGui::BeginTabBar("MainTabs"))
    {
        float imageSize = ImGui::GetMainViewport()->Size.y * 0.075;
        float imageY = (50 + (ImGui::GetMainViewport()->Size.y * 0.185 - 30)) / 2 - imageSize / 2;
        if (Editor::state != Editor::State::Testing)
        {
            if (ImGui::BeginTabItem("Sprites"))
            {
                // TODO: Make button/text positions more dynamic, responsive
                // Select button
                ImGui::SetCursorPos(ImVec2(ImGui::GetMainViewport()->Size.x / 20, imageY));
                if (ImGui::ImageButton("Select Button", icons[0], sf::Vector2f(imageSize, imageSize)))
                {
                    // do stuff if button clicked
                }
                ImGui::SetCursorPos(ImVec2(ImGui::GetMainViewport()->Size.x / 20 + imageSize / 2 - 17, imageY + imageSize + 10));
                ImGui::Text("Select");

                // Sprite button
                ImGui::SetCursorPos(ImVec2(ImGui::GetMainViewport()->Size.x / 3, imageY));
                if (ImGui::ImageButton("Sprite Button", icons[1], sf::Vector2f(imageSize, imageSize)))
                {
                    // do stuff if button clicked
                }
                ImGui::SetCursorPos(ImVec2(ImGui::GetMainViewport()->Size.x / 3 + imageSize / 2 - 16, imageY + imageSize + 10));
                ImGui::Text("Sprite");

                // Game Object button
                ImGui::SetCursorPos(ImVec2(ImGui::GetMainViewport()->Size.x / 3 + imageSize + 40, imageY));
                if (ImGui::ImageButton("Game Object Button", icons[2], sf::Vector2f(imageSize, imageSize)))
                {
                    // do stuff if button clicked
                }
                ImGui::SetCursorPos(ImVec2(ImGui::GetMainViewport()->Size.x / 3 + imageSize + 40 + imageSize / 2 - 34, imageY + imageSize + 10));
                ImGui::Text("Game Object");
                ImGui::EndTabItem();
            }
        }

        if (ImGui::BeginTabItem("Testing"))
        {
            // Sprite button
            ImGui::SetCursorPos(ImVec2(ImGui::GetMainViewport()->Size.x / 3, imageY));
            if (ImGui::ImageButton("Start Button", icons[0], sf::Vector2f(imageSize, imageSize)))
            {
                Editor::state = Editor::State::Testing; // Button clicked: now testing
                // TODO: start game: init?
            }
            ImGui::SetCursorPos(ImVec2(ImGui::GetMainViewport()->Size.x / 3 + imageSize / 2 - 13, imageY + imageSize + 10));
            ImGui::Text("Start");

            // Game Object button
            ImGui::SetCursorPos(ImVec2(ImGui::GetMainViewport()->Size.x / 3 + imageSize + 40, imageY));
            if (ImGui::ImageButton("Stop Button", icons[1], sf::Vector2f(imageSize, imageSize)))
            {
                Editor::state = Editor::State::None; // Reset state to none
                // TODO: end game: unload content?
            }
            ImGui::SetCursorPos(ImVec2(ImGui::GetMainViewport()->Size.x / 3 + imageSize + 40 + imageSize / 2 - 9, imageY + imageSize + 10));
            ImGui::Text("Stop");
            ImGui::EndTabItem();
        }

        ImGui::EndTabBar();
    }
}