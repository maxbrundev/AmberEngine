#include "Amberpch.h"

#include "AmberEngine/Core/ECS/Components/CMaterialRenderer.h"

#include "AmberEngine/Core/ECS/Actor.h"

#include "AmberEngine/Core/ECS/Components/CModelRenderer.h"
#include "AmberEngine/Managers/ShaderManager.h"
#include "AmberEngine/Managers/TextureManager.h"
#include "AmberEngine/Tools/Global/ServiceLocator.h"
#include "AmberEngine/UI/GUIDrawer.h"
#include "AmberEngine/UI/Widgets/ButtonSmall.h"
#include "AmberEngine/UI/Widgets/Group.h"
#include "AmberEngine/UI/Widgets/Text.h"
#include "AmberEngine/UI/Widgets/TextColored.h"

AmberEngine::Core::ECS::Components::CMaterialRenderer::CMaterialRenderer(Actor& p_owner) : AComponent(p_owner)
{
	m_materials.fill(nullptr);

	for (uint8_t i = 0; i < MAX_MATERIAL_COUNT; i++)
		m_materialFields[i].fill(nullptr);

	UpdateMaterialList();
}

void AmberEngine::Core::ECS::Components::CMaterialRenderer::FillTextureData(std::map<int, std::vector<std::tuple<Resources::ETextureType, std::string>>> p_textureData)
{
	for (uint8_t i = 0; i < m_materials.size(); i++)
	{
		m_materials[i] = new Resources::Material();

		m_materials[i]->SetShader(Tools::Global::ServiceLocator::Get<ResourceManagement::ShaderManager>().GetResource(":Shaders\\Standard.glsl"));

		for (auto [type, path] : p_textureData[i])
		{
			std::string uniform;

			switch (type)
			{
			case Resources::ETextureType::DIFFUSE_MAP:
				uniform = "u_DiffuseMap";
				break;
			case Resources::ETextureType::SPECULAR_MAP:
				uniform = "u_SpecularMap";
				break;
			}

			m_materials[i]->SetUniform(uniform, Tools::Global::ServiceLocator::Get<ResourceManagement::TextureManager>().GetResource(path));
		}
	}
}

void AmberEngine::Core::ECS::Components::CMaterialRenderer::FillWithMaterial(Resources::Material& p_material)
{
	for (uint8_t i = 0; i < m_materials.size(); i++)
		m_materials[i] = &p_material;
}

void AmberEngine::Core::ECS::Components::CMaterialRenderer::FillWithMaterials(const MaterialList& p_materialList)
{
	for (uint8_t i = 0; i < m_materials.size(); i++)
		m_materials[i] = p_materialList[i];
}

void AmberEngine::Core::ECS::Components::CMaterialRenderer::SetMaterialAtIndex(uint8_t p_index, Resources::Material& p_material)
{
	m_materials[p_index] = &p_material;
}

AmberEngine::Resources::Material* AmberEngine::Core::ECS::Components::CMaterialRenderer::GetMaterialAtIndex(uint8_t p_index) const
{
	return m_materials.at(p_index);
}

void AmberEngine::Core::ECS::Components::CMaterialRenderer::RemoveMaterialAtIndex(uint8_t p_index)
{
	if (p_index < m_materials.size())
	{
		m_materials[p_index] = nullptr;
	}
}

void AmberEngine::Core::ECS::Components::CMaterialRenderer::RemoveMaterialByInstance(Resources::Material& p_instance)
{
	for (uint8_t i = 0; i < m_materials.size(); i++)
		if (m_materials[i] == &p_instance)
			m_materials[i] = nullptr;
}

void AmberEngine::Core::ECS::Components::CMaterialRenderer::RemoveAllMaterials()
{
	for (uint8_t i = 0; i < m_materials.size(); i++)
		m_materials[i] = nullptr;
}

void AmberEngine::Core::ECS::Components::CMaterialRenderer::UpdateMaterialList()
{
	if (auto modelRenderer = owner.GetComponent<CModelRenderer>(); modelRenderer && modelRenderer->GetModel())
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
			m_materialFields[i][0]->enabled = enabled;
			m_materialFields[i][1]->enabled = enabled;
			m_materialFields[i][2]->enabled = enabled;
			reinterpret_cast<UI::Widgets::Text*>(m_materialFields[i][0])->content = m_materialNames[i];
		}
	}
}

void AmberEngine::Core::ECS::Components::CMaterialRenderer::SetUserMatrixElement(uint32_t p_row, uint32_t p_column, float p_value)
{
	if (p_row < 4 && p_column < 4)
		m_userMatrix[p_row][p_column] = p_value;
}

float AmberEngine::Core::ECS::Components::CMaterialRenderer::GetUserMatrixElement(uint32_t p_row,
	uint32_t p_column) const
{
	if (p_row < 4 && p_column < 4)
		return m_userMatrix[p_row][p_column];
	
	return 0.0f;
}

const AmberEngine::Core::ECS::Components::CMaterialRenderer::MaterialList& AmberEngine::Core::ECS::Components::
CMaterialRenderer::GetMaterials() const
{
	return m_materials;
}

const glm::mat4& AmberEngine::Core::ECS::Components::CMaterialRenderer::GetUserMatrix() const
{
	return m_userMatrix;
}

std::string AmberEngine::Core::ECS::Components::CMaterialRenderer::GetName()
{
	return "Material Renderer";
}

std::array<AmberEngine::UI::Widgets::AWidget*, 3> CustomMaterialDrawer(AmberEngine::UI::WidgetContainer& p_root, const std::string& p_name, AmberEngine::Resources::Material*& p_data)
{
	std::array<AmberEngine::UI::Widgets::AWidget*, 3> widgets;

	widgets[0] = &p_root.CreateWidget<AmberEngine::UI::Widgets::TextColored>(p_name, AmberEngine::UI::GUIDrawer::TITLE_COLOR);

	std::string displayedText = (p_data ? p_data->path : std::string("Empty"));
	auto & rightSide = p_root.CreateWidget<AmberEngine::UI::Widgets::Group>();

	auto& widget = rightSide.CreateWidget<AmberEngine::UI::Widgets::Text>(displayedText);

	widgets[1] = &widget;

	widget.lineBreak = false;

	auto & resetButton = rightSide.CreateWidget<AmberEngine::UI::Widgets::ButtonSmall>("Clear");
	resetButton.idleBackgroundColor = AmberEngine::UI::GUIDrawer::CLEAR_BUTTON_COLOR;
	resetButton.ClickedEvent += [&widget, &p_data]
	{
		p_data = nullptr;
		widget.content = "Empty";
	};

	widgets[2] = &resetButton;

	return widgets;
}

void AmberEngine::Core::ECS::Components::CMaterialRenderer::OnInspector(UI::WidgetContainer& p_root)
{
	for (uint8_t i = 0; i < m_materials.size(); i++)
	{
		m_materialFields[i] = CustomMaterialDrawer(p_root, "Material", m_materials[i]);
	}

	UpdateMaterialList();
}
