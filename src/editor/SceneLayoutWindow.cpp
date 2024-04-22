#include "SceneLayoutWindow.h"

#include <imgui_internal.h>

#include "Config.h"

static constexpr short kSelectionBoxBorder = 5;  // Border thickness around selected entities
static constexpr float kResizeDampen = 0.03f;

void DrawSelectionBox(const std::shared_ptr<Entity>& entity, const sf::FloatRect& dimensions) {
    float borderSize = entity == Editor::active_entity_ ? kSelectionBoxBorder : 0;
    ImGui::SetCursorPos(ImVec2(dimensions.top, dimensions.left));
    ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, borderSize);  // Show border only if active
    ImGui::Button(("##DraggableBox" + std::to_string(entity->id())).c_str(),
                  ImVec2(dimensions.width, dimensions.height));
    ImGui::PopStyleVar();
}

void DrawTriggerBox(const std::shared_ptr<Entity>& entity) {
    if (!entity->hasComponent<CTouchTrigger>())
        return;

    float borderSize = entity == Editor::active_entity_ ? kSelectionBoxBorder : 0;

    auto touchTrigger = entity->getComponent<CTouchTrigger>();
    auto transform = entity->getComponent<CTransform>();
    sf::FloatRect triggerRect;

    triggerRect.width = touchTrigger->trigger_size.x;
    triggerRect.height = touchTrigger->trigger_size.y;
    // Adjust these lines:
    triggerRect.left = transform->position.x - (triggerRect.width / 2);
    triggerRect.top = transform->position.y - (triggerRect.height / 2);
    
    short offsetHeight = ImGui::GetMainViewport()->Size.y * .2 + 20;
    ImVec2 rectMin = ImVec2(triggerRect.left, triggerRect.top + offsetHeight);
    ImVec2 rectMax = ImVec2(rectMin.x + triggerRect.width, rectMin.y + triggerRect.height);

    // Push style for border size
    ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, borderSize);

    ImDrawList* draw_list = ImGui::GetWindowDrawList();

    // Change border color if entity is active
    if (borderSize > 0) {
        // Push the color for the border
        draw_list->AddRect(rectMin, rectMax, IM_COL32(0, 255, 0, 255), 0.0f, 0, borderSize);
    }

    // Pop style for border size
    ImGui::PopStyleVar();
}

void DrawGridLines() {
    ImDrawList* drawList = ImGui::GetWindowDrawList();
    ImVec2 windowPos = ImGui::GetWindowPos();
    ImVec2 windowSize = ImGui::GetWindowSize();
    ImU32 color = IM_COL32(255, 255, 255, 20);  // Add some transparency
    for (float x = Editor::grid_size_ / 2; x < windowSize.x; x += Editor::grid_size_) {
        drawList->AddLine(ImVec2(windowPos.x + x, windowPos.y),
                          ImVec2(windowPos.x + x, windowPos.y + windowSize.y), color);
    }
    for (float y = Editor::grid_size_ / 2; y < windowSize.y; y += Editor::grid_size_) {
        drawList->AddLine(ImVec2(windowPos.x, windowPos.y + y),
                          ImVec2(windowPos.x + windowSize.x, windowPos.y + y), color);
    }
}

// Calculate snap position based on the grid
Vec2 SnapToGrid(const Vec2& position) {
    float x = round(position.x / Editor::grid_size_) * Editor::grid_size_;
    float y = round(position.y / Editor::grid_size_) * Editor::grid_size_;
    return Vec2(x, y);
}

void HandleSelectInteraction(const std::shared_ptr<Entity>& entity) {
    if (ImGui::IsItemClicked(ImGuiMouseButton_Left)) {
        Editor::active_entity_ = entity;  // Set as active entity on click
    }
}

void HandleMoveInteraction(const std::shared_ptr<Entity>& entity) {
    auto& transform = *(entity->getComponent<CTransform>());

    if (ImGui::IsItemActive() && ImGui::IsMouseDragging(ImGuiMouseButton_Left)) {
        // Get the mouse position relative to the current window to match CTransform coordinates
        ImVec2 globalMousePos = ImGui::GetIO().MousePos;
        ImVec2 windowPos = ImGui::GetWindowPos();
        Vec2 localMousePos;
        localMousePos.x = globalMousePos.x - windowPos.x;
        localMousePos.y = globalMousePos.y - windowPos.y;

        // Move the entity to the snap position
        transform.position = (Editor::snap_to_grid_) ? SnapToGrid(localMousePos) : localMousePos;
        transform.origin = (Editor::snap_to_grid_) ? SnapToGrid(localMousePos) : localMousePos;
    }
}

