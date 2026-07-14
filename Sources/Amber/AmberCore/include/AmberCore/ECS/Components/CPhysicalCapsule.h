#pragma once

#include "CPhysicalObject.h"

namespace AmberCore::ECS { class Actor; }

namespace AmberCore::ECS::Components
{
	class CPhysicalCapsule : public CPhysicalObject
	{
	public:
		CPhysicalCapsule(ECS::Actor& p_owner);

		std::string GetName() override;

		void SetRadius(float p_radius);

		void SetHeight(float p_height);

		float GetRadius() const;

		float GetHeight() const;

		virtual void OnSerialize(tinyxml2::XMLDocument& p_doc, tinyxml2::XMLNode* p_node) override;

		virtual void OnDeserialize(tinyxml2::XMLDocument& p_doc, tinyxml2::XMLNode* p_node) override;

		virtual void OnInspector(AmberUI::WidgetContainer& p_root) override;
	};
}