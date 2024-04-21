#include "TabBarWindow.h"
#include "Editor.h"

#include "imgui-SFML.h"

#include "Config.h"
#include "../AssetManager.h"
#include "../EntityManager.h"

static const char* kShowGrid = "Show Grid";
static const char* kSnapToGrid = "Grid Snap";
static const char* kGridSize = "##GridSize";

float GetGridPositionX(int index)
{
    ImGuiViewport* mv = ImGui::GetMainViewport();
    return WINDOW_HEIGHT(mv) / 20 + index * (TAB_BUTTON_SIZE(mv) + 40);
}

// Draw label on top of group of buttons, beginning at startIndex through endIndex
void DrawSectionLabel(const char* label, int startIndex, int endIndex)
{
    const ImGuiViewport* mv = ImGui::GetMainViewport();
    short startX = GetGridPositionX(startIndex);
    short endX = GetGridPositionX(endIndex + 1);
    short separatorWidth = endX - startX;
    short y = (TAB_ROW_YOFFSET(mv)) / 2 + 5; // Hardcoded 5px offset downwards

    ImGui::SetCursorPos(ImVec2(startX, y));
    ImGui::PushClipRect(ImVec2(startX, y), ImVec2(endX, y + 50), true);
    ImGui::SeparatorText(label);
    ImGui::PopClipRect();
}

void DrawGridButtons(int index)
{
    static short textHeight = ImGui::GetTextLineHeight();
    ImGui::SetCursorPosX(GetGridPositionX(3));
    ImGui::SetCursorPosY(TAB_ROW_YOFFSET(ImGui::GetMainViewport()));
    ImGui::Checkbox(kShowGrid, &Editor::show_grid_);
    ImGui::SetCursorPosX(GetGridPositionX(3));
    ImGui::Checkbox(kSnapToGrid, &Editor::snap_to_grid_);
    ImGui::SetCursorPosX(GetGridPositionX(3));
    ImGui::SetNextItemWidth(ImGui::CalcTextSize(kSnapToGrid).x + 22); // +20 px for checkbox
    ImGui::InputInt(kGridSize, &Editor::grid_size_);
    // Clamp grid size to a reasonable range
    if (Editor::grid_size_ < 1) {
        Editor::grid_size_ = 1;
    }
    else if (Editor::grid_size_ > 500) {
        Editor::grid_size_ = 500;
    }
}

void TabBarWindow::DrawButton(const char* name, sf::Texture& texture, int index, std::function<void()> onClick, bool highlighted)
{
    // Math to resize icons_ and maintain their relative position
    auto mv = ImGui::GetMainViewport();
    short imageSize = TAB_BUTTON_SIZE(mv);
    ImVec2 buttonPos = ImVec2(GetGridPositionX(index), TAB_ROW_YOFFSET(mv));

    // Use bold colors for an active button
    if (highlighted) {
        static ImVec4 activeColor = ImVec4(0.25f, 0.58f, 0.98f, 1.0f);
        ImGui::PushStyleColor(ImGuiCol_Button, activeColor);
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, activeColor);
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, activeColor);
    }

    // Draw the image button
    ImGui::SetCursorPos(buttonPos);
    if (ImGui::ImageButton(name, texture, sf::Vector2f(imageSize, imageSize))) {
        onClick();
    }

    // Draw text underneath button
    if (std::string(name) != "Sprite") {
        // Center text underneath based on its size
        short textWidth = ImGui::CalcTextSize(name).x - 8; // CalcTextSize seems to consistently be ~8 pixels off?
        float x = buttonPos.x + ((imageSize - textWidth) / 2);
        ImVec2 buttonTextPos = ImVec2(x, buttonPos.y + imageSize + 10);

        ImGui::SetCursorPos(buttonTextPos);
        ImGui::Text(name);
        // Special case for Sprite button // TODO: Refactor
    }
    else {
        // Center combo box underneath the button
        ImVec2 comboPos = ImVec2(buttonPos.x, buttonPos.y + imageSize + 10);
        ImGui::SetCursorPos(comboPos);

        // Set the width of the combo box to match the text width
        ImGui::PushItemWidth(20);
        ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 0);
        ImGui::Combo("Sprite##", &selectedSpriteIndex_, spriteNameList_.data(), spriteNameList_.size());
        ImGui::PopStyleVar();
        ImGui::PopItemWidth();
    }

    if (highlighted) {
        ImGui::PopStyleColor(3);
    }
}

