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
#define WINDOW_WIDTH(MV)				MV->Size.x
#define WINDOW_HEIGHT(MV)				MV->Size.y
#define FONT_SIZE						ImGui::GetFontSize()

// =========================
//  ImGui Window Dimensions
// =========================

// FileBarWindow
static constexpr short FILE_HEIGHT		= 19; // ~19px tall by default in ImGui

// TabBarWindow
#define TAB_WIDTH(MV)					WINDOW_WIDTH(MV)
#define TAB_HEIGHT(MV)					WINDOW_HEIGHT(MV) * 0.20 
static constexpr short TAB_XOFFSET		= 0;
#define TAB_YOFFSET						FILE_HEIGHT // Under the file bar

// SceneLayoutWindow
#define SCENE_WIDTH(MV)					WINDOW_WIDTH(MV) * 0.80
#define SCENE_HEIGHT(MV)				WINDOW_HEIGHT(MV) * 0.80 - FILE_HEIGHT
static constexpr short SCENE_XOFFSET	= 0;
#define SCENE_YOFFSET(MV)				TAB_HEIGHT(MV) + FILE_HEIGHT // Under tab bar

// ExplorerWindow
#define EXPLR_WIDTH(MV)					WINDOW_WIDTH(MV) * 0.20
#define EXPLR_HEIGHT(MV)				WINDOW_HEIGHT(MV) * 0.40
#define EXPLR_XOFFSET(MV)				WINDOW_WIDTH(MV) - EXPLR_WIDTH(MV) // Right side of the window
#define EXPLR_YOFFSET(MV)				TAB_HEIGHT(MV) + FILE_HEIGHT  // Under tab bar

// PropertyWindow
#define PROP_WIDTH(MV)					WINDOW_WIDTH(MV) * 0.20 
#define PROP_HEIGHT(MV)					WINDOW_HEIGHT(MV) * 0.40 - FILE_HEIGHT 
#define PROP_XOFFSET(MV)				WINDOW_WIDTH(MV) - PROP_WIDTH(MV) // Right side of the window
#define PROP_YOFFSET(MV)				EXPLR_HEIGHT(MV) + TAB_HEIGHT(MV) + FILE_HEIGHT // Under explorer

// =========================
//		Misc Dimensions
// =========================

// TabBarWindow
static constexpr short TAB_TOP_HEIGHT	= 19;
#define TAB_BUTTON_SIZE(MV)				WINDOW_HEIGHT(MV) * 0.075
#define TAB_ROW_YOFFSET(MV)				(TAB_HEIGHT(MV) - TAB_BUTTON_SIZE(MV)) / 2 + TAB_TOP_HEIGHT - FONT_SIZE
