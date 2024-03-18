#include "PropertyWindow.h"

#include <iostream>

#include "SFML/Graphics.hpp"
#include "misc/cpp/imgui_stdlib.h"

#include "../EntityManager.h"
#include "../Entity.h"
#include "Editor.h"

PropertyWindow::PropertyWindow()
{
    window_flags_ |= ImGuiWindowFlags_AlwaysVerticalScrollbar;

    table_flags = 0;
    table_flags |= ImGuiTableFlags_Borders;
    table_flags &= ~ImGuiTableFlags_BordersOuterV;
    table_flags &= ~ImGuiTableFlags_BordersOuterH;
    table_flags |= ImGuiTableFlags_NoPadInnerX;
    table_flags |= ImGuiTableFlags_NoPadOuterX;

    tree_node_flags = 0;
    tree_node_flags |= ImGuiTreeNodeFlags_DefaultOpen;
}

void PropertyWindow::SetPosition()
{
    // 20% of viewport's width, (almost) 40% of its height
    const ImGuiViewport *mainViewport = ImGui::GetMainViewport();
    short windowWidth = mainViewport->Size.x * 0.20;
    short windowHeight = mainViewport->Size.y * 0.40 - 20; // Hardcoding to fit to bottom

    short windowXPos = mainViewport->Size.x - windowWidth; // Right side of window
    short windowYPos = mainViewport->Size.y * .60 + 20;    // Hardcoding under the explorer

    ImGui::SetNextWindowPos(ImVec2(windowXPos, windowYPos));
    ImGui::SetNextWindowSize(ImVec2(windowWidth, windowHeight));
}

void PropertyWindow::PreDraw()
{
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(4, 5));
    ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_CellPadding, ImVec2(0, 9)); // x won't change stuff?
    ImGui::PushStyleVar(ImGuiStyleVar_IndentSpacing, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));
}

void PropertyWindow::DrawFrames()
{

    // Draw blank window if no active entity
    if (!Editor::active_entity_)
    {
        name_ = " ";
        return;
    }

    // Update title of Property Window to include the tag of the entity
    name_ = "Properties - " + Editor::active_entity_->getComponent<CName>()->name;

    // Draw a section for each components of the entity
    if (Editor::active_entity_->getComponent<CName>())
    {
		DrawComponent("Name", Editor::active_entity_->getComponent<CName>());
	}
    if (Editor::active_entity_->getComponent<CTransform>())
    {
        DrawComponent("Transform", Editor::active_entity_->getComponent<CTransform>());
    }
    if (Editor::active_entity_->getComponent<CShape>())
    {
        DrawComponent("Shape", Editor::active_entity_->getComponent<CShape>());
    }
    if (Editor::active_entity_->getComponent<CUserInput>())
	{
		DrawComponent("User Input", Editor::active_entity_->getComponent<CUserInput>());
	}
    if (Editor::active_entity_->hasComponent<CAnimation>()) {
        DrawComponent("Animation", Editor::active_entity_->getComponent<CAnimation>());
    }
    if (Editor::active_entity_->hasComponent<CSprite>()) {
        DrawComponent("Sprite", Editor::active_entity_->getComponent<CSprite>());
    }

    // Maybe TODO: At end of window, draw a button for adding new components
    if (ImGui::Button("Add Component", ImVec2(ImGui::GetContentRegionMax().x, ImGui::GetTextLineHeight() * 2.0f)))
    {
        // Editor::active_entity_->addComponent(newComponent);
    }
}

void PropertyWindow::PostDraw()
{
    ImGui::PopStyleVar(6);
}

template <typename T>
void PropertyWindow::DrawComponent(const char *name, const T &component)
{
    bool isOpen = true;

    if (ImGui::CollapsingHeader(name, &isOpen, tree_node_flags))
    {
        // Hacky solution to draw a button for the UserInput component
        if constexpr (std::is_same_v<T, std::shared_ptr<CUserInput>>) {
            DrawUserInputAddButton(component);
        }

        if (ImGui::BeginTable(name, 2, table_flags))
        {
            DrawComponentProperties(component);
            ImGui::EndTable();
        }
    }

    // Maybe TODO: Allow removing of component from entity if user closes the header
    if (!isOpen)
    {
        //Editor::active_entity_->removeComponent(component);
    }
}

