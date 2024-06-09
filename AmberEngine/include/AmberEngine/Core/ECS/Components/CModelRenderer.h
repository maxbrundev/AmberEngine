#pragma once

#include "AmberEngine/API/Export.h"

#include "AmberEngine/Core/ECS/Components/AComponent.h"
#include "AmberEngine/Eventing/Event.h"

#include "AmberEngine/Resources/Model.h"

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
	class API_AMBERENGINE CModelRenderer : public AComponent
	{
	public:
		CModelRenderer(Actor& p_owner);
		virtual ~CModelRenderer() override = default;

		void SetModel(Resources::Model* p_model);
		Resources::Model* GetModel() const;
		
		std::string GetName() override;
		void OnInspector(UI::WidgetContainer& p_root) override;

	private:
		Resources::Model* m_model;
		std::string_view m_name;
		Eventing::Event<> m_modelChangedEvent;
	};
}
