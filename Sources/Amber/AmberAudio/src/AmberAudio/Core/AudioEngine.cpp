#include "Amberpch.h"

#include <irrklang/irrKlang.h>

#include "AmberAudio/Core/AudioEngine.h"

AmberAudio::Core::AudioEngine::AudioEngine(const std::string& p_workingDirectory) : m_workingDirectory(p_workingDirectory)
{
	m_irrklangEngine = irrklang::createIrrKlangDevice();

	using AudioSourceReceiver = void(AudioEngine::*)(Entities::AudioSource&);
	using AudioListenerReceiver = void(AudioEngine::*)(Entities::AudioListener&);

	Entities::AudioSource::CreatedEvent += std::bind(static_cast<AudioSourceReceiver>(&AudioEngine::Consider), this, std::placeholders::_1);
	Entities::AudioSource::DestroyedEvent += std::bind(static_cast<AudioSourceReceiver>(&AudioEngine::Unconsider), this, std::placeholders::_1);
	Entities::AudioListener::CreatedEvent += std::bind(static_cast<AudioListenerReceiver>(&AudioEngine::Consider), this, std::placeholders::_1);
	Entities::AudioListener::DestroyedEvent += std::bind(static_cast<AudioListenerReceiver>(&AudioEngine::Unconsider), this, std::placeholders::_1);
}

AmberAudio::Core::AudioEngine::~AudioEngine()
{
	m_irrklangEngine->drop();
}

void AmberAudio::Core::AudioEngine::Update()
{
	std::for_each(m_audioSources.begin(), m_audioSources.end(), std::mem_fn(&Entities::AudioSource::UpdateTrackedSoundPosition));

	glm::vec3 listenerPosition;
	glm::vec3 listenerDirection;

	if (GetListenerInformation(listenerPosition, listenerDirection))
	{
		m_irrklangEngine->setListenerPosition
		(
			reinterpret_cast<const irrklang::vec3df&>(listenerPosition),
			reinterpret_cast<const irrklang::vec3df&>(listenerDirection)
		);
	}
	else
	{
		m_irrklangEngine->setListenerPosition({ 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, -1.0f });
	}
}

void AmberAudio::Core::AudioEngine::Suspend()
{
	std::for_each(m_audioSources.begin(), m_audioSources.end(), [this](std::reference_wrapper<Entities::AudioSource> p_audioSource)
	{
		if (p_audioSource.get().IsTrackingSound() && !p_audioSource.get().GetTrackedSound()->GetTrack()->getIsPaused())
		{
			m_suspendedAudioSources.push_back(p_audioSource);
			p_audioSource.get().Pause();
		}
	});

	m_suspended = true;
}

void AmberAudio::Core::AudioEngine::Unsuspend()
{
	std::for_each(m_suspendedAudioSources.begin(), m_suspendedAudioSources.end(), std::mem_fn(&Entities::AudioSource::Resume));
	m_suspendedAudioSources.clear();
	m_suspended = false;
}

bool AmberAudio::Core::AudioEngine::IsSuspended() const
{
	return m_suspended;
}

const std::string& AmberAudio::Core::AudioEngine::GetWorkingDirectory() const
{
	return m_workingDirectory;
}

irrklang::ISoundEngine* AmberAudio::Core::AudioEngine::GetIrrklangEngine() const
{
	return m_irrklangEngine;
}

bool AmberAudio::Core::AudioEngine::GetListenerInformation(glm::vec3& p_position, glm::vec3& p_direction, bool p_considerDisabled) const
{
	for (auto listener : m_audioListeners)
	{
		if (listener.get().IsEnabled() || p_considerDisabled)
		{
			auto& transform = m_audioListeners.back().get().GetTransform();

			p_position = transform.GetWorldPosition();
			p_direction = transform.GetWorldForward() * -1.0f;

			return true;
		}
	}

	return false;
}

void AmberAudio::Core::AudioEngine::Consider(Entities::AudioSource& p_audioSource)
{
	m_audioSources.push_back(std::ref(p_audioSource));
}

void AmberAudio::Core::AudioEngine::Consider(Entities::AudioListener& p_audioListener)
{
	m_audioListeners.push_back(std::ref(p_audioListener));
}

void AmberAudio::Core::AudioEngine::Unconsider(Entities::AudioSource& p_audioSource)
{
	auto found = std::find_if(m_audioSources.begin(), m_audioSources.end(), [&p_audioSource](std::reference_wrapper<Entities::AudioSource> element)
	{
		return std::addressof(p_audioSource) == std::addressof(element.get());
	});

	if (found != m_audioSources.end())
		m_audioSources.erase(found);

	if (m_suspended)
	{
		auto suspendedFound = std::find_if(m_suspendedAudioSources.begin(), m_suspendedAudioSources.end(), [&p_audioSource](std::reference_wrapper<Entities::AudioSource> element)
		{
			return std::addressof(p_audioSource) == std::addressof(element.get());
		});

		if (suspendedFound != m_suspendedAudioSources.end())
			m_suspendedAudioSources.erase(suspendedFound);
	}
}

void AmberAudio::Core::AudioEngine::Unconsider(Entities::AudioListener& p_audioListener)
{
	auto found = std::find_if(m_audioListeners.begin(), m_audioListeners.end(), [&p_audioListener](std::reference_wrapper<Entities::AudioListener> element)
	{
		return std::addressof(p_audioListener) == std::addressof(element.get());
	});

	if (found != m_audioListeners.end())
		m_audioListeners.erase(found);
}
