#pragma once
#include "AmberUI/Panels/APanelWindow.h"
#include "AmberCore/Resources/Material.h"
#include "AmberUI/Widgets/Column.h"
#include "AmberUI/Widgets/Group.h"
#include "AmberUI/Widgets/Text.h"

namespace AmberEditor::Panels
{
	class MaterialEditor : public AmberUI::Panels::APanelWindow
	{
	public:
		/**
		* Constructor
		* @param p_title
		* @param p_opened
		* @param p_windowSettings
		*/
		MaterialEditor
		(
			const std::string& p_title,
			bool p_opened,
			const AmberUI::Panels::PanelSettings& p_windowSettings
		);

		/**
		* Refresh the material editor
		*/
		void Refresh();

		/**
		* Defines the target material of the material editor
		* @param p_newTarget
		*/
		void SetTarget(AmberCore::Resources::Material& p_newTarget);

		/**
		* Returns the target of the material editor
		*/
		AmberCore::Resources::Material* GetTarget() const;

		/**
		* Remove the target of the material editor (Clear the material editor)
		*/
		void RemoveTarget();

		/**
		* Launch the preview of the currently targeted material
		*/
		void Preview();

		/**
		* Reset material
		*/
		void Reset();

	private:
		void OnMaterialDropped();
		void OnShaderDropped();

		void CreateHeaderButtons();
		void CreateMaterialSelector();
		void CreateShaderSelector();
		void CreateMaterialSettings();
		void CreateShaderSettings();

		void GenerateShaderSettingsContent();
		void GenerateMaterialSettingsContent();

	private:
		AmberCore::Resources::Material* m_target = nullptr;
		AmberRendering::Resources::Shader* m_shader = nullptr;

		AmberUI::Widgets::Text* m_targetMaterialText = nullptr;
		AmberUI::Widgets::Text* m_shaderText = nullptr;

		AmberTools::Eventing::Event<> m_materialDroppedEvent;
		AmberTools::Eventing::Event<> m_shaderDroppedEvent;

		AmberUI::Widgets::Group* m_settings = nullptr;
		AmberUI::Widgets::Group* m_materialSettings = nullptr;
		AmberUI::Widgets::Group* m_shaderSettings = nullptr;

		AmberUI::Widgets::Columns<2>* m_shaderSettingsColumns = nullptr;
		AmberUI::Widgets::Columns<2>* m_materialSettingsColumns = nullptr;
	};
}
