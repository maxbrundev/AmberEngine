#pragma once

#include "CPhysicalObject.h"

namespace AmberEngine::Core::ECS { class Actor; }

namespace AmberEngine::Core::ECS::Components
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

		virtual void OnInspector(UI::WidgetContainer& p_root) override;
	};
}