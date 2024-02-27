#include "Editor.h"

#include "FileBarWindow.h"
#include "TabBarWindow.h"
#include "ExplorerWindow.h"
#include "PropertyWindow.h"
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

    active_entity_ = nullptr;

    windows_.push_back(std::make_unique<FileBarWindow>());
    windows_.push_back(std::make_unique<TabBarWindow>());
    windows_.push_back(std::make_unique<PropertyWindow>());
    windows_.push_back(std::make_unique<ExplorerWindow>());
}

void Editor::Draw() {
    for (auto& window : windows_) {
        window->Draw();
    }
}