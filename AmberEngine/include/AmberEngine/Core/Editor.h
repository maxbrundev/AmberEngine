#pragma once

#include "AmberEngine/API/Export.h"

#include "AmberEngine/Core/Context.h"
#include "AmberEngine/Core/EditorRenderer.h"

#include "AmberEngine/UI/Canvas.h"

namespace AmberEngine::Core
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
		void PrepareRendering(float p_deltaTime);
		void RenderEditorUI(float p_deltaTime);
		void PostUpdate() const;
	
		void RenderViews(float p_deltaTime) const;
		void PostRenderUI() const;
		void InitializeUI();

		EditorRenderer& GetRenderer();

	public:
		Context& m_context;

		EditorRenderer	m_editorRenderer;
		UI::Canvas m_canvas;

		std::unique_ptr<PanelsManager> m_panelsManager;
	};
}
