#pragma once

#include "AmberEngine/API/Export.h"
#include "AmberEngine/Core/Renderer.h"

#include "AmberEngine/Resources/AssimpModel.h"
namespace AmberEngine::Core
{
	class API_AMBERENGINE Scene
	{
	public:
		Scene(Renderer& p_renderer);
		~Scene();
		void AddModel(Resources::AssimpModel* p_gameObject, const std::string& p_name);
		void Render();
	private:
		Renderer& m_renderer;
		std::unordered_map<std::string_view, Resources::AssimpModel*> m_models;
	};
}
