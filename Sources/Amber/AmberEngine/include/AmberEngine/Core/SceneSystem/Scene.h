#pragma once

#include "AmberEngine/API/Export.h"
#include "AmberEngine/Core/API/ISerializable.h"
#include "AmberEngine/Core/ECS/Components/CCamera.h"
#include "AmberEngine/Tools/FileSystem/tinyxml2.h"

namespace AmberEngine::Core::ECS
{
	class Actor;
}

namespace AmberEngine::Core::ECS::Components
{
	class AComponent;
	class CLight;
	class CModelRenderer;

}

namespace AmberEngine::Core::SceneSystem
{
	class API_AMBERENGINE Scene : public API::ISerializable
	{
	public:
		static Eventing::Event<bool> DirtyEvent;
	public:
		struct FastAccessComponents
		{
			std::vector<ECS::Components::CModelRenderer*> modelRenderers;
			std::vector<ECS::Components::CCamera*>        cameras;
			std::vector<ECS::Components::CLight*>         lights;
		};

		Scene() = default;
		~Scene();

		void Play();
		bool IsPlaying() const;
		void Update(float p_deltaTime);
		void FixedUpdate(float p_deltaTime);
		void LateUpdate(float p_deltaTime);
		void AddActor(ECS::Actor* p_actor);
		ECS::Actor& CreateActor();
		ECS::Actor& CreateActor(const std::string& p_name, const std::string& p_tag = "");
		bool DestroyActor(ECS::Actor& p_target);
		ECS::Actor* FindActorByID(int64_t p_id);
		void CollectGarbage();
		void SetDirty(bool p_isDirty);
		void OnComponentAdded(ECS::Components::AComponent& p_compononent);
		void OnComponentRemoved(ECS::Components::AComponent& p_compononent);

		std::vector<ECS::Actor*>& GetActors();

		const FastAccessComponents& GetFastAccessComponents() const;

		virtual void OnSerialize(tinyxml2::XMLDocument& p_doc, tinyxml2::XMLNode* p_node) override;
		virtual void OnDeserialize(tinyxml2::XMLDocument& p_doc, tinyxml2::XMLNode* p_node) override;

	private:
		int64_t m_availableID = 1;
		std::vector<ECS::Actor*> m_actors;
		FastAccessComponents m_fastAccessComponents;
		bool m_isDirty;
		bool m_isPlaying = false;
	};
}
