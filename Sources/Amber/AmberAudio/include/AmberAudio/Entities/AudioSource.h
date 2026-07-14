#pragma once

#include <irrklang/ik_ISoundEngine.h>
#include <irrklang/ik_ISoundStopEventReceiver.h>

#include "AmberMaths/Transform.h"
#include "AmberTools/Eventing/Event.h"

#include "AmberAudio/Resources/Sound.h"
#include "AmberAudio/Tracking/SoundTracker.h"

namespace AmberAudio::Core { class AudioPlayer; }

namespace AmberAudio::Entities
{
	class AudioSource
	{
	public:
		AudioSource(Core::AudioPlayer& p_audioPlayer);
		AudioSource(Core::AudioPlayer& p_audioPlayer, AmberMaths::Transform& p_transform);
		~AudioSource();

		void UpdateTrackedSoundPosition();
		void ApplySourceSettingsToTrackedSound();

		bool IsTrackingSound() const;
		Tracking::SoundTracker* GetTrackedSound() const;

		void SetVolume(float p_volume);
		void SetPan(float p_pan);
		void SetLooped(bool p_looped);
		void SetPitch(float p_pitch);
		void SetSpatial(bool p_value);
		void SetAttenuationThreshold(float p_distance);

		float GetVolume() const;
		float GetPan() const;
		bool IsLooped() const;
		float GetPitch() const;
		bool IsFinished() const;
		bool IsSpatial() const;
		float GetAttenuationThreshold() const;

		void Play(const Resources::Sound& p_sound);
		void Resume();
		void Pause();
		void Stop();
		void StopAndDestroyTrackedSound();

	private:
		void Setup();

	public:
		static AmberTools::Eventing::Event<AudioSource&> CreatedEvent;
		static AmberTools::Eventing::Event<AudioSource&> DestroyedEvent;

	private:
		Core::AudioPlayer& m_audioPlayer;

		std::unique_ptr<Tracking::SoundTracker> m_trackedSound;

		float m_volume = 1.0f;
		float m_pan = 0.0f;
		bool m_looped = false;
		float m_pitch = 1.0f;
		bool m_spatial = false;
		float m_attenuationThreshold = 1.0f;

		AmberMaths::Transform* const m_transform;
		const bool m_internalTransform;
	};
}
