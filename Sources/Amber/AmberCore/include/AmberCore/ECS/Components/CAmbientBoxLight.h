#pragma once

#include "AmberCore/ECS/Components/CLight.h"

namespace AmberCore::ECS { class Actor; }

namespace AmberCore::ECS::Components
{
	class CAmbientBoxLight : public CLight
	{
	public:
		CAmbientBoxLight(Actor& p_owner);
		virtual ~CAmbientBoxLight() override = default;

		virtual std::string GetName() override;

		glm::vec3 GetSize() const;

		void SetSize(const glm::vec3& p_size);

		virtual void OnSerialize(tinyxml2::XMLDocument& p_doc, tinyxml2::XMLNode* p_node) override;
		virtual void OnDeserialize(tinyxml2::XMLDocument& p_doc, tinyxml2::XMLNode* p_node) override;

		virtual void OnInspector(AmberUI::WidgetContainer& p_root) override;
	};
}
