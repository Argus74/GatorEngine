#include "UIWindow.h"

std::shared_ptr<Entity> UIWindow::active_entity_ = nullptr;

UIWindow::UIWindow() {
	name_ = "Name me!";

	// Disallow moving windows by default
	window_flags_ = 0;
	window_flags_ |= ImGuiWindowFlags_NoCollapse;
	window_flags_ |= ImGuiWindowFlags_NoMove;
	window_flags_ |= ImGuiWindowFlags_NoResize;
}

void UIWindow::Draw() {
	SetPosition();
	PreDraw();
	ImGui::Begin(name_.c_str(), NULL, window_flags_);
	DrawFrames();
	ImGui::End();
	PostDraw();
}

void UIWindow::PreDraw() {
	// No default implementation; override if needed
	// I didn't want to make it pure virtual because it's not always needed,
	// so why force y'all to implement it?
}

void UIWindow::PostDraw() {
	// Ditto
}
