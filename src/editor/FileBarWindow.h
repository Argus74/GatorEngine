#ifndef FILEBARWINDOW_H
#define FILEBARWINDOW_H

#include "UIWindow.h"

class FileBarWindow : public UIWindow {
public:
	FileBarWindow();
	~FileBarWindow() = default;
	FileBarWindow(const FileBarWindow&) = delete;
	FileBarWindow& operator=(const FileBarWindow&) = delete;

	void SetPosition() override;
	void DrawFrames() override;
};

#endif // FILEBARWINDOW_H
