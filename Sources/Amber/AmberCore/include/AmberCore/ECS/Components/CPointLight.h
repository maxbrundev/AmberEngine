#pragma once


#include "AmberCore/ECS/Components/CLight.h"

namespace AmberCore::ECS
{
	class Actor;
}

namespace AmberCore::ECS::Components
{
	class CPointLight : public CLight
	{
	public:
		CPointLight(Actor& p_owner);
		virtual ~CPointLight() override = default;

		float GetConstant() const;

		float GetLinear() const;

		float GetQuadratic() const;

		void SetConstant(float p_constant);

		void SetLinear(float p_linear);

		void SetQuadratic(float p_quadratic);

		virtual std::string GetName() override;

		virtual void OnSerialize(tinyxml2::XMLDocument& p_doc, tinyxml2::XMLNode* p_node) override;
		virtual void OnDeserialize(tinyxml2::XMLDocument& p_doc, tinyxml2::XMLNode* p_node) override;

		virtual void OnInspector(AmberUI::WidgetContainer& p_root) override;
	};
}
