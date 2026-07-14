#include "Amberpch.h"

#include "AmberCore/Scripting/LuaActorBinder.h"

#include "AmberCore/ECS/Actor.h"

#include "AmberCore/ECS/Components/CTransform.h"
#include "AmberCore/ECS/Components/CCamera.h"
#include "AmberCore/ECS/Components/CPhysicalBox.h"
#include "AmberCore/ECS/Components/CPhysicalSphere.h"
#include "AmberCore/ECS/Components/CPhysicalCapsule.h"
#include "AmberCore/ECS/Components/CDirectionalLight.h"
#include "AmberCore/ECS/Components/CPointLight.h"
#include "AmberCore/ECS/Components/CSpotLight.h"
#include "AmberCore/ECS/Components/CAmbientBoxLight.h"
#include "AmberCore/ECS/Components/CAmbientSphereLight.h"
#include "AmberCore/ECS/Components/CModelRenderer.h"
#include "AmberCore/ECS/Components/CMaterialRenderer.h"
#include "AmberCore/ECS/Components/CAudioSource.h"
#include "AmberCore/ECS/Components/CAudioListener.h"

void AmberCore::Scripting::LuaActorBinder::BindActor(sol::state& p_luaState)
{
	p_luaState.new_usertype<ECS::Actor>("Actor",
		"GetName", &ECS::Actor::GetName,
		"SetName", &ECS::Actor::SetName,
		"GetTag", &ECS::Actor::GetTag,
		"SetTag", &ECS::Actor::SetTag,
		"GetChildren", &ECS::Actor::GetChildren,
		"GetID", &ECS::Actor::GetID,
		"GetParent", &ECS::Actor::GetParent,
		"SetParent", &ECS::Actor::SetParent,
		"RemoveParent", &ECS::Actor::RemoveParent,
		"Destroy", &ECS::Actor::MarkAsDestroy,
		"IsSelfActive", &ECS::Actor::IsSelfActive,
		"IsActive", &ECS::Actor::IsActive,
		"SetActive", &ECS::Actor::SetActive,

		"GetTransform", &ECS::Actor::GetComponent<ECS::Components::CTransform>,
		"GetPhysicalObject", &ECS::Actor::GetComponent<ECS::Components::CPhysicalObject>,
		"GetPhysicalBox", &ECS::Actor::GetComponent<ECS::Components::CPhysicalBox>,
		"GetPhysicalSphere", &ECS::Actor::GetComponent<ECS::Components::CPhysicalSphere>,
		"GetPhysicalCapsule", &ECS::Actor::GetComponent<ECS::Components::CPhysicalCapsule>,
		"GetCamera", &ECS::Actor::GetComponent<ECS::Components::CCamera>,
		"GetLight", &ECS::Actor::GetComponent<ECS::Components::CLight>,
		"GetPointLight", &ECS::Actor::GetComponent<ECS::Components::CPointLight>,
		"GetSpotLight", &ECS::Actor::GetComponent<ECS::Components::CSpotLight>,
		"GetDirectionalLight", &ECS::Actor::GetComponent<ECS::Components::CDirectionalLight>,
		"GetAmbientBoxLight", &ECS::Actor::GetComponent<ECS::Components::CAmbientBoxLight>,
		"GetAmbientSphereLight", &ECS::Actor::GetComponent<ECS::Components::CAmbientSphereLight>,
		"GetModelRenderer", &ECS::Actor::GetComponent<ECS::Components::CModelRenderer>,
		"GetMaterialRenderer", &ECS::Actor::GetComponent<ECS::Components::CMaterialRenderer>,
		"GetAudioSource", &ECS::Actor::GetComponent<ECS::Components::CAudioSource>,
		"GetAudioListener", &ECS::Actor::GetComponent<ECS::Components::CAudioListener>,

		"GetBehaviour", [](ECS::Actor& p_this, const std::string& p_name)
		{
			sol::table result = sol::nil;

			auto behaviour = p_this.GetBehaviour(p_name);

			if (behaviour)
			{
				result = behaviour->GetTable();
			}

			return result;
		},

		"AddTransform", &ECS::Actor::AddComponent<ECS::Components::CTransform>,
		"AddModelRenderer", &ECS::Actor::AddComponent<ECS::Components::CModelRenderer>,
		"AddPhysicalBox", &ECS::Actor::AddComponent<ECS::Components::CPhysicalBox>,
		"AddPhysicalSphere", &ECS::Actor::AddComponent<ECS::Components::CPhysicalSphere>,
		"AddPhysicalCapsule", &ECS::Actor::AddComponent<ECS::Components::CPhysicalCapsule>,
		"AddCamera", &ECS::Actor::AddComponent<ECS::Components::CCamera>,
		"AddPointLight", &ECS::Actor::AddComponent<ECS::Components::CPointLight>,
		"AddSpotLight", &ECS::Actor::AddComponent<ECS::Components::CSpotLight>,
		"AddDirectionalLight", &ECS::Actor::AddComponent<ECS::Components::CDirectionalLight>,
		"AddAmbientBoxLight", &ECS::Actor::AddComponent<ECS::Components::CAmbientBoxLight>,
		"AddAmbientSphereLight", &ECS::Actor::AddComponent<ECS::Components::CAmbientSphereLight>,
		"AddMaterialRenderer", &ECS::Actor::AddComponent<ECS::Components::CMaterialRenderer>,
		"AddAudioSource", &ECS::Actor::AddComponent<ECS::Components::CAudioSource>,
		"AddAudioListener", &ECS::Actor::AddComponent<ECS::Components::CAudioListener>,

		"RemoveModelRenderer", &ECS::Actor::RemoveComponent<ECS::Components::CModelRenderer>,
		"RemovePhysicalBox", &ECS::Actor::RemoveComponent<ECS::Components::CPhysicalBox>,
		"RemovePhysicalSphere", &ECS::Actor::RemoveComponent<ECS::Components::CPhysicalSphere>,
		"RemovePhysicalCapsule", &ECS::Actor::RemoveComponent<ECS::Components::CPhysicalCapsule>,
		"RemoveCamera", &ECS::Actor::RemoveComponent<ECS::Components::CCamera>,
		"RemovePointLight", &ECS::Actor::RemoveComponent<ECS::Components::CPointLight>,
		"RemoveSpotLight", &ECS::Actor::RemoveComponent<ECS::Components::CSpotLight>,
		"RemoveDirectionalLight", &ECS::Actor::RemoveComponent<ECS::Components::CDirectionalLight>,
		"RemoveAmbientBoxLight", &ECS::Actor::RemoveComponent<ECS::Components::CAmbientBoxLight>,
		"RemoveAmbientSphereLight", &ECS::Actor::RemoveComponent<ECS::Components::CAmbientSphereLight>,
		"RemoveMaterialRenderer", &ECS::Actor::RemoveComponent<ECS::Components::CMaterialRenderer>,
		"RemoveAudioSource", &ECS::Actor::RemoveComponent<ECS::Components::CAudioSource>,
		"RemoveAudioListener", &ECS::Actor::RemoveComponent<ECS::Components::CAudioListener>,

		"AddBehaviour", &ECS::Actor::AddBehaviour,
		"RemoveBehaviour", sol::overload
		(
			sol::resolve<bool(ECS::Components::Behaviour&)>(&ECS::Actor::RemoveBehaviour),
			sol::resolve<bool(const std::string&)>(&ECS::Actor::RemoveBehaviour)
		)
	);
}
