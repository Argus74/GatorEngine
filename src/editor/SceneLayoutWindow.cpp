#include "Config.h"
#include "SceneLayoutWindow.h"

static const short kSelectionBoxBorder = 5; // Border thickness around selected entities

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
	if (Editor::state != Editor::State::Selecting && Editor::state != Editor::State::Moving) return;

	// Draw a draggable selection box for each entity with a transform component
	auto entityList = EntityManager::GetInstance().getEntitiesRenderingList(); //Drawing the draggable boxes in the order of the rendering list, Didn't really change much. Might be an Imgui thing - Alec
	for (int i = 0; i < entityList.size(); i++) {
		auto entity = entityList[i];

		if (!entity->hasComponent<CTransform>()) continue;

		// If user decided to make entity unselectable, skip it
		if (entity->hasComponent<CInformation>() &&
			!entity->getComponent<CInformation>()->selectable) continue;

		auto& transform = *(entity->getComponent<CTransform>());
		std::vector<float> dimensions = GetSelectionBoxDimensions(entity);
		short borderSize = (Editor::active_entity_ == entity) ? kSelectionBoxBorder : 0; // Show only active entity's border
		std::string label = "##DraggableBox" + std::to_string(i);

		// Calculate selection box dimensions and draw it
		ImGui::SetCursorPos(ImVec2(dimensions[0], dimensions[1]));
		ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, borderSize);
		if (ImGui::Button(label.c_str(), ImVec2(dimensions[2], dimensions[3]))) {
			Editor::active_entity_ = entity; // On click, make this entity the active one
		}
		ImGui::PopStyleVar();

		// If not moving (just selecting), skip the rest of the loop
		if (Editor::state != Editor::State::Moving) continue;

		//// Or if user decided to make entity draggable, also skip
		//if (entity->hasComponent<CEditorOptions>() &&
		//	!entity->getComponent<CEditorOptions>()->draggable) continue;

		// If this selection box is being dragged & is draggable, move the entity
		// TODO: Investigate why button drags behind sprite a bit
		if (ImGui::IsItemActive() &&
			ImGui::IsMouseDragging(ImGuiMouseButton_Left)) {
			transform.position.x += ImGui::GetIO().MouseDelta.x;
			transform.position.y += ImGui::GetIO().MouseDelta.y;
		}
	}
}

void SceneLayoutWindow::PostDraw() {
	ImGui::PopStyleColor(4);
}

std::vector<float> SceneLayoutWindow::GetSelectionBoxDimensions(const std::shared_ptr<Entity> entity) {
	auto& transform = *(entity->getComponent<CTransform>());
	std::vector<float> dimensions = { 0, 0, 0, 0 };

	// Set selection box size, depending on the entity's current sprite
	if (entity->hasComponent<CSprite>()) {
		auto& sprite = entity->getComponent<CSprite>()->sprite_;
		dimensions[2] = sprite.getGlobalBounds().width + (kSelectionBoxBorder * 3 / 2); // to make border line up correctly
		dimensions[3] = sprite.getGlobalBounds().height + (kSelectionBoxBorder * 3 / 2);
	}
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