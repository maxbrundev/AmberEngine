#include "Amberpch.h"

#include "AmberEngine/Core/ECS/Components/CModelRenderer.h"

#include "AmberEngine/Managers/ResourcesManager.h"
//#include "AmberEngine/Tools/Utils/String.h"

AmberEngine::Core::ECS::Components::CModelRenderer::CModelRenderer(Actor& p_owner, std::string_view p_name, const std::string& p_filePath) : AComponent(p_owner),
	m_model(&Managers::ResourcesManager::Instance().LoadModel(p_name, p_filePath)), m_name(p_name)
{
	//std::string name = Utils::String::RemoveExtensionFromFileName(Utils::String::ExtractFileNameFromPath(p_filePath));
}

AmberEngine::Core::ECS::Components::CModelRenderer::~CModelRenderer()
{
	if(m_model != nullptr)
	{
		m_model = nullptr;
	}

	// Require Model to be shared pointer in order to unload the resource if the reference count is <= 1
	//Managers::ResourcesManager::Instance().RemoveModel(m_name);
}

AmberEngine::Resources::Model* AmberEngine::Core::ECS::Components::CModelRenderer::GetModel() const
{
	return m_model;
}

std::vector<AmberEngine::Resources::Mesh*>& AmberEngine::Core::ECS::Components::CModelRenderer::GetMeshes() const
{
	return m_model->GetMeshes();
}

std::string AmberEngine::Core::ECS::Components::CModelRenderer::GetName()
{
	return "Model";
}

void AmberEngine::Core::ECS::Components::CModelRenderer::OnInspector(UI::WidgetContainer& p_root)
{

}
