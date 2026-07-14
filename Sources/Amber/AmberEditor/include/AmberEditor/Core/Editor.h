#pragma once


#include "AmberEditor/Core/Context.h"
#include "AmberEditor/Core/EditorRenderer.h"
#include "AmberEditor/Core/PanelsManager.h"
#include "AmberEditor/Core/EditorAction.h"

#include "AmberUI/Canvas.h"

namespace AmberEditor::Core
{
	class Editor final
	{
	public:
		Editor(Context& p_context);
		~Editor();

		Editor(const Editor& other)            = delete;
		Editor(Editor&& other)                 = delete;
		Editor& operator=(const Editor& other) = delete;
		Editor& operator=(Editor&& other)      = delete;

		void PreUpdate();
		void Update(float p_deltaTime);
		void PrepareRendering(float p_deltaTime);
		void RenderEditorUI(float p_deltaTime);
		void UpdateEditorPanels(float p_deltaTime);
		void PostUpdate();
		void UpdateCurrentEditorMode(float p_deltaTime);
		void RenderViews(float p_deltaTime);
		void PostRenderUI() const;
		void InitializeUI();
		void UpdatePlayMode(float p_deltaTime);
		void UpdateEditMode(float p_deltaTime);
		EditorRenderer& GetRenderer();

	public:
		Context& m_context;
		EditorRenderer m_editorRenderer;
		PanelsManager m_panelsManager;
		Core::EditorAction	m_editorActions;
		AmberUI::Canvas m_canvas;

		uint64_t m_elapsedFrames = 0;
	};
}
