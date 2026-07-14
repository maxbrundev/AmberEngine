#pragma once


#include "AmberCore/ECS/Components/CLight.h"

namespace AmberCore::ECS { class Actor; }

namespace AmberCore::ECS::Components
{
	class CDirectionalLight : public CLight
	{
	public:
		CDirectionalLight(Actor& p_owner);
		virtual ~CDirectionalLight() override = default;

		virtual std::string GetName() override;

		virtual void OnSerialize(tinyxml2::XMLDocument& p_doc, tinyxml2::XMLNode* p_node) override;
		virtual void OnDeserialize(tinyxml2::XMLDocument& p_doc, tinyxml2::XMLNode* p_node) override;

		virtual void OnInspector(AmberUI::WidgetContainer& p_root) override;
	};
}
