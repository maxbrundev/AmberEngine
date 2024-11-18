#pragma once

#include <variant>

#include "AmberEngine/Resources/Model.h"
#include "AmberEngine/Tools/FileSystem/IniFile.h"
#include "AmberEngine/UI/Panels/APanelWindow.h"
#include "AmberEngine/UI/Widgets/Button.h"
#include "AmberEngine/UI/Widgets/Column.h"
#include "AmberEngine/UI/Widgets/Group.h"
#include "AmberEngine/UI/Widgets/Text.h"

namespace AmberEngine::UI::Panels
{
	class AssetProperties : public AmberEngine::UI::Panels::APanelWindow
	{
	public:
		using EditableAssets = std::variant<Resources::Model*, Resources::Texture*>;

		AssetProperties
		(
			const std::string& p_title,
			bool p_opened,
			const PanelSettings& p_windowSettings
		);

		/**
		* Defines the target of the asset settings editor
		* @param p_path
		*/
		void SetTarget(const std::string& p_path);

		/**
		* Refresh the panel to show the current target settings
		*/
		void Refresh();

		/**
		* Launch the preview of the target asset
		*/
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

		Eventing::Event<> m_targetChanged;
		Widgets::Group* m_settings = nullptr;
		Widgets::Group* m_info = nullptr;
		Widgets::Button* m_applyButton = nullptr;
		Widgets::Button* m_revertButton = nullptr;
		Widgets::Button* m_previewButton = nullptr;
		Widgets::Button* m_resetButton = nullptr;
		Widgets::AWidget* m_headerSeparator = nullptr;
		Widgets::AWidget* m_headerLineBreak = nullptr;
		Widgets::Columns<2>* m_settingsColumns = nullptr;
		Widgets::Columns<2>* m_infoColumns = nullptr;
		Widgets::Text* m_assetSelector = nullptr;
		std::unique_ptr<Tools::Filesystem::IniFile> m_metadata;
	};
}
