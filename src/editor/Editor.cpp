#include "Editor.h"

#include "FileBarWindow.h"
#include "TabBarWindow.h"
#include "ExplorerWindow.h"
#include "PropertyWindow.h"
#include "SceneLayoutWindow.h"

// Static variables
Editor::State Editor::state;
std::shared_ptr<Entity> Editor::active_entity_;




Editor::Editor() {
    // Setup default, global style vars for consistent look
    ImGuiStyle& style = ImGui::GetStyle();
    style.WindowBorderSize = 0.0f;
    style.FrameBorderSize = 1.0f;
    style.ScrollbarRounding = 2.0f;
    style.GrabRounding = 2.0f;
    ImGui::StyleColorsLight();
    style.Colors[ImGuiCol_WindowBg] = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
    style.Colors[ImGuiCol_TitleBg] = style.Colors[ImGuiCol_TitleBgActive]; // Make title bar always same color
    style.Colors[ImGuiCol_Header] = ImVec4(0.25f, 0.58f, 0.98f, 0.45f);
    style.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.25f, 0.58f, 0.98f, 0.53f);
    style.Colors[ImGuiCol_HeaderActive] = ImVec4(0.25f, 0.58f, 0.98f, 0.60f);
    style.Colors[ImGuiCol_Button] = style.Colors[ImGuiCol_Header];
    style.Colors[ImGuiCol_ButtonHovered] = style.Colors[ImGuiCol_HeaderHovered];
    style.Colors[ImGuiCol_ButtonActive] = style.Colors[ImGuiCol_HeaderActive];
    style.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.25f, 0.58f, 0.98f, 0.30f);
    style.Colors[ImGuiCol_HeaderActive] = ImVec4(0.25f, 0.58f, 0.98f, 0.50f);

    state = State::None;
    active_entity_ = nullptr;

    windows_.push_back(std::make_unique<FileBarWindow>());
    windows_.push_back(std::make_unique<TabBarWindow>());
    windows_.push_back(std::make_unique<PropertyWindow>());
    windows_.push_back(std::make_unique<ExplorerWindow>());
    windows_.push_back(std::make_unique<SceneLayoutWindow>());
}

void Editor::Draw() {
    for (auto& window : windows_) {
        window->Draw();
    }
}