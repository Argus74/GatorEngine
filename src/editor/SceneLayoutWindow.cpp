#include "Config.h"
#include "SceneLayoutWindow.h"
#include <imgui_internal.h>

static const short kSelectionBoxBorder = 5; // Border thickness around selected entities

// Helper to determine an entity's selection box position and size
// TODO: Consider abstracting this more into a proper bounding box calculation for use in, say, the OnTouch system
std::vector<float> GetSelectionBoxDimensions(const std::shared_ptr<Entity>& entity) {
	auto& transform = *(entity->getComponent<CTransform>());
	std::vector<float> dimensions = { 0, 0, 0, 0 };

	// Set selection box size, depending on the entity's current sprite
	if (entity->hasComponent<CSprite>()) {
		auto& sprite = entity->getComponent<CSprite>()->sprite_;
		dimensions[2] = sprite.getGlobalBounds().width + (kSelectionBoxBorder * 3 / 2); // to make border line up correctly
		dimensions[3] = sprite.getGlobalBounds().height + (kSelectionBoxBorder * 3 / 2);
	}
	// TODO: Bug where dimensions are not correct after resizing animated sprites
	else if (entity->hasComponent<CAnimation>()) {
		auto& sprite = entity->getComponent<CAnimation>()->animation_.sprite_;
		dimensions[2] = sprite.getGlobalBounds().width + (kSelectionBoxBorder * 3 / 2);
		dimensions[3] = sprite.getGlobalBounds().height + (kSelectionBoxBorder * 3 / 2);
	}
	else {
		// If no sprite or animation, I guess just use the transform's scale?
		dimensions[2] = transform.scale.x;
		dimensions[3] = transform.scale.y;
	}

	// Set selection box position (shift left and up by 1/2 its size because sprites are using center origin)
	dimensions[0] = transform.position.x - (dimensions[2] / 2);
	dimensions[1] = transform.position.y - (dimensions[3] / 2);

	return dimensions;
}

void DrawSelectionBox(const std::shared_ptr<Entity>& entity, const std::vector<float>& dimensions, bool isActive) {
	ImGui::SetCursorPos(ImVec2(dimensions[0], dimensions[1]));
	ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, isActive ? kSelectionBoxBorder : 0); // Show border only if active
	ImGui::Button(("##DraggableBox" + std::to_string(entity->id())).c_str(), ImVec2(dimensions[2], dimensions[3]));
	ImGui::PopStyleVar();
}

void HandleSelectInteraction(const std::shared_ptr<Entity>& entity) {
	if (ImGui::IsItemClicked(ImGuiMouseButton_Left)) {
		Editor::active_entity_ = entity; // Set as active entity on click
	}
}

void HandleMoveInteraction(const std::shared_ptr<Entity>& entity) {
	auto& transform = *(entity->getComponent<CTransform>());

	if (ImGui::IsItemActive() && ImGui::IsMouseDragging(ImGuiMouseButton_Left)) {
		transform.position.x += ImGui::GetIO().MouseDelta.x;
		transform.position.y += ImGui::GetIO().MouseDelta.y;
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
	static float kResizeDampen = 0.03f;
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
	// Prevent drawing buttons when not in moving or selecting state
	if (Editor::state != Editor::State::Selecting && 
		Editor::state != Editor::State::Moving &&
		Editor::state != Editor::State::Resizing) return;

	// If resizing, draw resize handles over active entity
	if (Editor::active_entity_ && Editor::state == Editor::State::Resizing) {
		auto transform = Editor::active_entity_->getComponent<CTransform>();
		std::vector<float> dimensions = GetSelectionBoxDimensions(Editor::active_entity_);

		// Draw a resize handle for each X and Y axis on sides of entity, depending on its orientation
		// TODO: This math may be simplified, but I'm too tired to think about it right now
		auto mv = ImGui::GetMainViewport();
		short handleRadius = WINDOW_WIDTH(mv) * 0.006f;
		float xCenter = transform->position.x;
		float yCenter = transform->position.y;
		float halfEdgeX = dimensions[2] / 2;
		float halfEdgeY = dimensions[3] / 2;
		float offset = (handleRadius * 2);
		bool xAxis = true;

		// Left handle
		if (transform->scale.x > 0) {
			DrawResizeHandle(ImVec2(xCenter - halfEdgeX - offset, yCenter),
				handleRadius, xAxis);
		// Right handle
		} else {
			DrawResizeHandle(ImVec2(xCenter + halfEdgeX + offset, yCenter),
				handleRadius, xAxis);
		}
		HandleResizeInteraction(Editor::active_entity_, xAxis);

		// Top handle
		if (transform->scale.y > 0) {
			DrawResizeHandle(ImVec2(xCenter, yCenter - halfEdgeY - offset),
				handleRadius, !xAxis);
		// Bottom handle
		} else {
			DrawResizeHandle(ImVec2(xCenter, yCenter + halfEdgeY + offset),
				handleRadius, !xAxis);
		}
		HandleResizeInteraction(Editor::active_entity_, !xAxis);
	}

	// If selecting, moving, or resizing, draw a selection box over each entity
	auto entityList = EntityManager::GetInstance().getEntitiesRenderingList();
	std::vector<float> dimensions;
	for (const auto& entity : entityList) {
		// Skip unselectable entities
		if (entity->hasComponent<CInformation>() && !entity->getComponent<CInformation>()->selectable) continue;

		// Skip entities without a transform component
		if (!entity->hasComponent<CTransform>()) continue;

		dimensions = GetSelectionBoxDimensions(entity);

		DrawSelectionBox(entity, dimensions, entity == Editor::active_entity_);
		HandleSelectInteraction(entity);
		if (Editor::state == Editor::State::Moving) { // If moving, also handle moving interaction
			HandleMoveInteraction(entity);
		}
	}
}

void SceneLayoutWindow::PostDraw() {
	ImGui::PopStyleColor(4);
}
