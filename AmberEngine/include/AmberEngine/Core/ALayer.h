#pragma once

#include "AmberEngine/API/Export.h"

#include "AmberEngine/Core/Context.h"

#include "AmberEngine/Core/Editor.h"

namespace AmberEngine::Core
{
	class API_AMBERENGINE ALayer
	{
	public:
		inline explicit ALayer(Context& p_context, Editor& p_editor, std::string_view p_layerID) : m_context(p_context), m_editor(p_editor), m_id(p_layerID) {}
		virtual ~ALayer() = default;

		virtual void OnStart() = 0;
		virtual void OnUpdate(float p_deltaTime) = 0;
		virtual void OnDestroy() = 0;

		inline const std::string_view GetID() const { return m_id; }
	
	protected:
		Context& m_context;
		Editor& m_editor;
		const std::string_view m_id;
	};
}
