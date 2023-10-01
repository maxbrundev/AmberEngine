#pragma once

#include "AmberEngine/API/Export.h"

#include "AmberEngine/Core/Context.h"

#include "AmberEngine/UI/Canvas.h"

namespace AmberEngine::Managers
{
	class PanelsManager;
}

namespace AmberEngine::Core
{
	class API_AMBERENGINE Editor
	{
	public:
		Editor(Context& p_context);
		~Editor();

		void PreUpdate() const;
		void Update(float p_deltaTime);
		void PostUpdate() const;
		void UpdateLights(SceneSystem::Scene& p_scene) const;
		void RenderViews(float p_deltaTime) const;
		void RenderScene();
		void RenderUI() const;
		void HandleInput() const;
		void InitMaterials();
		void InitializeUI();
		void OnNormalsColorsVisualization(bool p_value);
		void ToggleNormalVisualization() const;

	public:
		Context& m_context;

		UI::Canvas m_canvas;

		std::unique_ptr<Managers::PanelsManager> m_panelsManager;

		Resources::Material m_defaultMaterial;

		//TODO: Remove.
		bool isNormalsColorsVisualization = false;
	};
}
