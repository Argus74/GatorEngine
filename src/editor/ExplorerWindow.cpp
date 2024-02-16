#include "ExplorerWindow.h"

#include "imgui-SFML.h"

#include "../EntityManager.h"
#include "../AssetManager.h";

ExplorerWindow::ExplorerWindow() {
	name_ = "Explorer";

	// Disable window-wide scrollbar to allow ListBox to scroll
	window_flags_ |= ImGuiWindowFlags_NoScrollbar;

	// Load icon texture and store reference for easy access. TODO: Maybe dont lol
	auto& assetManager = AssetManager::GetInstance();
	assetManager.AddTexture("GameObjectIconSmall", "assets/GameObjectIconSmall.png");
	icon_ = assetManager.GetTexture("GameObjectIconSmall");
}

void ExplorerWindow::SetPosition() {
	// Make the window 20% of the main viewport's width and 40% of its height
	const ImGuiViewport* mainViewport = ImGui::GetMainViewport();
	short windowWidth = mainViewport->Size.x * 0.20;
	short windowHeight = mainViewport->Size.y * 0.40;

	// Position the window right above the property window
	short windowXPos = mainViewport->Size.x - windowWidth;
	short windowYPos = mainViewport->Size.y * .625 - windowHeight; // Aligning under tabs bar

	ImGui::SetNextWindowPos(ImVec2(windowXPos, windowYPos));
	ImGui::SetNextWindowSize(ImVec2(windowWidth, windowHeight));
}

void ExplorerWindow::PreDraw() {
	// Push style vars
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
	ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(4, 5));
	ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 0.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(8, 10));
}

void ExplorerWindow::DrawFrames() {
	
	// Draw the list box
	if (ImGui::BeginListBox("##Explorer", ImVec2(ImGui::GetContentRegionAvail().x, -1))) {
		
		// Draw a list item for each entity
		auto& entities = EntityManager::GetInstance().getEntities();
		for (auto entity : entities) {
			
			// Draw little icon
			ImGui::Image(icon_, ImVec2(ImGui::GetTextLineHeight(), ImGui::GetTextLineHeight()));
			ImGui::SameLine();

			// Draw the entity's tag as a selectable item
			const bool is_selected = (active_entity_ == entity);
			if (ImGui::Selectable(entity->tag().c_str(), is_selected))
				active_entity_ = entity;
		}
		ImGui::EndListBox();
	}
}

void ExplorerWindow::PostDraw() {
	// Clear style vars
	ImGui::PopStyleVar(4);
}
