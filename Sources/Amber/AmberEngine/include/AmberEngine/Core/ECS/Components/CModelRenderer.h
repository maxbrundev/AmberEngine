#pragma once

#include "AmberEngine/API/Export.h"

#include "AmberEngine/Core/ECS/Components/AComponent.h"

#include "AmberEngine/Eventing/Event.h"

#include "AmberEngine/Resources/Model.h"

namespace AmberEngine::Core::ECS { class Actor; }

namespace AmberEngine::Core::ECS::Components
{
	class API_AMBERENGINE CModelRenderer : public AComponent
	{
	public:
		CModelRenderer(Actor& p_owner);
		virtual ~CModelRenderer() override = default;

		void SetModel(Resources::Model* p_model);
		Resources::Model* GetModel() const;
		
		virtual std::string GetName() override;

		virtual void OnSerialize(tinyxml2::XMLDocument& p_doc, tinyxml2::XMLNode* p_node) override;
		virtual void OnDeserialize(tinyxml2::XMLDocument& p_doc, tinyxml2::XMLNode* p_node) override;

		virtual void OnInspector(UI::WidgetContainer& p_root) override;

	private:
		Resources::Model* m_model = nullptr;
		Eventing::Event<> m_modelChangedEvent;
	};
}
