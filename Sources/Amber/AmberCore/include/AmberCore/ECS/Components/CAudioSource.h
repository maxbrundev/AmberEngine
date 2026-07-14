#pragma once

#include "AmberAudio/Entities/AudioSource.h"

#include "AmberCore/ECS/Components/AComponent.h"

namespace AmberCore::ECS { class Actor; }

namespace AmberCore::ECS::Components
{
	class CAudioSource : public AComponent
	{
	public:
		CAudioSource(Actor& p_owner);

		std::string GetName() override;

		void SetSound(AmberAudio::Resources::Sound* p_sound);
		void SetAutoplay(bool p_autoplay);
		void SetVolume(float p_volume);
		void SetPan(float p_pan);
		void SetLooped(bool p_looped);
		void SetPitch(float p_pitch);
		void SetSpatial(bool p_value);
		void SetAttenuationThreshold(float p_distance);

		AmberAudio::Resources::Sound* GetSound() const;
		bool IsAutoplayed() const;
		float GetVolume() const;
		float GetPan() const;
		bool IsLooped() const;
		float GetPitch() const;
		bool IsFinished() const;
		bool IsSpatial() const;
		float GetAttenuationThreshold() const;

		void Play();
		void Resume();
		void Pause();
		void Stop();

		virtual void OnSerialize(tinyxml2::XMLDocument& p_doc, tinyxml2::XMLNode* p_node) override;
		virtual void OnDeserialize(tinyxml2::XMLDocument& p_doc, tinyxml2::XMLNode* p_node) override;
		virtual void OnInspector(AmberUI::WidgetContainer& p_root) override;
		virtual void OnEnable() override;
		virtual void OnDisable() override;

	private:
		AmberAudio::Resources::Sound* m_sound = nullptr;
		AmberAudio::Entities::AudioSource m_audioSource;
		bool m_autoPlay = false;
	};
}
