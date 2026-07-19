#include "Amberpch.h"

#include "AmberCore/ECS/Components/CMaterialRenderer.h"

#include "AmberCore/ECS/Actor.h"
#include "AmberCore/ECS/Components/CModelRenderer.h"
#include "AmberCore/Helpers/Serializer.h"

#include "AmberCore/ResourceManagement/MaterialManager.h"
#include "AmberCore/ResourceManagement/TextureManager.h"

#include "AmberTools/Global/ServiceLocator.h"
#include "AmberTools/Utils/PathParser.h"

#include "AmberCore/Helpers/GUIDrawer.h"
#include "AmberUI/Widgets/ButtonSmall.h"
#include "AmberUI/Widgets/Group.h"
#include "AmberUI/Widgets/Text.h"
#include "AmberUI/Widgets/TextColored.h"

AmberTools::Eventing::Event<const std::string&, const std::string&> AmberCore::ECS::Components::CMaterialRenderer::MaterialFilesGenerationRequestedEvent;

AmberCore::ECS::Components::CMaterialRenderer::CMaterialRenderer(Actor& p_owner) : AComponent(p_owner)
{
	m_materials.fill(nullptr);

	for (uint8_t i = 0; i < MAX_MATERIAL_COUNT; i++)
		m_materialFields[i].fill(nullptr);

	UpdateMaterialList();
}

void AmberCore::ECS::Components::CMaterialRenderer::GenerateModelMaterials(const std::string& p_shaderPath)
{
	if (auto modelRenderer = Owner.GetComponent<CModelRenderer>(); modelRenderer && modelRenderer->GetModel())
	{
		uint8_t materialIndex = 0;

		std::string modelName = AmberTools::Utils::PathParser::GetElementName(modelRenderer->GetModel()->Path);

		if (const size_t extensionPosition = modelName.find_last_of('.'); extensionPosition != std::string::npos)
		{
			modelName = modelName.substr(0, extensionPosition);
		}

		const std::string materialFolder = "Materials\\" + modelName + "\\";
		const std::string modelFolder = AmberTools::Utils::PathParser::GetContainingFolder(modelRenderer->GetModel()->Path);

		for (const std::string& materialName : modelRenderer->GetModel()->GetMaterialNames())
		{
			if(materialName.empty())
				continue;

			const std::string materialPath = materialFolder + materialName + ".abmat";

			MaterialFilesGenerationRequestedEvent.Invoke(materialPath, p_shaderPath);

			auto mat = AmberTools::Global::ServiceLocator::Get<AmberCore::ResourceManagement::MaterialManager>()[materialPath];
			m_materials[materialIndex] = mat;

			for (auto[type, path] : modelRenderer->GetModel()->LoadedTextureData[materialIndex])
			{
				auto texture = AmberTools::Global::ServiceLocator::Get<AmberCore::ResourceManagement::TextureManager>().GetResource(modelFolder + path);

				switch (type)
				{
				case AmberRendering::Resources::ETextureType::DIFFUSE_MAP:
					mat->SetUniform("u_DiffuseMap", texture);
					mat->SetUniform("u_AlbedoMap", texture);
					break;
				case AmberRendering::Resources::ETextureType::SPECULAR_MAP:
					mat->SetUniform("u_SpecularMap", texture);
					break;
				}
			}

			materialIndex++;
		}
	}
}

void AmberCore::ECS::Components::CMaterialRenderer::FillWithMaterial(AmberCore::Resources::Material& p_material)
{
	for (uint8_t i = 0; i < m_materials.size(); i++)
		m_materials[i] = &p_material;
}

void AmberCore::ECS::Components::CMaterialRenderer::FillWithMaterials(const MaterialList& p_materialList)
{
	for (uint8_t i = 0; i < m_materials.size(); i++)
		m_materials[i] = p_materialList[i];
}

void AmberCore::ECS::Components::CMaterialRenderer::SetMaterialAtIndex(uint8_t p_index, AmberCore::Resources::Material& p_material)
{
	m_materials[p_index] = &p_material;
}

AmberCore::Resources::Material* AmberCore::ECS::Components::CMaterialRenderer::GetMaterialAtIndex(uint8_t p_index) const
{
	return m_materials.at(p_index);
}

void AmberCore::ECS::Components::CMaterialRenderer::RemoveMaterialAtIndex(uint8_t p_index)
{
	if (p_index < m_materials.size())
	{
		m_materials[p_index] = nullptr;
	}
}

void AmberCore::ECS::Components::CMaterialRenderer::RemoveMaterialByInstance(AmberCore::Resources::Material& p_instance)
{
	for (uint8_t i = 0; i < m_materials.size(); i++)
		if (m_materials[i] == &p_instance)
			m_materials[i] = nullptr;
}

void AmberCore::ECS::Components::CMaterialRenderer::RemoveAllMaterials()
{
	for (uint8_t i = 0; i < m_materials.size(); i++)
		m_materials[i] = nullptr;
}

