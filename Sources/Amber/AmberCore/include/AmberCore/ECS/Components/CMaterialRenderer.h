#pragma once


#include "AmberCore/ECS/Components/AComponent.h"

#include "AmberCore/Resources/Material.h"

#include "AmberTools/Eventing/Event.h"

constexpr uint8_t MAX_MATERIAL_COUNT = 255;

namespace AmberCore::ECS { class Actor; }

namespace AmberCore::ECS::Components
{
	class CMaterialRenderer : public AComponent
	{
	public:
		using MaterialList  = std::array<AmberCore::Resources::Material*, MAX_MATERIAL_COUNT>;
		using MaterialField = std::array<std::array<AmberUI::Widgets::AWidget*, 3>, MAX_MATERIAL_COUNT>;

		static AmberTools::Eventing::Event<const std::string&> MaterialFilesGenerationRequestedEvent;

		CMaterialRenderer(Actor& p_owner);
		virtual ~CMaterialRenderer() override = default;

		void GenerateModelMaterials();
		void FillWithMaterial(AmberCore::Resources::Material& p_material);
		void FillWithMaterials(const MaterialList& p_materialList);
		void SetMaterialAtIndex(uint8_t p_index, AmberCore::Resources::Material& p_material);

		AmberCore::Resources::Material* GetMaterialAtIndex(uint8_t p_index) const;

		void RemoveMaterialAtIndex(uint8_t p_index);
		void RemoveMaterialByInstance(AmberCore::Resources::Material& p_instance);
		void RemoveAllMaterials();
		void UpdateMaterialList();
		void SetUserMatrixElement(uint32_t p_row, uint32_t p_column, float p_value);
		float GetUserMatrixElement(uint32_t p_row, uint32_t p_column) const;
		const MaterialList& GetMaterials() const;
		const glm::mat4& GetUserMatrix() const;

		virtual std::string GetName() override;

		virtual void OnSerialize(tinyxml2::XMLDocument& p_doc, tinyxml2::XMLNode* p_node) override;
		virtual void OnDeserialize(tinyxml2::XMLDocument& p_doc, tinyxml2::XMLNode* p_node) override;

		virtual void OnInspector(AmberUI::WidgetContainer& p_root) override;

	private:
		MaterialList m_materials;
		MaterialField m_materialFields;
		std::array<std::string, MAX_MATERIAL_COUNT> m_materialNames;
		glm::mat4 m_userMatrix;
	};
}
