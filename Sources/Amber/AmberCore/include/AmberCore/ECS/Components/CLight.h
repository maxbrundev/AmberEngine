#pragma once


#include "AmberCore/ECS/Components/AComponent.h"

#include "AmberRendering/Settings/ELightType.h"
#include "AmberRendering/Entities/Light.h"

namespace AmberCore::ECS { class Actor; }

namespace AmberCore::ECS::Components
{
	class CLight : public AComponent
	{
	public:
		CLight(Actor& p_owner, AmberRendering::Settings::ELightType p_type);
		virtual ~CLight() override = default;

		void SetColor(const AmberRendering::Data::Color& p_color);
		void SetIntensity(float p_intensity);

		const AmberRendering::Data::Color& GetColor() const;

		float GetIntensity() const;

		AmberRendering::Entities::Light& GetData();

		virtual void OnSerialize(tinyxml2::XMLDocument& p_doc, tinyxml2::XMLNode* p_node) override;
		virtual void OnDeserialize(tinyxml2::XMLDocument& p_doc, tinyxml2::XMLNode* p_node) override;

		virtual void OnInspector(AmberUI::WidgetContainer& p_root) override;

	protected:
		AmberRendering::Entities::Light m_data;
	};
}