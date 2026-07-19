#pragma once

#include <variant>

#include "AmberRendering/Resources/Model.h"
#include "AmberRendering/Resources/Texture.h"
#include "AmberTools/FileSystem/IniFile.h"
#include "AmberUI/Panels/APanelWindow.h"
#include "AmberUI/Widgets/Button.h"
#include "AmberUI/Widgets/Column.h"
#include "AmberUI/Widgets/Group.h"
#include "AmberUI/Widgets/Text.h"

namespace AmberEditor::Panels
{
	class AssetProperties : public AmberUI::Panels::APanelWindow
	{
	public:
		using EditableAssets = std::variant<AmberRendering::Resources::Model*, AmberRendering::Resources::Texture*>;

		AssetProperties
		(
			const std::string& p_title,
			bool p_opened,
			const AmberUI::Panels::PanelSettings& p_windowSettings
		);

		void SetTarget(const std::string& p_path);

		void Refresh();

		void Preview();

	private:
		void CreateHeaderButtons();
		void CreateAssetSelector();
		void CreateSettings();
		void CreateInfo();
		void CreateModelSettings();
		void CreateTextureSettings();
		void Apply();

	private:
		std::string m_resource;

		AmberTools::Eventing::Event<> m_targetChanged;
		AmberUI::Widgets::Group* m_settings = nullptr;
		AmberUI::Widgets::Group* m_info = nullptr;
		AmberUI::Widgets::Button* m_applyButton = nullptr;
		AmberUI::Widgets::Button* m_revertButton = nullptr;
		AmberUI::Widgets::Button* m_previewButton = nullptr;
		AmberUI::Widgets::Button* m_resetButton = nullptr;
		AmberUI::Widgets::AWidget* m_headerSeparator = nullptr;
		AmberUI::Widgets::AWidget* m_headerLineBreak = nullptr;
		AmberUI::Widgets::Columns<2>* m_settingsColumns = nullptr;
		AmberUI::Widgets::Columns<2>* m_infoColumns = nullptr;
		AmberUI::Widgets::Text* m_assetSelector = nullptr;
		std::unique_ptr<AmberTools::FileSystem::IniFile> m_metadata;
	};
}
