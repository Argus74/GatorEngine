#include "SceneLayoutWindow.h"


SceneLayoutWindow::SceneLayoutWindow() {
	name_ = "Scene Layout";

	window_flags_ |= ImGuiWindowFlags_NoBackground;
	window_flags_ |= ImGuiWindowFlags_NoTitleBar;
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

void SceneLayoutWindow::DrawFrames() {

	static const short borderSize = 10;

	// TODO: Add logic here to prevent drawing buttons when not in moving or selecting state;

	// Make buttons invisible by default
	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.0f, 0.0f, 0.0f, 0.0f));
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.45f, 0.78f, 0.98f, 0.30f));
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.45f, 0.78f, 0.98f, 0.50f));

	// Draw a draggable box for each entity
	auto entityList = EntityManager::GetInstance().getEntities();
	for (int i = 0; i < entityList.size(); i++) {

		auto& transform = *(entityList[i]->getTransform());
		std::string label = "##DraggableBox" + std::to_string(i);

		ImGui::SetCursorPos(ImVec2(transform.position.x - (borderSize / 2), transform.position.y - (borderSize / 2)));

		// If this is the active entity, make button visible
		bool shouldRemove = false;
		if (Editor::active_entity_ == entityList[i]) {
			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.25f, 0.58f, 0.98f, 0.50f));
			shouldRemove = true;
		}

		// Draw button
		if (ImGui::Button(label.c_str(), ImVec2(transform.scale.x + borderSize, transform.scale.y + borderSize))) {
			Editor::active_entity_ = entityList[i];
		}

		// If this button is being dragged, move the entity
		// TODO: Add logic here to prevent moving when not in moving state
		if (ImGui::IsItemActive() &&
			ImGui::IsMouseDragging(ImGuiMouseButton_Left)) {
			transform.position.x += ImGui::GetIO().MouseDelta.x;
			transform.position.y += ImGui::GetIO().MouseDelta.y;
		}

		// If out-of-bounds, snap back to bounds
		if (transform.position.x < 0) {
			transform.position.x = 0;
		}
		if (transform.position.y < 0) {
			transform.position.y = 0;
		}
		if (transform.position.x + transform.scale.x > (ImGui::GetMainViewport()->Size.x * .80)) {
			transform.position.x = ImGui::GetMainViewport()->Size.x * .80 - transform.scale.x;
		}
		if (transform.position.y + transform.scale.y > (ImGui::GetMainViewport()->Size.y * .80 - 20)) {
			transform.position.y = (ImGui::GetMainViewport()->Size.y * .80 - 20) - transform.scale.y;
		}

		// If this button was previously showing active entity, pop the color
		if (shouldRemove) {
			ImGui::PopStyleColor();
		}
	}

	ImGui::PopStyleColor(3);
}