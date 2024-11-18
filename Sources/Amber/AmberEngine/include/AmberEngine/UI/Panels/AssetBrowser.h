#pragma once

#include "AmberEngine/UI/Panels/APanelWindow.h"
#include "AmberEngine/UI/Widgets/Group.h"
#include "AmberEngine/UI/Widgets/TreeNode.h"

namespace AmberEngine::UI::Panels
{
	class AssetBrowser : public APanelWindow
	{
	public:
		AssetBrowser
		(
			const std::string& p_title,
			bool p_opened,
			const PanelSettings& p_windowSettings,
			const std::string& p_engineAssetFolder = "",
			const std::string& p_projectAssetFolder = "",
			const std::string& p_projectScriptFolder = ""
		);
		virtual ~AssetBrowser() override = default;


		void Fill();

		void Clear();
		
		void Refresh();
		
		static const std::string __FILENAMES_CHARS;
	
		void ParseFolder(AmberEngine::UI::Widgets::TreeNode& p_root, const std::filesystem::directory_entry& p_directory, bool p_isEngineItem, bool p_scriptFolder = false);
		void ConsiderItem(AmberEngine::UI::Widgets::TreeNode* p_root, const std::filesystem::directory_entry& p_entry, bool p_isEngineItem, bool p_autoOpen = false, bool p_scriptFolder = false);
		std::unordered_map<AmberEngine::UI::Widgets::TreeNode*, std::string> m_pathUpdate;
	private:
		std::string m_engineAssetFolder;
		std::string m_projectAssetFolder;
		std::string m_projectScriptFolder;
		Widgets::Group* m_assetList;
	
	};
}
