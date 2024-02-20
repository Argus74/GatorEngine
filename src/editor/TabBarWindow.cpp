#include "TabBarWindow.h"

#include "imgui-SFML.h"

#include "../AssetManager.h"

TabBarWindow::TabBarWindow() {
    name_ = "My Tabs";
    window_flags_ |= ImGuiWindowFlags_NoTitleBar;

    // Load icon refs here for (possibly, hopefully) easier access
    auto& assetManager = AssetManager::GetInstance();
    assetManager.AddTexture("SelectIcon", "assets/SelectIcon.png");
    assetManager.AddTexture("SpriteIcon", "assets/SpriteIcon.png");
    assetManager.AddTexture("GameObjectIcon", "assets/GameObjectIcon.png");
    icons.push_back(assetManager.GetTexture("SelectIcon"));
    icons.push_back(assetManager.GetTexture("SpriteIcon"));
    icons.push_back(assetManager.GetTexture("GameObjectIcon"));
}

void TabBarWindow::SetPosition() {
    // TODO: Play around with this to see if it's the best spot
    // Position the window right below the file bar (20px tall)
    ImGui::SetNextWindowPos(ImVec2(0, 20));
    // Make it 20% of the full window height
    ImGui::SetNextWindowSize(ImVec2(ImGui::GetMainViewport()->Size.x, 
        ImGui::GetMainViewport()->Size.y * .20));
}

void TabBarWindow::DrawFrames() {
    if (ImGui::BeginTabBar("MainTabs"))
    {
        if (ImGui::BeginTabItem("Sprites"))
        {
            // TODO: Make button/text positions more dynamic, responsive
            // Select button
            ImGui::SetCursorPos(ImVec2(ImGui::GetMainViewport()->Size.x / 20, 50));
            if (ImGui::ImageButton("Select Button", icons[0], sf::Vector2f(60, 60)))
            {
                // do stuff if button clicked
            }
            ImGui::SetCursorPos(ImVec2(ImGui::GetMainViewport()->Size.x / 20 + 13, 118));
            ImGui::Text("Select");

            // Sprite button
            ImGui::SetCursorPos(ImVec2(ImGui::GetMainViewport()->Size.x / 3, 50));
            if (ImGui::ImageButton("Sprite Button", icons[1], sf::Vector2f(60, 60)))
            {
                // do stuff if button clicked
            }
            ImGui::SetCursorPos(ImVec2(ImGui::GetMainViewport()->Size.x / 3 + 14, 118));
            ImGui::Text("Sprite");

            // Game Object button
            ImGui::SetCursorPos(ImVec2(ImGui::GetMainViewport()->Size.x / 3 + 90, 50));
            if (ImGui::ImageButton("Game Object Button", icons[2], sf::Vector2f(60, 60)))
            {
                // do stuff if button clicked
            }
            ImGui::SetCursorPos(ImVec2(ImGui::GetMainViewport()->Size.x / 3 + 86, 118));
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
}
