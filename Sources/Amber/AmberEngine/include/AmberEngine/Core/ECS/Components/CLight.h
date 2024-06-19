#pragma once

#include "AmberEngine/API/Export.h"

#include "AmberEngine/Core/ECS/Components/AComponent.h"

#include "AmberEngine/Rendering/Entities/Light.h"

#include "AmberEngine/Rendering/Settings/ELightType.h"

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
	class API_AMBERENGINE CLight : public AComponent
	{
	public:
		CLight(Actor& p_owner, Rendering::Settings::ELightType p_type);
		virtual ~CLight() override = default;

		void SetColor(const Data::Color& p_color);
		void SetIntensity(float p_intensity);

		const Data::Color& GetColor() const;

		float GetIntensity() const;

		Rendering::Entities::Light& GetData();

		virtual void OnSerialize(tinyxml2::XMLDocument& p_doc, tinyxml2::XMLNode* p_node) override;
		virtual void OnDeserialize(tinyxml2::XMLDocument& p_doc, tinyxml2::XMLNode* p_node) override;

		virtual void OnInspector(UI::WidgetContainer& p_root) override;

	protected:
		Rendering::Entities::Light m_data;
	};
}