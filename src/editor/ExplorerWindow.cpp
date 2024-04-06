#include "ExplorerWindow.h"

#include "imgui-SFML.h"

#include "../EntityManager.h"
#include "../AssetManager.h";
#include "Editor.h"

#include "Editor.h"

static constexpr char* kEntityPayloadID = "DND_ENTITY";
static constexpr char* kEntityContextMenuID = "CM_ENTITY";
static constexpr char* kExplorerLabel = "##Explorer";
static constexpr char* kDropTargetLabel = "##Target";
static constexpr char* kCloneLabel = "Clone";
static constexpr char* kDeleteLabel = "Delete";
static constexpr short kDropTargetHeight = 10;


ExplorerWindow::ExplorerWindow() {
	name_ = "Explorer";

	// Disable window-wide scrollbar to allow ListBox to scroll
	window_flags_ |= ImGuiWindowFlags_NoScrollbar;

	// TODO: Maybe don't store ref to icon here
	// Load icon texture and store reference for easy access.
	auto& assetManager = AssetManager::GetInstance();
	assetManager.AddTexturePrivate("GameObjectIconSmall", "assets/GameObjectIconSmall.png");
	icon_ = assetManager.GetTexturePrivate("GameObjectIconSmall");
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
	ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(8, 10));
	ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 0.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(8, 10));
}

void ExplorerWindow::DrawFrames() {
	// Draw the list box
	if (ImGui::BeginListBox(kExplorerLabel, ImVec2(ImGui::GetContentRegionAvail().x, -1))) {

		// Draw a list item for each entity
		auto& entityList = EntityManager::GetInstance().getEntities();
		for (int i = 0; i < entityList.size(); i++) {
			auto entity = entityList[i];
			std::string entityName = entity->hasComponent<CName>() ? entity->getComponent<CName>()->name : "NULL";
			
			// Handle drag-and-drops above this selectable
			DrawDropTarget(i);

			// Draw little icon
			ImGui::Image(icon_, ImVec2(ImGui::GetTextLineHeight(), ImGui::GetTextLineHeight()));
			ImGui::SameLine();

			// Draw the entity's tag as a selectable
			const bool isSelected = Editor::active_entity_ == entity && Editor::state != Editor::State::Testing;
			std::string label = entityName + "##" + std::to_string(i); // Prevent name conflcits bugs
			if (ImGui::Selectable(label.c_str(), isSelected) && Editor::state != Editor::State::Testing) {
				Editor::active_entity_ = entity;
				Editor::state = Editor::State::Selecting;
			}

			// Open context menu on right-click // TODO: Bug if right-click while context menu open
			ImGui::OpenPopupOnItemClick(kEntityContextMenuID, ImGuiPopupFlags_MouseButtonRight);

			// Make this selectable draggable
			if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_AcceptNoDrawDefaultRect)) {
				ImGui::SetDragDropPayload(kEntityPayloadID, &i, sizeof(i));
				ImGui::Text(entityName.c_str());
				ImGui::EndDragDropSource();
			}
		}

		// Handle drag-and-drops at the end of the list
		DrawDropTarget(-1);

		ImGui::EndListBox();
	}

	// Draw context menu
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(8, 8));
	if (ImGui::BeginPopupContextItem(kEntityContextMenuID)) {
		if (ImGui::Selectable(kCloneLabel)) {
			EntityManager::GetInstance().cloneEntity(Editor::active_entity_);
		}
		if (ImGui::Selectable(kDeleteLabel)) {
			EntityManager::GetInstance().removeEntity(Editor::active_entity_);
		}
		ImGui::EndPopup();
	}
	ImGui::PopStyleVar();
}

void ExplorerWindow::PostDraw() {
	ImGui::PopStyleVar(4);
}

void ExplorerWindow::DrawDropTarget(int targetIndex) {
	auto& entityList = EntityManager::GetInstance().getEntities();

	// Store cursor position (top of selectable) to move it back after we draw invisible button
	auto cursorPosBegin = ImGui::GetCursorPosY();

	// Move cursor halfway down to center button
	ImGui::SetCursorPosY(cursorPosBegin - kDropTargetHeight);

	// Draw invisible button (just a placeholder) to accept drag and drop payloads
	ImGui::InvisibleButton(kDropTargetLabel, ImVec2(ImGui::GetContentRegionMax().x, kDropTargetHeight));
	if (ImGui::BeginDragDropTarget()) {
		// Draw line to indicate where entity will be inserted
		ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 1.5 * kDropTargetHeight); // IDK why this math works
		ImGui::Separator();

		// Accept the payload if it's an entity
		const ImGuiPayload* payload = ImGui::AcceptDragDropPayload(kEntityPayloadID);
		if (payload) {
			int sourceIndex = *(const int*)payload->Data;
			auto entity = entityList[sourceIndex];

			// Ensure that the sourceIndex is valid
			if (sourceIndex < 0 || sourceIndex >= entityList.size() || sourceIndex == targetIndex) return;

			// If target points to a real & separate entity, insert source entity at target index
			if (targetIndex >= 0 && targetIndex < entityList.size()) {
				auto it = entityList.begin() + targetIndex;
				entityList.insert(it, entity);
				if (targetIndex < sourceIndex) sourceIndex++;
				entityList.erase(entityList.begin() + sourceIndex);
				// Otherwise, must be end of the list (targetIndex == -1), so just move there
				EntityManager::GetInstance().sortEntitiesForRendering(); //Sorting our Render List
			}
			else {
				;
				entityList.erase(entityList.begin() + sourceIndex);
				entityList.push_back(entity);
				EntityManager::GetInstance().sortEntitiesForRendering(); //Sorting our Render List
			}

			// Once complete, make this the active entity
			Editor::active_entity_ = entity;
		}
		ImGui::EndDragDropTarget();
	}
	// Reset cursor to the top of selectable-to-be-drawn
	ImGui::SetCursorPosY(cursorPosBegin);
}
