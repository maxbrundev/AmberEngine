#pragma once

#include <vector>

#include "AmberAudio/Entities/AudioListener.h"
#include "AmberAudio/Entities/AudioSource.h"

namespace AmberAudio::Core
{
	class AudioEngine
	{
	public:
		AudioEngine(const std::string& p_workingDirectory);
		~AudioEngine();

		void Update();

		void Suspend();
		void Unsuspend();
		bool IsSuspended() const;

		const std::string& GetWorkingDirectory() const;

		irrklang::ISoundEngine* GetIrrklangEngine() const;

		bool GetListenerInformation(glm::vec3& p_position, glm::vec3& p_direction, bool p_considerDisabled = false) const;

	private:
		void Consider(Entities::AudioSource& p_audioSource);
		void Consider(Entities::AudioListener& p_audioListener);

		void Unconsider(Entities::AudioSource& p_audioSource);
		void Unconsider(Entities::AudioListener& p_audioListener);

	private:
		const std::string m_workingDirectory;
		bool m_suspended = false;

		std::vector<std::reference_wrapper<Entities::AudioSource>> m_audioSources;
		std::vector<std::reference_wrapper<Entities::AudioSource>> m_suspendedAudioSources;
		std::vector<std::reference_wrapper<Entities::AudioListener>> m_audioListeners;

		irrklang::ISoundEngine* m_irrklangEngine = nullptr;
	};
}
