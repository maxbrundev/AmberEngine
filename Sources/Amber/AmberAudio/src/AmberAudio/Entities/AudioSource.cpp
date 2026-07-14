#include "Amberpch.h"

#include <irrklang/ik_vec3d.h>

#include "AmberAudio/Core/AudioPlayer.h"
#include "AmberAudio/Entities/AudioSource.h"

AmberTools::Eventing::Event<AmberAudio::Entities::AudioSource&> AmberAudio::Entities::AudioSource::CreatedEvent;
AmberTools::Eventing::Event<AmberAudio::Entities::AudioSource&> AmberAudio::Entities::AudioSource::DestroyedEvent;

AmberAudio::Entities::AudioSource::AudioSource(Core::AudioPlayer& p_audioPlayer) :
	m_audioPlayer(p_audioPlayer),
	m_transform(new AmberMaths::Transform()),
	m_internalTransform(true)
{
	Setup();
}

AmberAudio::Entities::AudioSource::AudioSource(Core::AudioPlayer& p_audioPlayer, AmberMaths::Transform& p_transform) :
	m_audioPlayer(p_audioPlayer),
	m_transform(&p_transform),
	m_internalTransform(false)
{
	Setup();
}

AmberAudio::Entities::AudioSource::~AudioSource()
{
	DestroyedEvent.Invoke(*this);

	StopAndDestroyTrackedSound();

	if (m_internalTransform)
		delete m_transform;
}

void AmberAudio::Entities::AudioSource::Setup()
{
	CreatedEvent.Invoke(*this);
}

void AmberAudio::Entities::AudioSource::UpdateTrackedSoundPosition()
{
	if (IsTrackingSound())
		m_trackedSound->GetTrack()->setPosition(reinterpret_cast<const irrklang::vec3df&>(m_transform->GetWorldPosition()));
}

void AmberAudio::Entities::AudioSource::ApplySourceSettingsToTrackedSound()
{
	m_trackedSound->GetTrack()->setVolume(m_volume);
	m_trackedSound->GetTrack()->setPan(m_pan);
	m_trackedSound->GetTrack()->setIsLooped(m_looped);
	m_trackedSound->GetTrack()->setPlaybackSpeed(m_pitch);
	m_trackedSound->GetTrack()->setMinDistance(m_attenuationThreshold);
}

bool AmberAudio::Entities::AudioSource::IsTrackingSound() const
{
	return m_trackedSound.operator bool();
}

AmberAudio::Tracking::SoundTracker* AmberAudio::Entities::AudioSource::GetTrackedSound() const
{
	return m_trackedSound.get();
}

void AmberAudio::Entities::AudioSource::SetVolume(float p_volume)
{
	m_volume = p_volume;

	if (IsTrackingSound())
		m_trackedSound->GetTrack()->setVolume(p_volume);
}

void AmberAudio::Entities::AudioSource::SetPan(float p_pan)
{
	m_pan = p_pan;

	if (IsTrackingSound())
		m_trackedSound->GetTrack()->setPan(p_pan * -1.0f);
}

void AmberAudio::Entities::AudioSource::SetLooped(bool p_looped)
{
	m_looped = p_looped;

	if (IsTrackingSound())
		m_trackedSound->GetTrack()->setIsLooped(p_looped);
}

void AmberAudio::Entities::AudioSource::SetPitch(float p_pitch)
{
	m_pitch = p_pitch;

	if (IsTrackingSound())
		m_trackedSound->GetTrack()->setPlaybackSpeed(p_pitch < 0.01f ? 0.01f : p_pitch);
}

void AmberAudio::Entities::AudioSource::SetSpatial(bool p_value)
{
	m_spatial = p_value;
}

void AmberAudio::Entities::AudioSource::SetAttenuationThreshold(float p_distance)
{
	m_attenuationThreshold = p_distance;

	if (IsTrackingSound())
		m_trackedSound->GetTrack()->setMinDistance(p_distance);
}

float AmberAudio::Entities::AudioSource::GetVolume() const
{
	return m_volume;
}

float AmberAudio::Entities::AudioSource::GetPan() const
{
	return m_pan;
}

bool AmberAudio::Entities::AudioSource::IsLooped() const
{
	return m_looped;
}

float AmberAudio::Entities::AudioSource::GetPitch() const
{
	return m_pitch;
}

bool AmberAudio::Entities::AudioSource::IsFinished() const
{
	if (IsTrackingSound())
		return m_trackedSound->GetTrack()->isFinished();
	else
		return true;
}

bool AmberAudio::Entities::AudioSource::IsSpatial() const
{
	return m_spatial;
}

float AmberAudio::Entities::AudioSource::GetAttenuationThreshold() const
{
	return m_attenuationThreshold;
}

void AmberAudio::Entities::AudioSource::Play(const Resources::Sound& p_sound)
{
	StopAndDestroyTrackedSound();

	if (m_spatial)
		m_trackedSound = m_audioPlayer.PlaySpatialSound(p_sound, false, m_looped, m_transform->GetWorldPosition(), true);
	else
		m_trackedSound = m_audioPlayer.PlaySound(p_sound, false, m_looped, true);

	if (IsTrackingSound())
	{
		m_trackedSound->GetTrack()->setVolume(m_volume);
		m_trackedSound->GetTrack()->setPan(m_pan);
		m_trackedSound->GetTrack()->setPlaybackSpeed(m_pitch);
		m_trackedSound->GetTrack()->setMinDistance(m_attenuationThreshold);
		m_trackedSound->GetTrack()->setIsPaused(false);
	}
}

void AmberAudio::Entities::AudioSource::Resume()
{
	if (IsTrackingSound())
		m_trackedSound->GetTrack()->setIsPaused(false);
}

void AmberAudio::Entities::AudioSource::Pause()
{
	if (IsTrackingSound())
		m_trackedSound->GetTrack()->setIsPaused(true);
}

void AmberAudio::Entities::AudioSource::Stop()
{
	if (IsTrackingSound())
		m_trackedSound->GetTrack()->stop();
}

void AmberAudio::Entities::AudioSource::StopAndDestroyTrackedSound()
{
	if (IsTrackingSound())
	{
		m_trackedSound->GetTrack()->stop();
		m_trackedSound->GetTrack()->drop();
	}
}
