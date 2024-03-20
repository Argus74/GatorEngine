#ifndef SCENELAYOUTWINDOW_H
#define SCENELAYOUTWINDOW_H

#include "UIWindow.h"
#include "Editor.h"

#include "../EntityManager.h"

class SceneLayoutWindow : public UIWindow {
 public:
	SceneLayoutWindow();

	void SetPosition() override;
	void PreDraw() override;
	void DrawFrames() override;
	void PostDraw() override;

 private:
	// Helper to determine an entity's selection box position and size
	std::vector<float> GetSelectionBoxDimensions(const std::shared_ptr<Entity> entity);
};

#endif // SCENELAYOUTWINDOW_H