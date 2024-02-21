#ifndef EXPLORERWINDOW_H
#define EXPLORERWINDOW_H

#include <SFML/Graphics.hpp>

#include "UIWindow.h"

class ExplorerWindow : public UIWindow {
public:
	ExplorerWindow();
	ExplorerWindow(ExplorerWindow const&) = delete;
	ExplorerWindow& operator=(ExplorerWindow const&) = delete;

	// UIWindow virtual function implementations
	void SetPosition() override;
	void PreDraw() override;
	void DrawFrames() override;
	void PostDraw() override;

private:
	// Icon to appear next to entity name
	sf::Texture icon_;
};

#endif // EXPLORERWINDOW_H
