#pragma once

#include "Amberpch.h"

#include "AmberEngine/API/Export.h"

namespace AmberEngine::Data
{
	struct API_AMBERENGINE Constants
	{
		Constants() = delete;

		static const std::string ENGINE_ASSETS_PATH;
		static const std::string EDITOR_ASSETS_PATH;
		static const std::string PROJECT_ASSETS_PATH;
		static const std::string EDITOR_FONT_PATH;

		static const std::string EDITOR_PANEL_MENU_BAR_FILE_NAME;
		static const std::string EDITOR_PANEL_MENU_BAR_DRAW_MODE_NAME;

		static const std::string EDITOR_PANEL_MENU_ITEM_EXIT_NAME;
		static const std::string EDITOR_PANEL_MENU_ITEM_SHADED_NAME;
		static const std::string EDITOR_PANEL_MENU_ITEM_WIREFRAME_NAME;
		static const std::string EDITOR_PANEL_MENU_ITEM_NORMALS_NAME;

		static const std::string EDITOR_PANEL_SCENE_NAME;
		static const std::string EDITOR_PANEL_HIERARCHY_NAME;

		static const std::string EDITOR_TREE_NODE_HIERARCHY_LABEL_ROOT;

		static const float EDITOR_FONT_SIZE_SMALL;
		static const float EDITOR_FONT_SIZE_MEDIUM;
		static const float EDITOR_FONT_SIZE_BIG;

		static const char BACKSLASH;
		static const char FORWARDSLASH;
		static const char DOT;
	};
}