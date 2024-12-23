#pragma once

#include "AmberEngine/Core/ECS/Components/CLight.h"

namespace AmberEngine
{
	namespace Core
	{
		namespace ECS
		{
			class Actor;
		}
	}
}

namespace AmberEngine::Core::ECS::Components
{
	class CSpotLight : public CLight
	{
	public:
		CSpotLight(Actor& p_owner);
		virtual ~CSpotLight() override = default;

		float GetConstant() const;
		float GetLinear() const;
		float GetQuadratic() const;
		float GetCutoff() const;
		float GetOuterCutoff() const;
		void SetConstant(float p_constant);
		void SetLinear(float p_linear);
		void SetQuadratic(float p_quadratic);
		void SetCutoff(float p_cutoff);
		void SetOuterCutoff(float p_outerCutoff);

		virtual std::string GetName() override;

		virtual void OnSerialize(tinyxml2::XMLDocument& p_doc, tinyxml2::XMLNode* p_node) override;
		virtual void OnDeserialize(tinyxml2::XMLDocument& p_doc, tinyxml2::XMLNode* p_node) override;

		virtual void OnInspector(UI::WidgetContainer& p_root) override;
	};
}
