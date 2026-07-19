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
		MaterialEditor
		(
			const std::string& p_title,
			bool p_opened,
			const AmberUI::Panels::PanelSettings& p_windowSettings
		);

		void Refresh();

		void SetTarget(AmberCore::Resources::Material& p_newTarget);

		AmberCore::Resources::Material* GetTarget() const;

		void RemoveTarget();

		void Preview();

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
