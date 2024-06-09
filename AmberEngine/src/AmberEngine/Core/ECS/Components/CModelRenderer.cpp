#include "Amberpch.h"

#include "AmberEngine/Core/ECS/Components/CModelRenderer.h"

#include "AmberEngine/Core/ECS/Actor.h"
#include "AmberEngine/Core/ECS/Components/CMaterialRenderer.h"
#include "AmberEngine/Managers/ModelManager.h"
#include "AmberEngine/Tools/Global/ServiceLocator.h"
#include "AmberEngine/Tools/Utils/PathParser.h"

#include "AmberEngine/UI/GUIDrawer.h"


AmberEngine::Core::ECS::Components::CModelRenderer::CModelRenderer(Actor& p_owner) : AComponent(p_owner)
{
	m_modelChangedEvent += [this]
	{
		if (const auto materialRenderer = owner.GetComponent<CMaterialRenderer>())
			materialRenderer->UpdateMaterialList();
	};
}

void AmberEngine::Core::ECS::Components::CModelRenderer::SetModel(Resources::Model* p_model)
{
	m_model = p_model;
	m_modelChangedEvent.Invoke();
}

AmberEngine::Resources::Model* AmberEngine::Core::ECS::Components::CModelRenderer::GetModel() const
{
	return m_model;
}

std::string AmberEngine::Core::ECS::Components::CModelRenderer::GetName()
{
	return "Model Renderer";
}

void AmberEngine::Core::ECS::Components::CModelRenderer::OnInspector(UI::WidgetContainer& p_root)
{
	UI::GUIDrawer::DrawMesh(p_root, "Model", m_model, &m_modelChangedEvent);
}
