#pragma once

#include "AmberEngine/API/Export.h"

#include "AmberEngine/Core/Context.h"

#include "AmberEngine/UI/Panels/Views/SceneView.h"
#include "AmberEngine/UI/Panels/Hierarchy.h"
#include "AmberEngine/UI/Panels/MenuBar.h"

namespace AmberEngine::Core
{
	class API_AMBERENGINE Editor
	{
	public:
		Editor(Context& p_context);
		~Editor() = default;

		void PreUpdate() const;
		void Update(float p_deltaTime);
		void PostUpdate() const;
		void UpdateLights(SceneSystem::Scene& p_scene) const;
		void RenderViews(float p_deltaTime);
		void RenderScene();
		void HandleInput() const;
		void InitMaterials();
		void InitializeUI();
		void OnNormalsColorsVisualization(bool p_value);
		void ToggleNormalVisualization() const;

	public:
		Context& m_context;
		
		std::unique_ptr<UI::SceneView> m_sceneView;
		UI::Hierarchy m_hierarchy;
		UI::MenuBar m_menuBar;

		Resources::Material m_defaultMaterial;

		bool isNormalsColorsVisualization = false;
	};
}
