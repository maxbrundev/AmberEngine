#include "Amberpch.h"

#include "AmberAudio/Tracking/SoundTracker.h"

AmberAudio::Tracking::SoundTracker::SoundTracker(irrklang::ISound* p_track) :
	m_track(p_track),
	m_soundStopEventBinder(StopEvent)
{
}

AmberAudio::Tracking::SoundTracker::~SoundTracker()
{
}

irrklang::ISound* AmberAudio::Tracking::SoundTracker::GetTrack() const
{
	return m_track;
}
