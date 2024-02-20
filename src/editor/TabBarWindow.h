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
	// List of textures for the buttons in the tab bar. Duplicated from AssetManager.
	std::vector<sf::Texture> icons;
};

#endif // TABBARWINDOW_H
