#pragma once

#include "AmberCore/ECS/Components/CLight.h"

namespace AmberCore::ECS { class Actor; }

namespace AmberCore::ECS::Components
{
	class CAmbientSphereLight : public CLight
	{
	public:
		CAmbientSphereLight(Actor& p_owner);
		virtual ~CAmbientSphereLight() override = default;

		virtual std::string GetName() override;

		float GetRadius() const;
	
		void SetRadius(float p_radius);

		virtual void OnSerialize(tinyxml2::XMLDocument& p_doc, tinyxml2::XMLNode* p_node) override;
		virtual void OnDeserialize(tinyxml2::XMLDocument& p_doc, tinyxml2::XMLNode* p_node) override;

		virtual void OnInspector(AmberUI::WidgetContainer& p_root) override;
	};
}
