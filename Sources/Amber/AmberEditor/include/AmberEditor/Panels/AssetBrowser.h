#pragma once

#include "AmberUI/Panels/APanelWindow.h"
#include "AmberUI/Widgets/Group.h"
#include "AmberUI/Widgets/TreeNode.h"

namespace AmberEditor::Panels
{
	class AssetBrowser : public AmberUI::Panels::APanelWindow
	{
	public:
		AssetBrowser
		(
			const std::string& p_title,
			bool p_opened,
			const AmberUI::Panels::PanelSettings& p_windowSettings,
			const std::string& p_engineAssetFolder = "",
			const std::string& p_projectAssetFolder = "",
			const std::string& p_projectScriptFolder = ""
		);
		virtual ~AssetBrowser() override = default;


		void Fill();

		void Clear();
		
		void Refresh();
		
		static const std::string __FILENAMES_CHARS;
	
		void ParseFolder(AmberUI::Widgets::TreeNode& p_root, const std::filesystem::directory_entry& p_directory, bool p_isEngineItem, bool p_scriptFolder = false);
		void ConsiderItem(AmberUI::Widgets::TreeNode* p_root, const std::filesystem::directory_entry& p_entry, bool p_isEngineItem, bool p_autoOpen = false, bool p_scriptFolder = false);
		std::unordered_map<AmberUI::Widgets::TreeNode*, std::string> m_pathUpdate;
	private:
		std::string m_engineAssetFolder;
		std::string m_projectAssetFolder;
		std::string m_projectScriptFolder;
		AmberUI::Widgets::Group* m_assetList;
	
	};
}
