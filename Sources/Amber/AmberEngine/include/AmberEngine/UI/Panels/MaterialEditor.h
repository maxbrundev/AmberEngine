#pragma once
#include "APanelWindow.h"
#include "AmberEngine/Resources/Material.h"
#include "AmberEngine/UI/Widgets/Column.h"
#include "AmberEngine/UI/Widgets/Group.h"
#include "AmberEngine/UI/Widgets/Text.h"

namespace AmberEngine::UI::Panels
{
	class MaterialEditor : public APanelWindow
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
			const PanelSettings& p_windowSettings
		);

		/**
		* Refresh the material editor
		*/
		void Refresh();

		/**
		* Defines the target material of the material editor
		* @param p_newTarget
		*/
		void SetTarget(Resources::Material& p_newTarget);

		/**
		* Returns the target of the material editor
		*/
		Resources::Material* GetTarget() const;

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
		Resources::Material* m_target = nullptr;
		Resources::Shader* m_shader = nullptr;

		Widgets::Text* m_targetMaterialText = nullptr;
		Widgets::Text* m_shaderText = nullptr;

		Eventing::Event<> m_materialDroppedEvent;
		Eventing::Event<> m_shaderDroppedEvent;

		Widgets::Group* m_settings = nullptr;
		Widgets::Group* m_materialSettings = nullptr;
		Widgets::Group* m_shaderSettings = nullptr;

		Widgets::Columns<2>* m_shaderSettingsColumns = nullptr;
		Widgets::Columns<2>* m_materialSettingsColumns = nullptr;
	};
}
