#include "PropertyWindow.h"

#include <iostream>

#include "SFML/Graphics.hpp"
#include "misc/cpp/imgui_stdlib.h"

#include "../EntityManager.h"
#include "../Entity.h"

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
    if (!active_entity_)
    {
        name_ = " ";
        return;
    }

    // Update title of Property Window to include the tag of the entity
    name_ = "Properties - " + active_entity_->tag();

    // Draw a section for each components of the entity
    if (active_entity_->getTransform())
    {
        DrawComponent("Transform", active_entity_->getTransform());
    }
    if (active_entity_->getShape())
    {
        DrawComponent("Shape", active_entity_->getShape());
    }
    if (active_entity_->getNameComponent())
    {
        DrawComponent("Name", active_entity_->getNameComponent());
    }

    // Maybe TODO: At end of window, draw a button for adding new components
    if (ImGui::Button("Add Component", ImVec2(ImGui::GetContentRegionMax().x, ImGui::GetTextLineHeight() * 2.0f)))
    {
        // active_entity_->addComponent(newComponent);
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
        if (ImGui::BeginTable(name, 2, table_flags))
        {
            DrawComponentProperties(component);
        }
        ImGui::EndTable();
    }

    // Maybe TODO: Allow removing of component from entity if user closes the header
    if (!isOpen)
    {
        // active_entity_->removeComponent(component);
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

void PropertyWindow::DrawInputField(std::string &val)
{
    ImGui::InputText("##String", &val);
}

void PropertyWindow::DrawInputField(sf::Vector2f &val)
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