void AmberCore::ECS::Components::CMaterialRenderer::UpdateMaterialList()
{
	if (auto modelRenderer = Owner.GetComponent<CModelRenderer>(); modelRenderer && modelRenderer->GetModel())
	{
		uint8_t materialIndex = 0;

		for (const std::string& materialName : modelRenderer->GetModel()->GetMaterialNames())
		{
			m_materialNames[materialIndex++] = materialName;
		}

		for (uint8_t i = materialIndex; i < MAX_MATERIAL_COUNT; i++)
			m_materialNames[i] = "";
	}

	for (uint8_t i = 0; i < m_materialFields.size(); i++)
	{
		if (m_materialFields[i][0])
		{
			bool enabled = !m_materialNames[i].empty();
			m_materialFields[i][0]->Enabled = enabled;
			m_materialFields[i][1]->Enabled = enabled;
			m_materialFields[i][2]->Enabled = enabled;
			reinterpret_cast<AmberUI::Widgets::Text*>(m_materialFields[i][0])->Content = m_materialNames[i];
		}
	}
}

void AmberCore::ECS::Components::CMaterialRenderer::SetUserMatrixElement(uint32_t p_row, uint32_t p_column, float p_value)
{
	if (p_row < 4 && p_column < 4)
		m_userMatrix[p_row][p_column] = p_value;
}

float AmberCore::ECS::Components::CMaterialRenderer::GetUserMatrixElement(uint32_t p_row,
	uint32_t p_column) const
{
	if (p_row < 4 && p_column < 4)
		return m_userMatrix[p_row][p_column];
	
	return 0.0f;
}

const AmberCore::ECS::Components::CMaterialRenderer::MaterialList& AmberCore::ECS::Components::
CMaterialRenderer::GetMaterials() const
{
	return m_materials;
}

const glm::mat4& AmberCore::ECS::Components::CMaterialRenderer::GetUserMatrix() const
{
	return m_userMatrix;
}

std::string AmberCore::ECS::Components::CMaterialRenderer::GetName()
{
	return "Material Renderer";
}

void AmberCore::ECS::Components::CMaterialRenderer::OnSerialize(tinyxml2::XMLDocument& p_doc, tinyxml2::XMLNode* p_node)
{
	tinyxml2::XMLNode* materialsNode = p_doc.NewElement("materials");
	p_node->InsertEndChild(materialsNode);

	auto modelRenderer = Owner.GetComponent<CModelRenderer>();
	uint8_t elementsToSerialize = modelRenderer->GetModel() ? (uint8_t)std::min(modelRenderer->GetModel()->GetMaterialNames().size(), (size_t)MAX_MATERIAL_COUNT) : 0;

	for (uint8_t i = 0; i < elementsToSerialize; ++i)
	{
		Helpers::Serializer::SerializeMaterial(p_doc, materialsNode, "material", m_materials[i]);
	}
}

void AmberCore::ECS::Components::CMaterialRenderer::OnDeserialize(tinyxml2::XMLDocument& p_doc, tinyxml2::XMLNode* p_node)
{
	tinyxml2::XMLNode* materialsRoot = p_node->FirstChildElement("materials");
	if (materialsRoot)
	{
		tinyxml2::XMLElement* currentMaterial = materialsRoot->FirstChildElement("material");

		uint8_t materialIndex = 0;

		while (currentMaterial)
		{
			if (auto material = AmberTools::Global::ServiceLocator::Get<AmberCore::ResourceManagement::MaterialManager>()[currentMaterial->GetText()])
				m_materials[materialIndex] = material;

			currentMaterial = currentMaterial->NextSiblingElement("material");
			++materialIndex;
		}
	}

	UpdateMaterialList();
}

std::array<AmberUI::Widgets::AWidget*, 3> CustomMaterialDrawer(AmberUI::WidgetContainer& p_root, const std::string& p_name, AmberCore::Resources::Material*& p_data)
{
	std::array<AmberUI::Widgets::AWidget*, 3> widgets;

	widgets[0] = &p_root.CreateWidget<AmberUI::Widgets::TextColored>(p_name, AmberCore::Helpers::GUIDrawer::TITLE_COLOR);

	std::string displayedText = (p_data ? p_data->path : std::string("Empty"));
	auto & rightSide = p_root.CreateWidget<AmberUI::Widgets::Group>();

	auto& widget = rightSide.CreateWidget<AmberUI::Widgets::Text>(displayedText);

	widgets[1] = &widget;

	widget.AddDragDropTarget("File").DataReceivedEvent += [&widget, &p_data](auto p_receivedData)
	{
		if (AmberTools::Utils::PathParser::GetFileType(p_receivedData.first) == AmberTools::Utils::PathParser::EFileType::MATERIAL)
		{
			if (auto resource = AmberTools::Global::ServiceLocator::Get<AmberCore::ResourceManagement::MaterialManager>().GetResource(p_receivedData.first); resource)
			{
				p_data = resource;
				widget.Content = p_receivedData.first;
			}
		}
	};

	widget.LineBreak = false;

	auto & resetButton = rightSide.CreateWidget<AmberUI::Widgets::ButtonSmall>("Clear");
	resetButton.IdleBackgroundColor = AmberCore::Helpers::GUIDrawer::CLEAR_BUTTON_COLOR;
	resetButton.ClickedEvent += [&widget, &p_data]
	{
		p_data = nullptr;
		widget.Content = "Empty";
	};

	widgets[2] = &resetButton;

	return widgets;
}

void AmberCore::ECS::Components::CMaterialRenderer::OnInspector(AmberUI::WidgetContainer& p_root)
{
	for (uint8_t i = 0; i < m_materials.size(); i++)
	{
		m_materialFields[i] = CustomMaterialDrawer(p_root, "Material", m_materials[i]);
	}

	UpdateMaterialList();
}
