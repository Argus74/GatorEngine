/*
	File: UIWindow.h
	Base class for all windows comprising the app's Editor. Provides a common interface
	for these windows to be drawn. Also allows for polymorphic storage of windows.
	Pure virtual functions are provided for necessary customizations of each window,
	such as positioning and drawing. Non-pure virtual functions are provided for
	optional customizations, such as any extra setup or cleanup required for drawing.
	Some default, global ImGui customization options are also set here. This can help
	ensure a consistent look and feel across all windows. However, these options can
	be overridden by derived classes.
*/

#ifndef UIWINDOW_H
#define UIWINDOW_H

#include <string>

#include "imgui.h"

#include "../Entity.h"

class UIWindow {
 public:
    UIWindow();
    ~UIWindow() = default;
    UIWindow(const UIWindow&) = delete;
    UIWindow& operator=(const UIWindow&) = delete;

    // Main draw call for UIWindows. Calls all virtual functions in correct order.
    virtual void Draw() final;

 protected:
    // Set the UIWindow's position and size. Used for responsive app resizing.
    virtual void SetPosition() = 0;

    // Perform any extra setup wanted (positioning, styling) before DrawFrames()
    virtual void PreDraw();

    // Draw the frames (aka, the inside content) of the UIWindow.
    virtual void DrawFrames() = 0;

    // Perform any cleanup necessary after DrawFrames()
    virtual void PostDraw();

    // The name of the UIWindow. Used for the title bar.
    std::string name_;

    // ImGui options for the UIWindow.
    ImGuiWindowFlags window_flags_;
};

#endif  // UIWINDOW_H
