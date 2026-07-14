#include "Amberpch.h"

#include "AmberCore/ECS/Components/CModelRenderer.h"

#include "AmberCore/ECS/Actor.h"
#include "AmberCore/ECS/Components/CMaterialRenderer.h"

#include "AmberCore/Helpers/Serializer.h"

#include "AmberCore/ResourceManagement/ModelManager.h"

#include "AmberCore/Helpers/GUIDrawer.h"


AmberCore::ECS::Components::CModelRenderer::CModelRenderer(Actor& p_owner) : AComponent(p_owner)
{
	m_modelChangedEvent += [this]
	{
		if (const auto materialRenderer = Owner.GetComponent<CMaterialRenderer>())
		{
			materialRenderer->UpdateMaterialList();
		}
	};
}

void AmberCore::ECS::Components::CModelRenderer::SetModel(AmberRendering::Resources::Model* p_model)
{
	m_model = p_model;
	m_modelChangedEvent.Invoke();
}

AmberRendering::Resources::Model* AmberCore::ECS::Components::CModelRenderer::GetModel() const
{
	return m_model;
}

std::string AmberCore::ECS::Components::CModelRenderer::GetName()
{
	return "Model Renderer";
}

void AmberCore::ECS::Components::CModelRenderer::OnSerialize(tinyxml2::XMLDocument& p_doc, tinyxml2::XMLNode* p_node)
{
	Helpers::Serializer::SerializeModel(p_doc, p_node, "model", m_model);
}

void AmberCore::ECS::Components::CModelRenderer::OnDeserialize(tinyxml2::XMLDocument& p_doc, tinyxml2::XMLNode* p_node)
{
	Helpers::Serializer::DeserializeModel(p_doc, p_node, "model", m_model);
}

void AmberCore::ECS::Components::CModelRenderer::OnInspector(AmberUI::WidgetContainer& p_root)
{
	AmberCore::Helpers::GUIDrawer::DrawMesh(p_root, "Model", m_model, &m_modelChangedEvent);
}
