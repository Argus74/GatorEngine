#include "SceneLayoutWindow.h"

static const short kBORDER_SIZE = 5;

SceneLayoutWindow::SceneLayoutWindow() {
	name_ = "Scene Layout";

	window_flags_ |= ImGuiWindowFlags_NoBackground;
	window_flags_ |= ImGuiWindowFlags_NoTitleBar;
	window_flags_ |= ImGuiWindowFlags_NoScrollbar;
}

void SceneLayoutWindow::SetPosition() {
	// 80% of viewport's width, (almost) 80% of its height
	const ImGuiViewport* mainViewport = ImGui::GetMainViewport();
	short windowWidth = mainViewport->Size.x * 0.80;
	short windowHeight = mainViewport->Size.y * 0.80 - 20;

	short windowXPos = 0;// Right side of window
	short windowYPos = mainViewport->Size.y * .20 + 20; // RIght under tab bar

	ImGui::SetNextWindowPos(ImVec2(windowXPos, windowYPos));
	ImGui::SetNextWindowSize(ImVec2(windowWidth, windowHeight));
}

void SceneLayoutWindow::PreDraw() {
	// Make selection box invisible by default
	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.0f, 0.0f, 0.0f, 0.0f));
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.0f, 0.0f, 0.0f, 0.0f));
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.45f, 0.78f, 0.98f, 0.10f));
	ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0.25f, 0.58f, 0.98f, 0.50f));
}

void SceneLayoutWindow::DrawFrames() {
	// TODO: Add logic here to prevent drawing buttons when not in moving or selecting state;
	// Draw a draggable selection box for each entity with a transform component
	auto entityList = EntityManager::GetInstance().getEntitiesRenderingList(); //Drawing the draggable boxes in the order of the rendering list, Didn't really change much. Might be an Imgui thing - Alec
	for (int i = 0; i < entityList.size(); i++) {
		if (!entityList[i]->hasComponent<CTransform>()) continue;
		auto& transform = *(entityList[i]->getComponent<CTransform>());
		std::vector<float> dimensions = GetSelectionBoxDimensions(entityList[i]);
		if (Editor::state == Editor::State::Selecting || Editor::state == Editor::State::Moving) {

			// Make border non-zero if this is the active entity
			short borderSize = (Editor::active_entity_ == entityList[i]) ? kBORDER_SIZE : 0;
			ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, borderSize);

			// Calculate selection box dimensions and draw it
			ImGui::SetCursorPos(ImVec2(dimensions[0], dimensions[1]));

			std::string label = "##DraggableBox" + std::to_string(i);
			if (ImGui::Button(label.c_str(), ImVec2(dimensions[2], dimensions[3]))) {
				// On click, set this entity as the active entity
				Editor::active_entity_ = entityList[i];
			}

			// If this selection box is being dragged, move the entity
			// TODO: Add logic here to prevent moving when not in moving state
			// TODO: Investigate why button drags behind sprite a bit
			auto& transform = *(entityList[i]->getComponent<CTransform>());
			if (ImGui::IsItemActive() &&
				ImGui::IsMouseDragging(ImGuiMouseButton_Left)) {
				transform.position.x += ImGui::GetIO().MouseDelta.x;
				transform.position.y += ImGui::GetIO().MouseDelta.y;
				Editor::state = Editor::State::Moving;
			}


			ImGui::PopStyleVar();
		}
		// If out-of-bounds, snap back to bounds
		// TODO: This may break once we start camera stuff... if we ever do?
		// TODO: Do you want to clamp more since we're using a center origin for sprites?
		short centerToEdgeX = dimensions[2] / 2;
		short centerToEdgeY = dimensions[3] / 2;
		if (transform.position.x - centerToEdgeX < 0) {
			transform.position.x = centerToEdgeX;
		}
		if (transform.position.y - centerToEdgeY < 0) {
			transform.position.y = centerToEdgeY;
		}
		if (transform.position.x + centerToEdgeX > (ImGui::GetMainViewport()->Size.x * .80)) {
			transform.position.x = ImGui::GetMainViewport()->Size.x * .80 - centerToEdgeX;
		}
		if (transform.position.y + centerToEdgeY > (ImGui::GetMainViewport()->Size.y * .80 - 20)) {
			transform.position.y = (ImGui::GetMainViewport()->Size.y * .80 - 20) - centerToEdgeY;
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
		dimensions[2] = sprite.getGlobalBounds().width + (kBORDER_SIZE * 3 / 2); // to make border line up correctly
		dimensions[3] = sprite.getGlobalBounds().height + (kBORDER_SIZE * 3 / 2);
	}
	else if (entity->hasComponent<CAnimation>()) {
		auto& sprite = entity->getComponent<CAnimation>()->animation_.sprite_;
		dimensions[2] = sprite.getGlobalBounds().width + (kBORDER_SIZE * 3 / 2);
		dimensions[3] = sprite.getGlobalBounds().height + (kBORDER_SIZE * 3 / 2);
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