#pragma once

#include "AmberAudio/Entities/AudioListener.h"

#include "AmberCore/ECS/Components/AComponent.h"

namespace AmberCore::ECS { class Actor; }

namespace AmberCore::ECS::Components
{
	class CAudioListener : public AComponent
	{
	public:
		CAudioListener(Actor& p_owner);

		std::string GetName() override;

		virtual void OnSerialize(tinyxml2::XMLDocument& p_doc, tinyxml2::XMLNode* p_node) override;
		virtual void OnDeserialize(tinyxml2::XMLDocument& p_doc, tinyxml2::XMLNode* p_node) override;
		virtual void OnInspector(AmberUI::WidgetContainer& p_root) override;
		virtual void OnEnable() override;
		virtual void OnDisable() override;

	private:
		AmberAudio::Entities::AudioListener m_audioListener;
	};
}
