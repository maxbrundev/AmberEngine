#pragma once

#include "AmberEngine/API/Export.h"

#include "AmberEngine/Core/ECS/Components/AComponent.h"

#include "AmberEngine/Resources/Material.h"

constexpr uint8_t MAX_MATERIAL_COUNT = 255;

namespace AmberEngine
{
	namespace Core
	{
		namespace ECS
		{
			class Actor;
		}
	}
}

namespace AmberEngine::Core::ECS::Components
{
	class API_AMBERENGINE CMaterialRenderer : public AComponent
	{
	public:
		using MaterialList  = std::array<Resources::Material*, MAX_MATERIAL_COUNT>;
		using MaterialField = std::array<std::array<UI::Widgets::AWidget*, 3>, MAX_MATERIAL_COUNT>;

		CMaterialRenderer(Actor& p_owner);
		virtual ~CMaterialRenderer() override = default;

		void FillTextureData(std::map<int, std::vector<std::tuple<Resources::ETextureType, std::string>>> p_textureData);
		void FillWithMaterial(Resources::Material& p_material);
		void FillWithMaterials(const MaterialList& p_materialList);
		void SetMaterialAtIndex(uint8_t p_index, Resources::Material& p_material);

		Resources::Material* GetMaterialAtIndex(uint8_t p_index) const;

		void RemoveMaterialAtIndex(uint8_t p_index);
		void RemoveMaterialByInstance(Resources::Material& p_instance);
		void RemoveAllMaterials();
		void UpdateMaterialList();
		void SetUserMatrixElement(uint32_t p_row, uint32_t p_column, float p_value);
		float GetUserMatrixElement(uint32_t p_row, uint32_t p_column) const;
		const MaterialList& GetMaterials() const;
		const glm::mat4& GetUserMatrix() const;

		std::string GetName() override;
		void OnInspector(UI::WidgetContainer& p_root) override;

	private:
		MaterialList m_materials;
		MaterialField m_materialFields;
		std::array<std::string, MAX_MATERIAL_COUNT> m_materialNames;
		glm::mat4 m_userMatrix;
	};
}
