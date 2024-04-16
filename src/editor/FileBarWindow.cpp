#include "FileBarWindow.h"
#include "nfd.h"
#include <filesystem>

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
                nfdfilteritem_t filterItem[] = {
                    { "PNG files", "png" },
                    { "JPEG files", "jpg;jpeg" }
                };
                nfdresult_t result = NFD_OpenDialog(&outPath, filterItem, 2, NULL); 
                
                if (result == NFD_OKAY) { //If a file/path is selected in the dialog
                    std::string pathString(outPath);
                    std::filesystem::path sourcePath = pathString;
                    std::filesystem::path targetDirectory = "assets/UserAssets";
                    if (!std::filesystem::exists(targetDirectory)) {
                        std::filesystem::create_directories(targetDirectory);
                    }

                    // Create the new file path in the target directory
                    std::string filename = sourcePath.filename().string();
                    std::filesystem::path targetPath = targetDirectory / filename;

                    // Copy the file to the new directory
                    std::filesystem::copy(sourcePath, targetPath, std::filesystem::copy_options::overwrite_existing);

                    // Use the new file path to add the texture
                    std::string newFilePath = targetPath.string();
                    std::cout << "Location of new Sprite file: " << newFilePath << std::endl;
                    std::string assetName = assetManager.ExtractFilename(newFilePath);
                    assetManager.AddTexture(assetName, newFilePath);

                    std::cout << "Selected file: " << outPath << std::endl;
                    NFD_FreePath(outPath);
                }
                else if (result == NFD_CANCEL) { //If no file or path is selected
                    std::cout << "Dialog canceled." << std::endl;
                }
                else {
                    std::cerr << "Error: " << NFD_GetError() << std::endl;
                }
            } 
            if (ImGui::MenuItem("Upload Font")) {
                auto& assetManager = AssetManager::GetInstance();
                nfdchar_t* outPath = NULL;
                nfdfilteritem_t filterItem[] = {
                    { "TrueType fonts", "ttf" }, // Focus only on ttf for direct SFML support
                };
                nfdresult_t result = NFD_OpenDialog(&outPath, filterItem, 1, NULL); // Updated count to 1 as only one filter item is used

                if (result == NFD_OKAY) { //If a file/path is selected in the dialog
                    std::string pathString(outPath);
                    std::filesystem::path sourcePath = pathString;
                    std::filesystem::path targetDirectory = "assets/UserAssets"; // Changed to a more specific directory for fonts
                    if (!std::filesystem::exists(targetDirectory)) {
                        std::filesystem::create_directories(targetDirectory);
                    }

                    // Create the new file path in the target directory
                    std::string filename = sourcePath.filename().string();
                    std::filesystem::path targetPath = targetDirectory / filename;

                    // Copy the file to the new directory
                    std::filesystem::copy(sourcePath, targetPath, std::filesystem::copy_options::overwrite_existing);

                    // Use the new file path to add the font
                    std::string newFilePath = targetPath.string();
                    std::cout << "Location of new font file: " << newFilePath << std::endl;
                    std::string assetName = assetManager.ExtractFilename(newFilePath);
                    assetManager.AddFont(assetName, newFilePath); // Using AddFont instead of AddTexture

                    std::cout << "Selected file: " << outPath << std::endl;
                    NFD_FreePath(outPath);
                }
                else if (result == NFD_CANCEL) { //If no file or path is selected
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