void PropertyWindow::DrawComponentProperties(std::shared_ptr<CTransform> transform)
{
    // TODO: Update below. These are based on the placeholder components in Entity.h
    DrawProperty("Position", transform->position);
    DrawProperty("Scale", transform->scale);
    DrawProperty("Velocity", transform->velocity);
    DrawProperty("Angle", transform->angle);
}

void PropertyWindow::DrawComponentProperties(std::shared_ptr<CName> name)
{
    // TODO: Update below. These are based on the placeholder components in Entity.h
    DrawProperty("Name", name->name);
}

void PropertyWindow::DrawComponentProperties(std::shared_ptr<CShape> shape)
{
    // TODO: Update below. These are based on the placeholder components in Entity.h
    DrawProperty("Type", shape->type);
    DrawProperty("Color", shape->color);
}

void PropertyWindow::DrawComponentProperties(std::shared_ptr<CUserInput> userinput)
{
    for (auto& entry : userinput->mouseMap) {
        DrawProperty(kSFMLMouseNames[static_cast<int>(entry.first)], entry.second);
    }
    for (auto& entry : userinput->keyMap) {
        DrawProperty(kSFMLKeyNames[static_cast<int>(entry.first)], entry.second);
    }
}

void PropertyWindow::DrawComponentProperties(std::shared_ptr<CSprite> sprite)
{
    DrawProperty("Sprite Name", sprite->name_);
    DrawProperty("Draw Sprite", sprite->drawSprite_);
}

void PropertyWindow::DrawComponentProperties(std::shared_ptr<CAnimation> animation)
{
    DrawProperty("Animation Name", animation->name_);
    DrawProperty("Animation Speed", animation->animationSpeed_);
    DrawProperty("Disappear", animation->disappear_);
}

// TODO: Add new overloads for future components here

template <typename T>
void PropertyWindow::DrawProperty(const char *name, T &val)
{
    // 1st column
    ImGui::TableNextColumn();
    ImGui::AlignTextToFramePadding();
    ImGui::SetCursorPosX(ImGui::GetWindowContentRegionMax().x * 0.05f);
    ImGui::Text(name); // Draw the property name

    // 2nd column
    ImGui::TableNextColumn();
    ImGui::PushID(&val);                                 // Assign unique ID to prevent ref conflicts
    ImGui::PushItemWidth(ImGui::GetColumnWidth() * 0.9); // Fit next widget to 90% column width
    ImGui::SetCursorPosX(ImGui::GetCursorPosX() +
                         (ImGui::GetColumnWidth() * 0.05f)); // Offset to center align next widget
    DrawInputField(val);                                     // Draw input field based on type
    ImGui::PopItemWidth();
    ImGui::PopID();
}

void PropertyWindow::DrawInputField(std::string& val)
{
    // Only set value if user presses Enter or loses focus
    std::string buffer = val;
    if (ImGui::InputText("##String", &buffer, ImGuiInputTextFlags_EnterReturnsTrue)) {
        val = buffer;
    } else if (ImGui::IsItemDeactivatedAfterEdit()) {
		val = buffer;
	}
}

void PropertyWindow::DrawInputField(Vec2 &val)
{
    ImGui::InputFloat2("##Vec2", (float *)&val, "%.2f");
}

void PropertyWindow::DrawInputField(sf::Color &val)
{
    // Cast the sf::Color to a float array for ColorPicker widget
    ImVec4 color = ImVec4(val.r / 255.0f, val.g / 255.0f,
                          val.b / 255.0f, val.a / 255.0f);
    ImGui::ColorEdit3("##Color", (float *)&color);
    val = sf::Color(color.x * 255.0f, color.y * 255.0f,
                    color.z * 255.0f, color.w * 255.0f);
}

