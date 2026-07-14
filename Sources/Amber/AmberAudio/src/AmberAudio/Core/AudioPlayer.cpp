#include "Amberpch.h"

#include "AmberAudio/Core/AudioPlayer.h"

#include "AmberDebug/Logger.h"

AmberAudio::Core::AudioPlayer::AudioPlayer(AudioEngine& p_audioEngine) : m_audioEngine(p_audioEngine)
{
}

std::unique_ptr<AmberAudio::Tracking::SoundTracker> AmberAudio::Core::AudioPlayer::PlaySound(const Resources::Sound& p_sound, bool p_autoPlay, bool p_looped, bool p_track)
{
	std::unique_ptr<Tracking::SoundTracker> result;

	irrklang::ISound* sound = m_audioEngine.GetIrrklangEngine()->play2D((m_audioEngine.GetWorkingDirectory() + p_sound.Path).c_str(), p_looped, !p_autoPlay, p_track);

	if (p_track)
	{
		if (sound)
			result = std::make_unique<Tracking::SoundTracker>(sound);
		else
			AB_LOG_ERROR("Unable to play \"" + p_sound.Path + "\"");
	}

	return result;
}

std::unique_ptr<AmberAudio::Tracking::SoundTracker> AmberAudio::Core::AudioPlayer::PlaySpatialSound(const Resources::Sound& p_sound, bool p_autoPlay, bool p_looped, const glm::vec3& p_position, bool p_track)
{
	std::unique_ptr<Tracking::SoundTracker> result;

	irrklang::ISound* sound = m_audioEngine.GetIrrklangEngine()->play3D((m_audioEngine.GetWorkingDirectory() + p_sound.Path).c_str(), reinterpret_cast<const irrklang::vec3df&>(p_position), p_looped, !p_autoPlay, p_track);

	if (p_track)
	{
		if (sound)
			result = std::make_unique<Tracking::SoundTracker>(sound);
		else
			AB_LOG_ERROR("Unable to play \"" + p_sound.Path + "\"");
	}

	return result;
}
