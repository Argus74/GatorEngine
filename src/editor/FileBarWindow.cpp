#include "FileBarWindow.h"
#include "nfd.h"

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
        if (ImGui::BeginMenu("Upload"))
        {
            if (ImGui::MenuItem("Upload Sprite")) { 
                auto& assetManager = AssetManager::GetInstance();
                nfdchar_t* outPath = NULL;
                nfdfilteritem_t filterItem[1] = { { "Image files", "png;jpg;jpeg" } };
                nfdresult_t result = NFD_OpenDialog(&outPath, filterItem, 1, NULL);

                
                if (result == NFD_OKAY) { //If a file/path is selected in the dialog
                    std::string pathString(outPath); 
                    std::string assetName = assetManager.ExtractFilename(pathString);
                    assetManager.AddTexture(assetName, pathString);

                    std::cout << "Selected file: " << outPath << std::endl;
                    NFD_FreePath(outPath);
                }
                else if (result == NFD_CANCEL) { //If no file or path is selecte
                    std::cout << "Dialog canceled." << std::endl;
                }
                else {
                    std::cerr << "Error: " << NFD_GetError() << std::endl;
                }
            }
            ImGui::EndMenu();
        }
        ImGui::EndMainMenuBar();
    }
}
