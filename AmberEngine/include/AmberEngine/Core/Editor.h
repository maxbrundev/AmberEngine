#pragma once

#include "AmberEngine/API/Export.h"

#include "AmberEngine/Core/Context.h"

#include "AmberEngine/UI/Views/SceneView.h"
#include "AmberEngine/UI/MenuBar.h"

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
		void RenderViews(float p_deltaTime);
		void UpdateInput();
		
	public:
		Context& m_context;

		UI::SceneView m_sceneView;
		UI::MenuBar m_menuBar;
	};
}
