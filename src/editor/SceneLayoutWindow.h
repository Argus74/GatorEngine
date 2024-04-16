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
};

#endif // SCENELAYOUTWINDOW_H