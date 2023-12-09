#pragma once

#include "AmberEngine/API/Export.h"

namespace AmberEngine::Data
{
	struct API_AMBERENGINE EditorConstants
	{
		EditorConstants() = delete;

		static const std::string ENGINE_ASSETS_PATH;
		static const std::string EDITOR_ASSETS_PATH;
		static const std::string PROJECT_ASSETS_PATH;
		static const std::string EDITOR_FONT_PATH;

		static const std::string EDITOR_PANEL_SCENE_VIEW_TITLE;
		static const std::string EDITOR_PANEL_HIERARCHY_TITLE;
		static const std::string EDITOR_PANEL_MENU_BAR_TITLE;
		static const std::string EDITOR_PANEL_INSPECTOR_TITLE;
		static const std::string EDITOR_PANEL_FRAME_INFO_TITLE;

		static const float EDITOR_FONT_SIZE_SMALL;
		static const float EDITOR_FONT_SIZE_MEDIUM;
		static const float EDITOR_FONT_SIZE_BIG;

		static const char BACKSLASH;
		static const char FORWARDSLASH;
		static const char DOT;
	};
}