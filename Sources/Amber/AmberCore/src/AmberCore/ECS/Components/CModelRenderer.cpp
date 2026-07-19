#include "Amberpch.h"

#include "AmberCore/ECS/Components/CModelRenderer.h"

#include "AmberCore/ECS/Actor.h"
#include "AmberCore/ECS/Components/CMaterialRenderer.h"

#include "AmberCore/Helpers/Serializer.h"

#include "AmberCore/ResourceManagement/ModelManager.h"

#include "AmberCore/Helpers/GUIDrawer.h"

#include "AmberUI/Widgets/Button.h"
#include "AmberUI/Widgets/ComboBox.h"


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

void AmberCore::ECS::Components::CModelRenderer::SetFrustumBehaviour(EFrustumBehaviour p_boundingMode)
{
	m_frustumBehaviour = p_boundingMode;
}

AmberCore::ECS::Components::CModelRenderer::EFrustumBehaviour AmberCore::ECS::Components::CModelRenderer::GetFrustumBehaviour() const
{
	return m_frustumBehaviour;
}

const AmberRendering::Geometry::BoundingSphere& AmberCore::ECS::Components::CModelRenderer::GetCustomBoundingSphere() const
{
	return m_customBoundingSphere;
}

void AmberCore::ECS::Components::CModelRenderer::SetCustomBoundingSphere(const AmberRendering::Geometry::BoundingSphere& p_boundingSphere)
{
	m_customBoundingSphere = p_boundingSphere;
}

std::string AmberCore::ECS::Components::CModelRenderer::GetName()
{
	return "Model Renderer";
}

void AmberCore::ECS::Components::CModelRenderer::OnSerialize(tinyxml2::XMLDocument& p_doc, tinyxml2::XMLNode* p_node)
{
	Helpers::Serializer::SerializeModel(p_doc, p_node, "model", m_model);
	Helpers::Serializer::SerializeInt(p_doc, p_node, "frustum_behaviour", static_cast<int>(m_frustumBehaviour));
	Helpers::Serializer::SerializeVec3(p_doc, p_node, "custom_bounding_sphere_position", m_customBoundingSphere.Position);
	Helpers::Serializer::SerializeFloat(p_doc, p_node, "custom_bounding_sphere_radius", m_customBoundingSphere.Radius);
}

void AmberCore::ECS::Components::CModelRenderer::OnDeserialize(tinyxml2::XMLDocument& p_doc, tinyxml2::XMLNode* p_node)
{
	Helpers::Serializer::DeserializeModel(p_doc, p_node, "model", m_model);

	if (p_node->FirstChildElement("frustum_behaviour"))
	{
		m_frustumBehaviour = static_cast<EFrustumBehaviour>(Helpers::Serializer::DeserializeInt(p_doc, p_node, "frustum_behaviour"));
		m_customBoundingSphere.Position = Helpers::Serializer::DeserializeVec3(p_doc, p_node, "custom_bounding_sphere_position");
		m_customBoundingSphere.Radius = Helpers::Serializer::DeserializeFloat(p_doc, p_node, "custom_bounding_sphere_radius");
	}
}

void AmberCore::ECS::Components::CModelRenderer::OnInspector(AmberUI::WidgetContainer& p_root)
{
	AmberCore::Helpers::GUIDrawer::DrawMesh(p_root, "Model", m_model, &m_modelChangedEvent);

	AmberCore::Helpers::GUIDrawer::CreateTitle(p_root, "Material Generation");
	auto& shaderChoice = p_root.CreateWidget<AmberUI::Widgets::ComboBox>(0);
	shaderChoice.Choices.emplace(0, "Standard");
	shaderChoice.Choices.emplace(1, "Standard PBR");
	shaderChoice.Choices.emplace(2, "Lambert");
	shaderChoice.Choices.emplace(3, "Unlit");

	p_root.CreateWidget<AmberUI::Widgets::Text>("");
	auto& generateMaterialsButton = p_root.CreateWidget<AmberUI::Widgets::Button>("Generate materials");
	generateMaterialsButton.ClickedEvent += [this, &shaderChoice]()
	{
		std::string shaderPath;

		switch (shaderChoice.CurrentChoice)
		{
		case 0:
			shaderPath = ":Shaders\\Standard.glsl";
			break;
		case 1:
			shaderPath = ":Shaders\\StandardPBR.glsl";
			break;
		case 2:
			shaderPath = ":Shaders\\Lambert.glsl";
			break;
		case 3:
			shaderPath = ":Shaders\\Unlit.glsl";
			break;
		}

		if (auto materialRenderer = Owner.GetComponent<CMaterialRenderer>())
		{
			materialRenderer->GenerateModelMaterials(shaderPath);
			materialRenderer->UpdateMaterialList();
		}
	};

	AmberCore::Helpers::GUIDrawer::CreateTitle(p_root, "Frustum Culling Behaviour");
	auto& boundingMode = p_root.CreateWidget<AmberUI::Widgets::ComboBox>(static_cast<int>(m_frustumBehaviour));
	boundingMode.Choices.emplace(0, "Disabled");
	boundingMode.Choices.emplace(1, "Cull model");
	boundingMode.Choices.emplace(2, "Cull model & sub-meshes");
	boundingMode.Choices.emplace(3, "Cull custom bounding sphere");

	AmberCore::Helpers::GUIDrawer::DrawVec3(p_root, "Bounding Sphere Center", [this]() { return m_customBoundingSphere.Position; }, [this](glm::vec3 p_position) { m_customBoundingSphere.Position = p_position; }, 0.05f);
	auto& centerWidget = *p_root.GetWidgets()[p_root.GetWidgets().size() - 1];
	auto& centerLabel = *p_root.GetWidgets()[p_root.GetWidgets().size() - 2];

	AmberCore::Helpers::GUIDrawer::DrawScalar<float>(p_root, "Bounding Sphere Radius", [this]() { return m_customBoundingSphere.Radius; }, [this](float p_radius) { m_customBoundingSphere.Radius = p_radius; }, 0.1f);
	auto& radiusWidget = *p_root.GetWidgets()[p_root.GetWidgets().size() - 1];
	auto& radiusLabel = *p_root.GetWidgets()[p_root.GetWidgets().size() - 2];

	boundingMode.ValueChangedEvent += [this, &centerWidget, &centerLabel, &radiusWidget, &radiusLabel](int p_choice)
	{
		m_frustumBehaviour = static_cast<EFrustumBehaviour>(p_choice);
		centerWidget.Enabled = centerLabel.Enabled = radiusWidget.Enabled = radiusLabel.Enabled = p_choice == 3;
	};

	centerWidget.Enabled = centerLabel.Enabled = radiusWidget.Enabled = radiusLabel.Enabled = m_frustumBehaviour == EFrustumBehaviour::CULL_CUSTOM;
}
