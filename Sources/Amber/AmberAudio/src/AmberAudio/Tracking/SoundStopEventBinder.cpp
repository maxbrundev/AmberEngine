#include "Amberpch.h"

#include "AmberAudio/Tracking/SoundStopEventBinder.h"

AmberAudio::Tracking::SoundStopEventBinder::SoundStopEventBinder(AmberTools::Eventing::Event<>& p_soundFinishedEvent) : SoundFinishedEvent(p_soundFinishedEvent)
{
}

void AmberAudio::Tracking::SoundStopEventBinder::OnSoundStopped(irrklang::ISound* sound, irrklang::E_STOP_EVENT_CAUSE reason, void* userData)
{
	SoundFinishedEvent.Invoke();
}
