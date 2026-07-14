#include "Amberpch.h"

#include "AmberAudio/Entities/AudioListener.h"

AmberTools::Eventing::Event<AmberAudio::Entities::AudioListener&> AmberAudio::Entities::AudioListener::CreatedEvent;
AmberTools::Eventing::Event<AmberAudio::Entities::AudioListener&> AmberAudio::Entities::AudioListener::DestroyedEvent;

AmberAudio::Entities::AudioListener::AudioListener() :
	m_transform(new AmberMaths::Transform()),
	m_internalTransform(true)
{
	Setup();
}

AmberAudio::Entities::AudioListener::AudioListener(AmberMaths::Transform& p_transform) :
	m_transform(&p_transform),
	m_internalTransform(false)
{
	Setup();
}

AmberAudio::Entities::AudioListener::~AudioListener()
{
	DestroyedEvent.Invoke(*this);

	if (m_internalTransform)
		delete m_transform;
}

AmberMaths::Transform& AmberAudio::Entities::AudioListener::GetTransform()
{
	return *m_transform;
}

void AmberAudio::Entities::AudioListener::SetEnabled(bool p_enable)
{
	m_enabled = p_enable;
}

bool AmberAudio::Entities::AudioListener::IsEnabled() const
{
	return m_enabled;
}

void AmberAudio::Entities::AudioListener::Setup()
{
	CreatedEvent.Invoke(*this);
}
