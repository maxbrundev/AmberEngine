#pragma once

#include <irrklang/irrKlang.h>
#include <irrklang/ik_ISoundStopEventReceiver.h>

#include "AmberTools/Eventing/Event.h"

namespace AmberAudio::Tracking
{
	class SoundStopEventBinder : public irrklang::ISoundStopEventReceiver
	{
	public:
		SoundStopEventBinder(AmberTools::Eventing::Event<>& p_soundFinishedEvent);

		virtual void OnSoundStopped(irrklang::ISound* sound, irrklang::E_STOP_EVENT_CAUSE reason, void* userData) override;

	public:
		AmberTools::Eventing::Event<>& SoundFinishedEvent;
	};
}