void PropertyWindow::DrawInputField(float &val)
{
    ImGui::InputFloat("##Float", &val, 0, 0, "%.2f");
}

void PropertyWindow::DrawInputField(int &val)
{
    ImGui::InputInt("##Int", &val, 0, 0);
}

void PropertyWindow::DrawInputField(bool &val)
{
    ImGui::Checkbox("##Bool", &val);
}

void PropertyWindow::DrawInputField(sf::Keyboard::Key& val)
{
    int selection = static_cast<int>(val);
    ImGui::Combo("##Keys", &selection, kSFMLKeyNames, IM_ARRAYSIZE(kSFMLKeyNames));
    val = static_cast<sf::Keyboard::Key>(selection);
}

void PropertyWindow::DrawInputField(sf::Mouse::Button& val)
{
	int selection = static_cast<int>(val);
	ImGui::Combo("##MouseButtons", &selection, kSFMLMouseNames, IM_ARRAYSIZE(kSFMLMouseNames));
	val = static_cast<sf::Mouse::Button>(selection);
}

void PropertyWindow::DrawInputField(Action& val)
{
    int selection = static_cast<int>(val);
    ImGui::Combo("##Actions", &selection, kActionNames, IM_ARRAYSIZE(kActionNames));
    val = static_cast<Action>(selection);
}

void PropertyWindow::DrawUserInputAddButton(std::shared_ptr<CUserInput> userinput) 
{
    // Button within header that, when pressed, allows user to add a new binding
    static const char* buttonTitle = "+ New Item";
    ImGui::SameLine(); // same line as header
    ImGui::SetCursorPosX(ImGui::GetWindowWidth() - (ImGui::CalcTextSize(buttonTitle).x * 1.05) - 40); // Right flush by 40 pixels
    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.0f, 0.0f, 0.0f, 0.0f)); // Make button transparent
    if (ImGui::Button(buttonTitle, ImVec2(ImGui::CalcTextSize(buttonTitle).x * 1.05, ImGui::GetTextLineHeight() * 1.75)))
    {
        ImGui::OpenPopup("InputSelection");
    }
    ImGui::PopStyleColor();

    // Display a pop-up to prompt user to select the input to add
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(10.0f, 10.0f));
    if (ImGui::BeginPopup("InputSelection"))
    {
        // Decide which input type to use so we can display the correct map below
        ImGui::Text("Select input type");
        static int typeSelection = 0;
        ImGui::RadioButton("Keyboard", &typeSelection, 0);
        ImGui::RadioButton("Mouse", &typeSelection, 1);
        ImGui::NewLine();

        // Use a drop-down (Combo) to select an input from the correct map
        ImGui::Text("Select input");
        static int inputSelection = 0;
        if (typeSelection == 0) 
        {
            DrawInputField(reinterpret_cast<sf::Keyboard::Key&>(inputSelection));
        }
        else 
        {
            DrawInputField(reinterpret_cast<sf::Mouse::Button&>(inputSelection));
        }
        ImGui::NewLine();

        // When pressed, add the input to its map, so long as it doesn't already exist
        if (ImGui::Button("Create")) 
        {
            if (typeSelection == 0 && userinput->keyMap.find(static_cast<sf::Keyboard::Key>(inputSelection)) == userinput->keyMap.end())
            {
                userinput->keyMap.emplace(static_cast<sf::Keyboard::Key>(inputSelection), Action::NoAction);
            } 
            else if (userinput->mouseMap.find(static_cast<sf::Mouse::Button>(inputSelection)) == userinput->mouseMap.end()) 
            {
                userinput->mouseMap.emplace(static_cast<sf::Mouse::Button>(inputSelection), Action::NoAction);
            }
            ImGui::CloseCurrentPopup();
        }
        ImGui::EndPopup();
    }
    ImGui::PopStyleVar();
}

