#include "FileBarWindow.h"

FileBarWindow::FileBarWindow() {
    name_ = "File Bar";
}

void FileBarWindow::SetPosition() {
    // This is the only UIWindow that doesn't need to be positioned
}

void FileBarWindow::DrawFrames() {
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

        //For now placing the Upload Sprite in the File Bar
        if (ImGui::BeginMenu("Upload Sprite"))
        {
            ImGui::MenuItem("Stuff Here");
            ImGui::EndMenu();
        }
        ImGui::EndMainMenuBar();
    }
}
