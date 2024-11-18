#pragma once

#include "AmberEngine/Core/ECS/Components/CLight.h"

namespace AmberEngine::Core::ECS { class Actor; }

namespace AmberEngine::Core::ECS::Components
{
	class API_AMBERENGINE CAmbientSphereLight : public CLight
	{
	public:
		CAmbientSphereLight(Actor& p_owner);
		virtual ~CAmbientSphereLight() override = default;

		virtual std::string GetName() override;

		float GetRadius() const;
	
		void SetRadius(float p_radius);

		virtual void OnSerialize(tinyxml2::XMLDocument& p_doc, tinyxml2::XMLNode* p_node) override;
		virtual void OnDeserialize(tinyxml2::XMLDocument& p_doc, tinyxml2::XMLNode* p_node) override;

		virtual void OnInspector(UI::WidgetContainer& p_root) override;
	};
}
