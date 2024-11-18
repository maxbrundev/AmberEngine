#pragma once

#include "AmberEngine/Core/ECS/Components/CLight.h"

namespace AmberEngine::Core::ECS { class Actor; }

namespace AmberEngine::Core::ECS::Components
{
	class API_AMBERENGINE CAmbientBoxLight : public CLight
	{
	public:
		CAmbientBoxLight(Actor& p_owner);
		virtual ~CAmbientBoxLight() override = default;

		virtual std::string GetName() override;

		glm::vec3 GetSize() const;

		void SetSize(const glm::vec3& p_size);

		virtual void OnSerialize(tinyxml2::XMLDocument& p_doc, tinyxml2::XMLNode* p_node) override;
		virtual void OnDeserialize(tinyxml2::XMLDocument& p_doc, tinyxml2::XMLNode* p_node) override;

		virtual void OnInspector(UI::WidgetContainer& p_root) override;
	};
}
