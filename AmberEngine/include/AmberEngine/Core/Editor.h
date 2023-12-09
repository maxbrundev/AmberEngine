#pragma once


#include "AmberEngine/API/Export.h"


#include "AmberEngine/Core/EditorRenderer.h"
#include "AmberEngine/Core/PanelsManager.h"
#include "AmberEngine/UI/Canvas.h"
#include "AmberEngine/Core/Context.h"

namespace AmberEngine::Core
{
	class API_AMBERENGINE Editor final
	{
	public:
		Editor(Context& p_context);
		~Editor();

		Editor(const Editor& other)            = delete;
		Editor(Editor&& other)                 = delete;
		Editor& operator=(const Editor& other) = delete;
		Editor& operator=(Editor&& other)      = delete;

		void PreUpdate() const;
		void Update(float p_deltaTime);
		void PrepareRendering(float p_deltaTime);
		void RenderEditorUI(float p_deltaTime);
		void UpdateEditorPanels(float p_deltaTime);
		void PostUpdate() const;
	
		void RenderViews(float p_deltaTime);
		void PostRenderUI() const;
		void InitializeUI();

		EditorRenderer& GetRenderer();

	public:
		AmberEngine::UI::Canvas m_canvas;
		Context& m_context;
		EditorRenderer m_editorRenderer;
		PanelsManager m_panelsManager;
	};
}
