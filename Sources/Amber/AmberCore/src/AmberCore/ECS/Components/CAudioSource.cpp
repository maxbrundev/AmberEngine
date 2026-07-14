#include "Amberpch.h"

#include "AmberCore/ECS/Components/CAudioSource.h"

#include "AmberAudio/Core/AudioPlayer.h"
#include "AmberCore/ECS/Actor.h"
#include "AmberCore/Helpers/GUIDrawer.h"
#include "AmberCore/Helpers/Serializer.h"
#include "AmberCore/SceneSystem/Scene.h"
#include "AmberCore/SceneSystem/SceneManager.h"
#include "AmberTools/Global/ServiceLocator.h"
#include "AmberUI/Widgets/PlotLines.h"

AmberCore::ECS::Components::CAudioSource::CAudioSource(Actor& p_owner) :
	AComponent(p_owner),
	m_audioSource(AmberTools::Global::ServiceLocator::Get<AmberAudio::Core::AudioPlayer>(), Owner.transform.GetTransform())
{
}

std::string AmberCore::ECS::Components::CAudioSource::GetName()
{
	return "Audio Source";
}

void AmberCore::ECS::Components::CAudioSource::SetSound(AmberAudio::Resources::Sound* p_sound)
{
	m_sound = p_sound;
}

void AmberCore::ECS::Components::CAudioSource::SetAutoplay(bool p_autoplay)
{
	m_autoPlay = p_autoplay;
}

void AmberCore::ECS::Components::CAudioSource::SetVolume(float p_volume)
{
	m_audioSource.SetVolume(p_volume);
}

void AmberCore::ECS::Components::CAudioSource::SetPan(float p_pan)
{
	m_audioSource.SetPan(p_pan);
}

void AmberCore::ECS::Components::CAudioSource::SetLooped(bool p_looped)
{
	m_audioSource.SetLooped(p_looped);
}

void AmberCore::ECS::Components::CAudioSource::SetPitch(float p_pitch)
{
	m_audioSource.SetPitch(p_pitch);
}

void AmberCore::ECS::Components::CAudioSource::SetSpatial(bool p_value)
{
	m_audioSource.SetSpatial(p_value);
}

void AmberCore::ECS::Components::CAudioSource::SetAttenuationThreshold(float p_distance)
{
	m_audioSource.SetAttenuationThreshold(p_distance);
}

AmberAudio::Resources::Sound* AmberCore::ECS::Components::CAudioSource::GetSound() const
{
	return m_sound;
}

bool AmberCore::ECS::Components::CAudioSource::IsAutoplayed() const
{
	return m_autoPlay;
}

float AmberCore::ECS::Components::CAudioSource::GetVolume() const
{
	return m_audioSource.GetVolume();
}

float AmberCore::ECS::Components::CAudioSource::GetPan() const
{
	return m_audioSource.GetPan();
}

bool AmberCore::ECS::Components::CAudioSource::IsLooped() const
{
	return m_audioSource.IsLooped();
}

float AmberCore::ECS::Components::CAudioSource::GetPitch() const
{
	return m_audioSource.GetPitch();
}

bool AmberCore::ECS::Components::CAudioSource::IsFinished() const
{
	return m_audioSource.IsFinished();
}

bool AmberCore::ECS::Components::CAudioSource::IsSpatial() const
{
	return m_audioSource.IsSpatial();
}

float AmberCore::ECS::Components::CAudioSource::GetAttenuationThreshold() const
{
	return m_audioSource.GetAttenuationThreshold();
}

void AmberCore::ECS::Components::CAudioSource::Play()
{
	if (Owner.IsActive() && m_sound)
		m_audioSource.Play(*m_sound);
}

void AmberCore::ECS::Components::CAudioSource::Resume()
{
	if (Owner.IsActive())
		m_audioSource.Resume();
}

void AmberCore::ECS::Components::CAudioSource::Pause()
{
	if (Owner.IsActive())
		m_audioSource.Pause();
}

void AmberCore::ECS::Components::CAudioSource::Stop()
{
	if (Owner.IsActive())
		m_audioSource.Stop();
}

void AmberCore::ECS::Components::CAudioSource::OnSerialize(tinyxml2::XMLDocument& p_doc, tinyxml2::XMLNode* p_node)
{
	AmberCore::Helpers::Serializer::SerializeBoolean(p_doc, p_node, "autoplay", m_autoPlay);
	AmberCore::Helpers::Serializer::SerializeBoolean(p_doc, p_node, "spatial", IsSpatial());
	AmberCore::Helpers::Serializer::SerializeFloat(p_doc, p_node, "volume", GetVolume());
	AmberCore::Helpers::Serializer::SerializeFloat(p_doc, p_node, "pan", GetPan());
	AmberCore::Helpers::Serializer::SerializeBoolean(p_doc, p_node, "looped", IsLooped());
	AmberCore::Helpers::Serializer::SerializeFloat(p_doc, p_node, "pitch", GetPitch());
	AmberCore::Helpers::Serializer::SerializeFloat(p_doc, p_node, "attenuation_threshold", GetAttenuationThreshold());
	AmberCore::Helpers::Serializer::SerializeSound(p_doc, p_node, "audio_clip", m_sound);
}

