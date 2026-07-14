#pragma once

#include <irrklang/irrKlang.h>

#include "AmberAudio/Tracking/SoundStopEventBinder.h"

namespace AmberAudio::Tracking
{
	class SoundTracker
	{
	public:
		SoundTracker(irrklang::ISound* p_track);
		~SoundTracker();

		irrklang::ISound* GetTrack() const;

	public:
		AmberTools::Eventing::Event<> StopEvent;

	protected:
		irrklang::ISound* const m_track = nullptr;

	private:
		SoundStopEventBinder m_soundStopEventBinder;
	};
}
