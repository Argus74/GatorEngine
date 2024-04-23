/*
	File: Editor.h
	The Editor class is "high-level" and manages the UI of Gator Engine's game editor.
	Conceptually, the editor is just a bunch of UI overlay that the user interacts with
	to create and modify their game objects, components, and whatnot.
	This is why the Editor class contains a list of UIWindows, which are the individual
	window panes of the editor. These UIWindows include the Property window, Explorer window,
	Sprite tab, Physics tab, etc. When the Editor is drawn, it simply calls the Draw() function
	of each UIWindow in its list. Nothing crazy! Editor is a pretty simple class.
	In addition to drawing windows, Editor is also responsible for setting up the global
	ImGui style options, which can be overridden by individual UIWindows using the ImGui
	style stack (PushStyleVar() and PopStyleVar()).
*/

#ifndef EDITOR_H
#define EDITOR_H

#include <vector>

#include "imgui-SFML.h"
#include "imgui.h"

#include "UIWindow.h"

class Editor {
 public:
    Editor();
    ~Editor() = default;
    Editor(Editor& other) = delete;
    Editor& operator=(const Editor&) = delete;

    // Draw all UIWindows of the editor
    void Draw();

    // State of the editor
    static enum State {
        Selecting,
        Moving,
        Resizing,
        Testing,
        None
    } state;

    // Active entity to modify.
    static std::shared_ptr<Entity> active_entity_;

    // Grid properties
    static bool show_grid_;
    static bool snap_to_grid_;
    static int grid_size_;

 private:
    // List of all windows in the Editor
    std::vector<std::unique_ptr<UIWindow>> windows_;
};

#endif  // EDITOR_H
