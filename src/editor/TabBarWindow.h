#ifndef TABBARWINDOW_H
#define TABBARWINDOW_H

#include "SFML/Graphics.hpp"

#include <vector>

#include "UIWindow.h"

class TabBarWindow : public UIWindow {
public:
	TabBarWindow();
	~TabBarWindow() = default;
	TabBarWindow(const TabBarWindow&) = delete;
	TabBarWindow& operator=(const TabBarWindow&) = delete;

	// UIWindow interface
	void SetPosition() override;
	void DrawFrames() override;

private:
	// To track all available sprites to be selected
	std::vector<const char*> spriteNameList_;

	// To track currently selected sprite for sprite button
	int selectedSpriteIndex_ = 3; // Default to "Grass Tile"

	// Helper to draw a button somewhere in a horizontal grid of squares
	void DrawButton(const char* name, sf::Texture& texture, int index, 
		std::function<void()> onClick, bool highlighted = false);
};

#endif // TABBARWINDOW_H
