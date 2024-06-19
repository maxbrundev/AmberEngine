#pragma once

#include "AmberEngine/API/Export.h"


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
	class API_AMBERENGINE CDirectionalLight : public CLight
	{
	public:
		CDirectionalLight(Actor& p_owner);
		virtual ~CDirectionalLight() override = default;

		virtual std::string GetName() override;

		virtual void OnSerialize(tinyxml2::XMLDocument& p_doc, tinyxml2::XMLNode* p_node) override;
		virtual void OnDeserialize(tinyxml2::XMLDocument& p_doc, tinyxml2::XMLNode* p_node) override;

		virtual void OnInspector(UI::WidgetContainer& p_root) override;
	};
}
