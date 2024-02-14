#include "UIWindow.h"

UIWindow::UIWindow() {
	name_ = "Name me!";
	window_flags_ = 0;

	// TODO: Elevate this to higher-level class once we have one
	ImGuiStyle& style = ImGui::GetStyle();
	style.WindowBorderSize = 0.0f;
	style.FrameBorderSize = 1.0f;
	style.ScrollbarRounding = 2.0f;
	style.GrabRounding = 2.0f;
	ImGui::StyleColorsLight();
	style.Colors[ImGuiCol_WindowBg] = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
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
