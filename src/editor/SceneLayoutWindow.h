#ifndef SCENELAYOUTWINDOW_H
#define SCENELAYOUTWINDOW_H

#include "../EntityManager.h"
#include "Editor.h"
#include "UIWindow.h"

class SceneLayoutWindow : public UIWindow {
 public:
    SceneLayoutWindow();

    void SetPosition() override;
    void PreDraw() override;
    void DrawFrames() override;
    void PostDraw() override;
};

#endif  // SCENELAYOUTWINDOW_H