void DrawResizeHandle(const ImVec2& center, float radius, bool xAxis) {
    // Draw an invisible button over the handle that is slightly larger than the handle itself
    float buttonSize = radius * 4.0f;
    ImGui::SetCursorPos(ImVec2(center.x - (buttonSize / 2.0f), center.y - (buttonSize / 2.0f)));
    ImGui::PushID(xAxis);
    ImGui::InvisibleButton("##ResizeHandle", ImVec2(buttonSize, buttonSize));
    ImGui::PopID();

    // Convert relative center coords to screen space so we can use ImGui's internal drawing functions
    // TODO: Wonder if whole editor should be refactored to use screen space coords
    ImGui::SetCursorPos(center);
    ImVec2 centerScreenPos = ImGui::GetCursorScreenPos();
    ImGui::SetCursorScreenPos(centerScreenPos);

    // Represent X axis handles with red, Y axis handles with blue
    ImU32 color = ImU32(xAxis ? IM_COL32(255, 0, 0, 255) : IM_COL32(0, 0, 255, 255));

    // Draw the handle
    ImDrawList* draw_list = ImGui::GetWindowDrawList();
    draw_list->AddCircleFilled(centerScreenPos, radius, color, 32);
    if (ImGui::IsItemHovered() || ImGui::IsItemActive()) {
        draw_list->AddCircle(centerScreenPos, radius * 1.7f, color, 32);
    }
}

void HandleResizeInteraction(const std::shared_ptr<Entity>& entity, bool onX) {
    if (ImGui::IsItemActive() && ImGui::IsMouseDragging(ImGuiMouseButton_Left)) {
        auto transform = entity->getComponent<CTransform>();
        ImVec2 mouseDelta = ImGui::GetIO().MouseDelta;
        if (onX) {
            transform->scale.x -= mouseDelta.x * kResizeDampen;
        } else {
            transform->scale.y -= mouseDelta.y * kResizeDampen;
        }
    }
}

SceneLayoutWindow::SceneLayoutWindow() {
    name_ = "Scene Layout";

    window_flags_ |= ImGuiWindowFlags_NoBackground;
    window_flags_ |= ImGuiWindowFlags_NoTitleBar;
    window_flags_ |= ImGuiWindowFlags_NoScrollbar;
}

void SceneLayoutWindow::SetPosition() {
    const ImGuiViewport* mainViewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(ImVec2(SCENE_XOFFSET, SCENE_YOFFSET(mainViewport)));
    ImGui::SetNextWindowSize(ImVec2(SCENE_WIDTH(mainViewport), SCENE_HEIGHT(mainViewport)));
}

void SceneLayoutWindow::PreDraw() {
    // Make selection box invisible by default
    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.0f, 0.0f, 0.0f, 0.0f));
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.0f, 0.0f, 0.0f, 0.0f));
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.45f, 0.78f, 0.98f, 0.10f));
    ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0.25f, 0.58f, 0.98f, 0.50f));
}

void SceneLayoutWindow::DrawFrames() {
    // If resizing, draw resize handles over active entity
    if (Editor::active_entity_ && Editor::state == Editor::State::Resizing) {
        auto transform = Editor::active_entity_->getComponent<CTransform>();
        auto rect = Editor::active_entity_->GetRect();

        // Draw a resize handle for each X and Y axis on sides of entity, depending on its orientation
        // TODO: This math may be simplified, but I'm too tired to think about it right now
        auto mv = ImGui::GetMainViewport();
        short handleRadius = WINDOW_WIDTH(mv) * 0.006f;
        float xCenter = transform->position.x;
        float yCenter = transform->position.y;
        float halfEdgeX = rect.width / 2;
        float halfEdgeY = rect.height / 2;
        float offset = (handleRadius * 2);
        bool xAxis = true;

        // Left handle
        if (transform->scale.x > 0) {
            DrawResizeHandle(ImVec2(xCenter - halfEdgeX - offset, yCenter), handleRadius, xAxis);
            // Right handle
        } else {
            DrawResizeHandle(ImVec2(xCenter + halfEdgeX + offset, yCenter), handleRadius, xAxis);
        }
        HandleResizeInteraction(Editor::active_entity_, xAxis);

        // Top handle
        if (transform->scale.y > 0) {
            DrawResizeHandle(ImVec2(xCenter, yCenter - halfEdgeY - offset), handleRadius, !xAxis);
            // Bottom handle
        } else {
            DrawResizeHandle(ImVec2(xCenter, yCenter + halfEdgeY + offset), handleRadius, !xAxis);
        }
        HandleResizeInteraction(Editor::active_entity_, !xAxis);
    }

    // If selecting, moving, or resizing, draw a selection box over each entity
    if (Editor::state == Editor::State::Selecting || Editor::state == Editor::State::Moving ||
        Editor::state == Editor::State::Resizing) {

        auto entityList = EntityManager::GetInstance().getEntitiesRenderingList();
        for (const auto& entity : entityList) {
            // Skip unselectable entities
            if (entity->hasComponent<CInformation>() &&
                !entity->getComponent<CInformation>()->selectable)
                continue;

            // Skip entities without a transform component
            if (!entity->hasComponent<CTransform>())
                continue;

            static constexpr float margin = kSelectionBoxBorder * 3 / 2;
            auto dimensions = entity->GetRect(margin); // Don't remove, fixes box bug for mac
            DrawSelectionBox(entity, dimensions);
            DrawTriggerBox(entity);
            HandleSelectInteraction(entity);
            if (Editor::state ==
                Editor::State::Moving) {  // If moving, also handle moving interaction
                HandleMoveInteraction(entity);
            }
        }
    }

    // If showing grid lines, draw them
    if (Editor::show_grid_) {
        DrawGridLines();
    }
}

void SceneLayoutWindow::PostDraw() {
    ImGui::PopStyleColor(4);
}