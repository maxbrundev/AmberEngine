#pragma once

#include "AmberMaths/Transform.h"
#include "AmberTools/Eventing/Event.h"

namespace AmberAudio::Entities
{
	class AudioListener
	{
	public:
		AudioListener();
		AudioListener(AmberMaths::Transform& p_transform);
		~AudioListener();

		AmberMaths::Transform& GetTransform();

		void SetEnabled(bool p_enable);
		bool IsEnabled() const;

	private:
		void Setup();

	public:
		static AmberTools::Eventing::Event<AudioListener&> CreatedEvent;
		static AmberTools::Eventing::Event<AudioListener&> DestroyedEvent;

	private:
		AmberMaths::Transform* const m_transform;
		const bool m_internalTransform;

		bool m_enabled = true;
	};
}
