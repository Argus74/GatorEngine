#include "FileBarWindow.h"
#include "nfd.h"
#include "../GameEngine.h"

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
            if (ImGui::MenuItem("Open")) {
                nfdchar_t* outPath = NULL;
                nfdfilteritem_t filterItem[1] = { { "Scene files", "scene" } };
                nfdresult_t result = NFD_OpenDialog(&outPath, filterItem, 1, NULL);

                
                if (result == NFD_OKAY) { //If a file/path is selected in the dialog
                    std::string pathString(outPath); 
                    GameEngine::GetInstance().changeScene(pathString);
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
            if (ImGui::MenuItem("Save")) {
                nfdchar_t* outPath = NULL;
                nfdfilteritem_t filterItem[1] = { { "Scene files", "scene" } };
                nfdresult_t result = NFD_SaveDialog(&outPath, filterItem, 1, NULL, NULL);
                if (result == NFD_OKAY) { 
                    std::string pathString(outPath); 
                    GameEngine::GetInstance().saveScene(outPath);
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

        //For now placing the Upload Sprite in the File Bar
        if (ImGui::BeginMenu("Upload"))
        {
            if (ImGui::MenuItem("Upload Sprite")) { 
                auto& assetManager = AssetManager::GetInstance();
                nfdchar_t* outPath = NULL;
                nfdfilteritem_t filterItem[] = {
                    { "PNG files", "png" },
                    { "JPEG files", "jpg,jpeg" }
                };
                nfdresult_t result = NFD_OpenDialog(&outPath, filterItem, 2, NULL); 


                
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
