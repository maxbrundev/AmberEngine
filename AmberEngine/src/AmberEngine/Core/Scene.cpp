#include "Amberpch.h"

#include "AmberEngine/Core/Scene.h"


AmberEngine::Core::Scene::Scene(Renderer& p_renderer) : m_renderer(p_renderer)
{
}

AmberEngine::Core::Scene::~Scene()
{
}

void AmberEngine::Core::Scene::Render()
{

	if (m_models.empty())
		return;

	for (auto& gameObject : m_models)
	{
		//m_renderer.DrawModelWithShader(*gameObject.second, *gameObject.second->GetShader());
	}
}

void AmberEngine::Core::Scene::AddModel(Resources::AssimpModel* p_gameObject, const std::string& p_name)
{
	try
	{
		m_models.try_emplace(p_name, p_gameObject);
	}
	catch (...)
	{
		std::cerr << p_name + " is already added to Scene\n";
	}
}
