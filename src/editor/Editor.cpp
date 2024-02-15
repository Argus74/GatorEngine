#include "Editor.h"


Editor::Editor() {
    // Setup default, global style vars for consistent look
    ImGuiStyle& style = ImGui::GetStyle();
    style.WindowBorderSize = 0.0f;
    style.FrameBorderSize = 1.0f;
    style.ScrollbarRounding = 2.0f;
    style.GrabRounding = 2.0f;
    ImGui::StyleColorsLight();
    style.Colors[ImGuiCol_WindowBg] = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);

    // TODO: Initialize all UIWindows of windows_
    //windows_.push_back(std::make_unique<PropertyWindow>());
    //windows_.push_back(std::make_unique<ExplorerWindow>());
    //windows_.push_back(std::make_unique<ExplorerWindow>());
}

void Editor::Draw() {
    // TODO: Uncomment once UIWindows derivations are implemented
    //for (auto& window : windows_) {
    //    window->Draw();
    //}
}