void AmberCore::ECS::Components::CAudioSource::OnDeserialize(tinyxml2::XMLDocument& p_doc, tinyxml2::XMLNode* p_node)
{
	AmberCore::Helpers::Serializer::DeserializeBoolean(p_doc, p_node, "autoplay", m_autoPlay);
	SetSpatial(AmberCore::Helpers::Serializer::DeserializeBoolean(p_doc, p_node, "spatial"));
	SetVolume(AmberCore::Helpers::Serializer::DeserializeFloat(p_doc, p_node, "volume"));
	SetPan(AmberCore::Helpers::Serializer::DeserializeFloat(p_doc, p_node, "pan"));
	SetLooped(AmberCore::Helpers::Serializer::DeserializeBoolean(p_doc, p_node, "looped"));
	SetPitch(AmberCore::Helpers::Serializer::DeserializeFloat(p_doc, p_node, "pitch"));
	SetAttenuationThreshold(AmberCore::Helpers::Serializer::DeserializeFloat(p_doc, p_node, "attenuation_threshold"));
	AmberCore::Helpers::Serializer::DeserializeSound(p_doc, p_node, "audio_clip", m_sound);
}

void AmberCore::ECS::Components::CAudioSource::OnInspector(AmberUI::WidgetContainer& p_root)
{
	AmberCore::Helpers::GUIDrawer::DrawSound(p_root, "Sound", m_sound);
	AmberCore::Helpers::GUIDrawer::DrawBoolean(p_root, "Auto-play", m_autoPlay);
	AmberCore::Helpers::GUIDrawer::DrawScalar<float>(p_root, "Volume", std::bind(&CAudioSource::GetVolume, this), std::bind(&CAudioSource::SetVolume, this, std::placeholders::_1), 0.01f, 0.0f, 1.0f);
	AmberCore::Helpers::GUIDrawer::DrawScalar<float>(p_root, "Pan", std::bind(&CAudioSource::GetPan, this), std::bind(&CAudioSource::SetPan, this, std::placeholders::_1), 0.01f, -1.0f, 1.0f);
	AmberCore::Helpers::GUIDrawer::DrawBoolean(p_root, "Looped", std::bind(&CAudioSource::IsLooped, this), std::bind(&CAudioSource::SetLooped, this, std::placeholders::_1));
	AmberCore::Helpers::GUIDrawer::DrawScalar<float>(p_root, "Pitch", std::bind(&CAudioSource::GetPitch, this), std::bind(&CAudioSource::SetPitch, this, std::placeholders::_1), 0.01f, 0.01f, 10000.0f);
	AmberCore::Helpers::GUIDrawer::DrawBoolean(p_root, "Spatial", std::bind(&CAudioSource::IsSpatial, this), std::bind(&CAudioSource::SetSpatial, this, std::placeholders::_1));
	AmberCore::Helpers::GUIDrawer::DrawScalar<float>(p_root, "Attenuation threshold", std::bind(&CAudioSource::GetAttenuationThreshold, this), std::bind(&CAudioSource::SetAttenuationThreshold, this, std::placeholders::_1), 0.5f);

	AmberCore::Helpers::GUIDrawer::CreateTitle(p_root, "Spatial graph");
	auto& graph = p_root.CreateWidget<AmberUI::Widgets::PlotLines>(std::vector<float>(), -0.1f, 1.1f);
	graph.Size.y = 75.0f;

	graph.SetGathererAndProvider([this, &graph]() -> std::vector<float>
	{
		std::vector<float> result;

		glm::vec3 listenerPosition(0.0f, 0.0f, 0.0f);
		glm::vec3 listenerDirection(0.0f, 0.0f, 0.0f);

		bool playMode = AmberTools::Global::ServiceLocator::Get<AmberCore::SceneSystem::SceneManager>().GetCurrentScene()->IsPlaying();

		AmberTools::Global::ServiceLocator::Get<AmberAudio::Core::AudioEngine>().GetListenerInformation(listenerPosition, listenerDirection, !playMode);

		float distanceToListener = glm::distance(listenerPosition, Owner.transform.GetWorldPosition());

		for (float graphX = 0.0f; graphX < 50.0f; graphX += 0.25f)
		{
			float graphY = graphX < m_audioSource.GetAttenuationThreshold() ? 1.0f : 1.0f / (1.0f + 1.0f * (graphX - m_audioSource.GetAttenuationThreshold()));

			if (abs(graphX - distanceToListener) <= 0.25f)
			{
				graph.ForceHover = static_cast<int>(graphX * 4.0f);
				graph.Overlay = std::to_string(static_cast<int>(graphY * 100.0f)) + "%";
			}

			result.push_back(graphY);
		}

		return result;
	}, nullptr);
}

void AmberCore::ECS::Components::CAudioSource::OnEnable()
{
	if (m_autoPlay)
		Play();
}

void AmberCore::ECS::Components::CAudioSource::OnDisable()
{
	m_audioSource.Stop();
}
