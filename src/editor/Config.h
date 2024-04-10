/*
* File: Config.h
* 
* This is a header file that contains common configuration settings for the editor.
* Mainly, the sizes and positions of the windows, since they're all very coupled and
* it'd be nice to see them all in one place.
* Feel free to move more Editor-related things here if you think they should be here.
*/

#pragma once

// =========================
//        Helpers
// =========================

// NOTE: MV == ImGui::GetMainViewport()
#define WINDOW_WIDTH(MV)	MV->Size.x
#define WINDOW_HEIGHT(MV)	MV->Size.y

// =========================
//  ImGui Window Dimensions
// =========================

// FileBarWindow
#define FILE_HEIGHT			19 // ~19px tall by default in ImGui

// TabBarWindow
#define TAB_WIDTH(MV)		WINDOW_WIDTH(MV)
#define TAB_HEIGHT(MV)		WINDOW_HEIGHT(MV) * 0.20 
#define TAB_XOFFSET			0
#define TAB_YOFFSET			FILE_HEIGHT // Under the file bar

#define TAB_BUTTON_SIZE(MV)	WINDOW_HEIGHT(MV) * 0.075

// SceneLayoutWindow
#define SCENE_WIDTH(MV)		WINDOW_WIDTH(MV) * 0.80
#define SCENE_HEIGHT(MV)	WINDOW_HEIGHT(MV) * 0.80 - FILE_HEIGHT
#define SCENE_XOFFSET		0
#define SCENE_YOFFSET(MV)	TAB_HEIGHT(MV) + FILE_HEIGHT // Under tab bar

// ExplorerWindow
#define EXPLR_WIDTH(MV)		WINDOW_WIDTH(MV) * 0.20
#define EXPLR_HEIGHT(MV)	WINDOW_HEIGHT(MV) * 0.40
#define EXPLR_XOFFSET(MV)	WINDOW_WIDTH(MV) - EXPLR_WIDTH(MV) // Right side of the window
#define EXPLR_YOFFSET(MV)	TAB_HEIGHT(MV) + FILE_HEIGHT  // Under tab bar

// PropertyWindow
#define PROP_WIDTH(MV)		WINDOW_WIDTH(MV) * 0.20 
#define PROP_HEIGHT(MV)		WINDOW_HEIGHT(MV) * 0.40 - FILE_HEIGHT 
#define PROP_XOFFSET(MV)	WINDOW_WIDTH(MV) - PROP_WIDTH(MV) // Right side of the window
#define PROP_YOFFSET(MV)	EXPLR_HEIGHT(MV) + TAB_HEIGHT(MV) + FILE_HEIGHT // Under explorer


