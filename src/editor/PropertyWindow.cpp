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
    if (!Editor::active_entity_ || Editor::state == Editor::State::Testing) 
    {
        name_ = " ";
        return;
    }

    // Name the window with entity's CName, if it exists
    bool hasCName = Editor::active_entity_->hasComponent<CName>();
    std::string nameTag = hasCName ? Editor::active_entity_->getComponent<CName>()->name : "NULL";
    name_ = "Properties - " + nameTag;

    // Draw a section for each component of the entity
    Editor::active_entity_->forEachComponent([&](auto& component, int index) 
    {
        if (component && component->has) 
        {
            DrawComponent(component);
        }
    });

    // Draw a button for adding new components
    DrawPopupButton("Add Component", Editor::active_entity_,
        ImVec2(ImGui::GetContentRegionMax().x, ImGui::GetTextLineHeight() * 2.0f));
}

void PropertyWindow::PostDraw() 
{
    ImGui::PopStyleVar(6);
}

template <typename T>
void PropertyWindow::DrawComponent(T& component) 
{
    bool isOpen = true;

    if (ImGui::CollapsingHeader(component->componentName, &isOpen, tree_node_flags)) 
    {
        // Hacky solution to draw a button within header that allows user to add a new binding
        if constexpr (std::is_same_v<T, std::shared_ptr<CUserInput>>) 
        {
            // Position the button to the right of the header
            static const char* name = "Add Input";
            ImGui::SameLine(); // same line as header
            ImGui::SetCursorPosX(ImGui::GetWindowWidth() - (ImGui::CalcTextSize(name).x * 1.05) - 40); // Right flush by 40 pixels
            ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.0f, 0.0f, 0.0f, 0.0f)); // Make button transparent
            DrawPopupButton(name, component, ImVec2(ImGui::CalcTextSize(name).x * 1.05, ImGui::GetTextLineHeight() * 1.75));
            ImGui::PopStyleColor();
        }

        if (ImGui::BeginTable(component->componentName, 2, table_flags)) 
        {
            DrawComponentProperties(component);
            ImGui::EndTable();
        }
    }

    if (!isOpen) 
    {
        Editor::active_entity_->removeComponent(component);
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

void PropertyWindow::DrawComponentProperties(std::shared_ptr<CRigidBody> rigidbody) 
{
	DrawProperty("Is Static", rigidbody->staticBody);
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

template <typename T>
void PropertyWindow::DrawPopupButton(const char* name, T& subject, ImVec2 size) 
{
    if (ImGui::Button(name, size)) 
    {
        ImGui::OpenPopup(name);
    }

    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(10.0f, 10.0f));
    if (ImGui::BeginPopup(name)) 
    {
        DrawPopup(subject);
        ImGui::EndPopup();
    }
    ImGui::PopStyleVar();
}

void PropertyWindow::DrawPopup(std::shared_ptr<CUserInput> userinput) 
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
        else if (userinput->mouseMap.find(static_cast<sf::Mouse::Button>(inputSelection)) == userinput->mouseMap.end()) {
            userinput->mouseMap.emplace(static_cast<sf::Mouse::Button>(inputSelection), Action::NoAction);
        }
        ImGui::CloseCurrentPopup();
    }
}

void PropertyWindow::DrawPopup(std::shared_ptr<Entity> entity)
{
    // Use a drop-down (Combo) to select an input from the correct map
    ImGui::Text("Select component");
    static int selection = 0;
    static const char* selectionName = "";

    if (ImGui::BeginCombo("##Components", selectionName))
    {
        Editor::active_entity_->forEachComponent([&](auto& component, int index)
        {
            // Don't display components that already exist
            if (component && component->has) return;

            bool isSelected = (selection == index);
            if (ImGui::Selectable(component->componentName, isSelected))
            {
                selection = index;
                selectionName = component->componentName;
            }
        });
        ImGui::EndCombo();
    }
    ImGui::NewLine();

    // Draw button that, when pressed, find & initialize the component if it is nullptr
    if (ImGui::Button("Create"))
    {
        Editor::active_entity_->forEachComponent([&](auto& component, int index)
        {
            if (!component && selection == index)
            {
                Editor::active_entity_->addComponent(component);
            }
        });
        ImGui::CloseCurrentPopup();
    }
}