TabBarWindow::TabBarWindow()
{
    name_ = "My Tabs";
    window_flags_ |= ImGuiWindowFlags_NoTitleBar;
}

void TabBarWindow::SetPosition()
{
    const ImGuiViewport* mv = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(ImVec2(TAB_XOFFSET, TAB_YOFFSET));
    ImGui::SetNextWindowSize(ImVec2(TAB_WIDTH(mv), TAB_HEIGHT(mv)));
}

void TabBarWindow::DrawFrames()
{
    if (ImGui::BeginTabBar("MainTabs"))
    {
        if (Editor::state != Editor::State::Testing) // only available for use when user isn't testing the game
        {
            if (ImGui::BeginTabItem("Sprites"))
            {
                // === Tools ==================
                DrawSectionLabel("Tools", 0, 3);

                // Select button
                auto selectButton = [&]() {
                    Editor::state = (Editor::state == Editor::State::Selecting) ? 
                        Editor::State::None : 
                        Editor::State::Selecting;
				};
                DrawButton("Select", AssetManager::GetInstance().GetTexturePrivate("SelectIcon"), 
                    0, selectButton, (Editor::state == Editor::State::Selecting));

                // Move button
                auto moveButton = [&]() {
                    Editor::state = (Editor::state == Editor::State::Moving) ?
                        Editor::State::None :
                        Editor::State::Moving;
                };
                DrawButton("Move", AssetManager::GetInstance().GetTexturePrivate("MoveIcon"), 
                    1, moveButton, (Editor::state == Editor::State::Moving));

                // Scale button
                auto scaleButton = [&]() {
                    Editor::state = (Editor::state == Editor::State::Resizing) ?
                        Editor::State::None :
                        Editor::State::Resizing;
                };
                DrawButton("Scale", AssetManager::GetInstance().GetTexturePrivate("ScaleIcon"), 
                    2, scaleButton, (Editor::state == Editor::State::Resizing));

                //// TODO: Rotate button

                // Grid options
                DrawGridButtons(3);

                /// === Insert =================
                DrawSectionLabel("Insert", 6, 10);

                // Sprite button
                spriteNameList_ = AssetManager::GetInstance().GenerateAssetNameList("textures");
                auto spriteButton = [&]() {
                    auto entity = EntityManager::GetInstance().addEntity("Sprite");
                    entity->addComponent<CTransform>(Vec2(50, 50));
                    entity->addComponent<CName>("Sprite");
                    entity->addComponent<CSprite>(spriteNameList_[selectedSpriteIndex_]);
                    entity->addComponent<CInformation>();
                    GatorPhysics::GetInstance().createBody(entity.get(), true);
                    EntityManager::GetInstance().sortEntitiesForRendering();
                    EntityManager::GetInstance().UpdateUIRenderingList();

                };
                DrawButton("Sprite", AssetManager::GetInstance().GetTexture(spriteNameList_[selectedSpriteIndex_]),
                    6, spriteButton);

                // Game Object button
                auto gameObjectButton = [&]() {
                    auto entity = EntityManager::GetInstance().addEntity("GameObject");
                    entity->addComponent<CTransform>(Vec2(50, 50));
                    entity->addComponent<CName>("Game Object");
                    entity->addComponent<CInformation>();
                    EntityManager::GetInstance().sortEntitiesForRendering();
                    EntityManager::GetInstance().UpdateUIRenderingList();
				};
                DrawButton("Game Object", AssetManager::GetInstance().GetTexturePrivate("GameObjectIcon"), 
                    7, gameObjectButton);

                // Background button
                auto backgroundButton = [&]() {
                    auto entity = EntityManager::GetInstance().addEntity("Background");
                    entity->addComponent<CBackgroundColor>();   
                    entity->addComponent<CSprite>("DefaultBackground");
                    entity->getComponent<CName>()->name = "Background";
                    entity->getComponent<CInformation>()->layer = 0;
                    entity->getComponent<CInformation>()->selectable = false; // Disable selection to avoid accidental moving of background

                    // Calculate transform parameters that will make that default sprite^ fit window
                    const ImGuiViewport* mainViewport = ImGui::GetMainViewport();
                    float sceneWidth = SCENE_WIDTH(mainViewport);
                    float sceneHeight = SCENE_HEIGHT(mainViewport);

                    const Vec2 pos = Vec2(
                        sceneWidth / 2,
                        sceneHeight / 2
                    );

                    entity->getComponent<CTransform>()->position = pos;
                    entity->getComponent<CTransform>()->resetPosition();

                    auto sprite = entity->getComponent<CSprite>()->sprite;
                    const Vec2 scl = Vec2(
                        static_cast<float>(sceneWidth) / sprite.getLocalBounds().width,
                        static_cast<float>(sceneHeight) / sprite.getLocalBounds().height
                    );

                    entity->getComponent<CTransform>()->scale = scl;
                };
                DrawButton("Background", AssetManager::GetInstance().GetTexturePrivate("BackgroundIcon"), 
                    8, backgroundButton);

                // Player button
                auto playerButton = [&]() {
                    auto m_player = EntityManager::GetInstance().addEntity("Player");
                    m_player->addComponent<CName>("Player");
                    m_player->addComponent<CInformation>();
                    m_player->addComponent<CTransform>(Vec2(50, 50));
                    m_player->addComponent<CHealth>();
                    auto anim = m_player->addComponent<CAnimation>();
                    anim->animation = AssetManager::GetInstance().GetAnimation("DefaultAnimation");
                    auto input = m_player->addComponent<CUserInput>();
                    input->key_map = { 
                        {sf::Keyboard::Space, Action::Jump}, 
                        {sf::Keyboard::S, Action::MoveDown}, // TODO: replace this?
                        {sf::Keyboard::A, Action::MoveLeft},
                        {sf::Keyboard::D, Action::MoveRight}
                    };
                    GatorPhysics::GetInstance().createBody(m_player.get(), false);
                };
                DrawButton("Player", AssetManager::GetInstance().GetTexturePrivate("PlayerIcon"), 
                    9, playerButton);

                // Collectible button
                auto collectibleButton = [&]() {
                    // TODO: Implement collectible button
                };
                DrawButton("Collectible", AssetManager::GetInstance().GetTexturePrivate("CollectibleIcon"),
                    10, collectibleButton);

                ImGui::EndTabItem();
            }
        }

        // Testing tab
        if (ImGui::BeginTabItem("Testing"))
        {
            // Start Button
            static auto startButton = [&]() {
                Editor::state = Editor::State::Testing; // Button clicked: now testing, TODO: disable clicking on the explorer & property window
                // TODO: start game: init?
            };
            DrawButton("Start", AssetManager::GetInstance().GetTexturePrivate("StartIcon"), 
                4, startButton, (Editor::state == Editor::State::Testing));

            // Stop Button
            static auto stopButton = [&]() {
                Editor::state = Editor::State::None; // Testing stopped: Reset state to none
                EntityManager::GetInstance().resetPositions();
                // TODO: end game: unload content/reset entity pos?
            };
            DrawButton("Stop", AssetManager::GetInstance().GetTexturePrivate("StopIcon"), 5, stopButton);

            // TODO: add pause button?
            // TODO: add step forward button?
            // TODO: add step back button?
            ImGui::EndTabItem();
        }

        ImGui::EndTabBar();
    }
}