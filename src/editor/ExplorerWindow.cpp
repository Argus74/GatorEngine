#include "ExplorerWindow.h"

#include "imgui-SFML.h"

#include "../EntityManager.h"
#include "../AssetManager.h";
#include "Editor.h"

#include "Editor.h"

ExplorerWindow::ExplorerWindow() {
	name_ = "Explorer";

	// Disable window-wide scrollbar to allow ListBox to scroll
	window_flags_ |= ImGuiWindowFlags_NoScrollbar;

	// TODO: Maybe don't store ref to icon here
	// Load icon texture and store reference for easy access.
	auto& assetManager = AssetManager::GetInstance();
	assetManager.AddTexture("GameObjectIconSmall", "assets/GameObjectIconSmall.png");
	icon_ = assetManager.GetTexture("GameObjectIconSmall");
}

void ExplorerWindow::SetPosition() {
	// 20% of viewport's width, 40% of its height
	const ImGuiViewport* mainViewport = ImGui::GetMainViewport();
	short windowWidth = mainViewport->Size.x * 0.20;
	short windowHeight = mainViewport->Size.y * 0.40;

	short windowXPos = mainViewport->Size.x - windowWidth; // Right side of window
	short windowYPos = mainViewport->Size.y * .20 + 20; // Hardcoding to be under the tab bar

	ImGui::SetNextWindowPos(ImVec2(windowXPos, windowYPos));
	ImGui::SetNextWindowSize(ImVec2(windowWidth, windowHeight));
}

void ExplorerWindow::PreDraw() {
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
	ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(4, 5));
	ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 0.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(8, 10));
}

void ExplorerWindow::DrawFrames() {
	
	// Draw the list box
	if (ImGui::BeginListBox("##Explorer", ImVec2(ImGui::GetContentRegionAvail().x, -1))) {
		
		// Draw a list item for each entity
		auto& entityList = EntityManager::GetInstance().getEntities();
		for (int i = 0; i < entityList.size(); i++) {
			auto entity = entityList[i];
			// Draw little icon
			ImGui::Image(icon_, ImVec2(ImGui::GetTextLineHeight(), ImGui::GetTextLineHeight()));
			ImGui::SameLine();

			// Draw the entity's tag as a selectable item
			const bool isSelected = Editor::active_entity_ == entity && Editor::state != Editor::State::Testing;
			bool hasName = entity->hasComponent<CName>();
			std::string entityName = hasName ? entity->getComponent<CName>()->name : "NULL";
			std::string label = entityName + "##" + std::to_string(i); // Prevent name conflcits bugs
			if ((ImGui::Selectable(label.c_str(), isSelected) 
				|| ImGui::IsItemClicked(ImGuiMouseButton_Right)) 
				&& Editor::state != Editor::State::Testing) {
				Editor::active_entity_ = entity;
				Editor::state = Editor::State::Selecting;
			}

			// Open context menu on right-click // TODO: Bug if right-click while context menu open
			ImGui::OpenPopupOnItemClick("EntityContextMenu", ImGuiPopupFlags_MouseButtonRight);
		}
		ImGui::EndListBox();
	}

	// Draw context menu
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(8, 8));
	if (ImGui::BeginPopupContextItem("EntityContextMenu")) {
		if (ImGui::Selectable("Clone")) {
			EntityManager::GetInstance().cloneEntity(Editor::active_entity_);
		}
		if (ImGui::Selectable("Delete")) {
			EntityManager::GetInstance().removeEntity(Editor::active_entity_);
		}
		ImGui::EndPopup();
	}
	ImGui::PopStyleVar();
}

void ExplorerWindow::PostDraw() {
	ImGui::PopStyleVar(4);
}
