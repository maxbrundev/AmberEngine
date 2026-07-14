#pragma once


#include "AmberCore/ECS/Components/AComponent.h"

#include "AmberTools/Eventing/Event.h"

#include "AmberRendering/Resources/Model.h"

namespace AmberCore::ECS { class Actor; }

namespace AmberCore::ECS::Components
{
	class CModelRenderer : public AComponent
	{
	public:
		CModelRenderer(Actor& p_owner);
		virtual ~CModelRenderer() override = default;

		void SetModel(AmberRendering::Resources::Model* p_model);
		AmberRendering::Resources::Model* GetModel() const;
		
		virtual std::string GetName() override;

		virtual void OnSerialize(tinyxml2::XMLDocument& p_doc, tinyxml2::XMLNode* p_node) override;
		virtual void OnDeserialize(tinyxml2::XMLDocument& p_doc, tinyxml2::XMLNode* p_node) override;

		virtual void OnInspector(AmberUI::WidgetContainer& p_root) override;

	private:
		AmberRendering::Resources::Model* m_model = nullptr;
		AmberTools::Eventing::Event<> m_modelChangedEvent;
	};
}
