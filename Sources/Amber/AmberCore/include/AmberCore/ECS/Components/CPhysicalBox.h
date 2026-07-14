#pragma once

#include "CPhysicalObject.h"

namespace AmberCore::ECS { class Actor; }

namespace AmberCore::ECS::Components
{
	class CPhysicalBox : public CPhysicalObject
	{
	public:
		CPhysicalBox(ECS::Actor& p_owner);
		std::string GetName() override;

		void SetSize(const glm::vec3& p_size);

		glm::vec3 GetSize() const;

		virtual void OnSerialize(tinyxml2::XMLDocument& p_doc, tinyxml2::XMLNode* p_node) override;

		virtual void OnDeserialize(tinyxml2::XMLDocument& p_doc, tinyxml2::XMLNode* p_node) override;

		virtual void OnInspector(AmberUI::WidgetContainer& p_root) override;
	};
}