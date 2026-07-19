#include "Amberpch.h"

#include "AmberCore/Scripting/LuaComponentBinder.h"

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

void AmberCore::Scripting::LuaComponentBinder::BindComponent(sol::state& p_luaState)
{
	p_luaState.new_usertype<ECS::Components::AComponent>("Component",
		"GetOwner", [](ECS::Components::AComponent& p_component) { return &p_component.Owner; }
	);

	p_luaState.new_usertype<ECS::Components::CTransform>("Transform",
		sol::base_classes, sol::bases<ECS::Components::AComponent>(),
		"SetPosition", &ECS::Components::CTransform::SetLocalPosition,
		"SetRotation", &ECS::Components::CTransform::SetLocalRotation,
		"SetScale", &ECS::Components::CTransform::SetLocalScale,
		"SetLocalPosition", &ECS::Components::CTransform::SetLocalPosition,
		"SetLocalRotation", &ECS::Components::CTransform::SetLocalRotation,
		"SetLocalScale", &ECS::Components::CTransform::SetLocalScale,
		"GetPosition", [](ECS::Components::CTransform& p_this) { return glm::vec3(p_this.GetWorldPosition()); },
		"GetRotation", [](ECS::Components::CTransform& p_this) { return glm::quat(p_this.GetWorldRotation()); },
		"GetScale", [](ECS::Components::CTransform& p_this) { return glm::vec3(p_this.GetWorldScale()); },
		"GetLocalPosition", [](ECS::Components::CTransform& p_this) { return glm::vec3(p_this.GetLocalPosition()); },
		"GetLocalRotation", [](ECS::Components::CTransform& p_this) { return glm::quat(p_this.GetLocalRotation()); },
		"GetLocalScale", [](ECS::Components::CTransform& p_this) { return glm::vec3(p_this.GetLocalScale()); },
		"GetWorldPosition", [](ECS::Components::CTransform& p_this) { return glm::vec3(p_this.GetWorldPosition()); },
		"GetWorldRotation", [](ECS::Components::CTransform& p_this) { return glm::quat(p_this.GetWorldRotation()); },
		"GetWorldScale", [](ECS::Components::CTransform& p_this) { return glm::vec3(p_this.GetWorldScale()); },
		"GetForward", &ECS::Components::CTransform::GetWorldForward,
		"GetUp", &ECS::Components::CTransform::GetWorldUp,
		"GetRight", &ECS::Components::CTransform::GetWorldRight,
		"GetLocalForward", &ECS::Components::CTransform::GetLocalForward,
		"GetLocalUp", &ECS::Components::CTransform::GetLocalUp,
		"GetLocalRight", &ECS::Components::CTransform::GetLocalRight,
		"GetWorldForward", &ECS::Components::CTransform::GetWorldForward,
		"GetWorldUp", &ECS::Components::CTransform::GetWorldUp,
		"GetWorldRight", &ECS::Components::CTransform::GetWorldRight
		);

	p_luaState.new_enum<ECS::Components::CModelRenderer::EFrustumBehaviour>("FrustumBehaviour",
		{
			{"DISABLED", ECS::Components::CModelRenderer::EFrustumBehaviour::DISABLED},
			{"CULL_MODEL", ECS::Components::CModelRenderer::EFrustumBehaviour::CULL_MODEL},
			{"CULL_MESHES", ECS::Components::CModelRenderer::EFrustumBehaviour::CULL_MESHES},
			{"CULL_CUSTOM", ECS::Components::CModelRenderer::EFrustumBehaviour::CULL_CUSTOM}
		});

	p_luaState.new_usertype<ECS::Components::CModelRenderer>("ModelRenderer",
		sol::base_classes, sol::bases<ECS::Components::AComponent>(),
		"GetModel", &ECS::Components::CModelRenderer::GetModel,
		"SetModel", &ECS::Components::CModelRenderer::SetModel,
		"GetFrustumBehaviour", &ECS::Components::CModelRenderer::GetFrustumBehaviour,
		"SetFrustumBehaviour", &ECS::Components::CModelRenderer::SetFrustumBehaviour
		);

	p_luaState.new_usertype<ECS::Components::CMaterialRenderer>("MaterialRenderer",
		sol::base_classes, sol::bases<ECS::Components::AComponent>(),
		"SetMaterial", &ECS::Components::CMaterialRenderer::SetMaterialAtIndex,
		"SetUserMatrixElement", &ECS::Components::CMaterialRenderer::SetUserMatrixElement,
		"GetUserMatrixElement", &ECS::Components::CMaterialRenderer::GetUserMatrixElement
	);

	p_luaState.new_enum<AmberPhysics::Entities::PhysicalObject::ECollisionDetectionMode>("CollisionDetectionMode",
		{
			{"DISCRETE", AmberPhysics::Entities::PhysicalObject::ECollisionDetectionMode::DISCRETE},
			{"CONTINUOUS", AmberPhysics::Entities::PhysicalObject::ECollisionDetectionMode::CONTINUOUS}
		});

	p_luaState.new_usertype<ECS::Components::CPhysicalObject>("PhysicalObject",
		sol::base_classes, sol::bases<ECS::Components::AComponent>(),
		"GetMass", &ECS::Components::CPhysicalObject::GetMass,
		"SetMass", &ECS::Components::CPhysicalObject::SetMass,
		"GetFriction", &ECS::Components::CPhysicalObject::GetFriction,
		"SetFriction", &ECS::Components::CPhysicalObject::SetFriction,
		"GetBounciness", &ECS::Components::CPhysicalObject::GetBounciness,
		"SetBounciness", &ECS::Components::CPhysicalObject::SetBounciness,
		"SetLinearVelocity", &ECS::Components::CPhysicalObject::SetLinearVelocity,
		"SetAngularVelocity", &ECS::Components::CPhysicalObject::SetAngularVelocity,
		"GetLinearVelocity", &ECS::Components::CPhysicalObject::GetLinearVelocity,
		"GetAngularVelocity", &ECS::Components::CPhysicalObject::GetAngularVelocity,
		"SetLinearFactor", &ECS::Components::CPhysicalObject::SetLinearFactor,
		"SetAngularFactor", &ECS::Components::CPhysicalObject::SetAngularFactor,
		"GetLinearFactor", &ECS::Components::CPhysicalObject::GetLinearFactor,
		"GetAngularFactor", &ECS::Components::CPhysicalObject::GetAngularFactor,
		"IsTrigger", &ECS::Components::CPhysicalObject::IsTrigger,
		"SetTrigger", &ECS::Components::CPhysicalObject::SetTrigger,
		"AddForce", &ECS::Components::CPhysicalObject::AddForce,
		"AddImpulse", &ECS::Components::CPhysicalObject::AddImpulse,
		"ClearForces", &ECS::Components::CPhysicalObject::ClearForces,
		"SetCollisionDetectionMode", &ECS::Components::CPhysicalObject::SetCollisionDetectionMode,
		"GetCollisionMode", &ECS::Components::CPhysicalObject::GetCollisionDetectionMode,
		"SetKinematic", &ECS::Components::CPhysicalObject::SetKinematic
		);

	p_luaState.new_usertype<ECS::Components::CPhysicalBox>("PhysicalBox",
		sol::base_classes, sol::bases<ECS::Components::CPhysicalObject>(),
		"GetSize", &ECS::Components::CPhysicalBox::GetSize,
		"SetSize", &ECS::Components::CPhysicalBox::SetSize
		);

	p_luaState.new_usertype<ECS::Components::CPhysicalSphere>("PhysicalSphere",
		sol::base_classes, sol::bases<ECS::Components::CPhysicalObject>(),
		"GetRadius", &ECS::Components::CPhysicalSphere::GetRadius,
		"SetRadius", &ECS::Components::CPhysicalSphere::SetRadius
		);

	p_luaState.new_usertype<ECS::Components::CPhysicalCapsule>("PhysicalCapsule",
		sol::base_classes, sol::bases<ECS::Components::CPhysicalObject>(),
		"GetRadius", &ECS::Components::CPhysicalCapsule::GetRadius,
		"SetRadius", &ECS::Components::CPhysicalCapsule::SetRadius,
		"GetHeight", &ECS::Components::CPhysicalCapsule::GetHeight,
		"SetHeight", &ECS::Components::CPhysicalCapsule::SetHeight
		);

	p_luaState.new_usertype<ECS::Components::CCamera>("Camera",
		sol::base_classes, sol::bases<ECS::Components::AComponent>(),
		"GetFov", &ECS::Components::CCamera::GetFov,
		"GetNear", &ECS::Components::CCamera::GetNear,
		"GetFar", &ECS::Components::CCamera::GetFar,
		"GetClearColor", [](ECS::Components::CCamera& p_this) { return glm::vec3(p_this.GetClearColor()); },
		"SetFov", &ECS::Components::CCamera::SetFov,
		"SetNear", &ECS::Components::CCamera::SetNear,
		"SetFar", &ECS::Components::CCamera::SetFar,
		"SetClearColor", [](ECS::Components::CCamera& p_this, const glm::vec3& p_color) { p_this.SetClearColor(p_color); },
		"HasFrustumGeometryCulling", &ECS::Components::CCamera::HasFrustumGeometryCulling,
		"HasFrustumLightCulling", &ECS::Components::CCamera::HasFrustumLightCulling,
		"SetFrustumGeometryCulling", &ECS::Components::CCamera::SetFrustumGeometryCulling,
		"SetFrustumLightCulling", &ECS::Components::CCamera::SetFrustumLightCulling
		);

	p_luaState.new_usertype<ECS::Components::CLight>("Light",
		sol::base_classes, sol::bases<ECS::Components::AComponent>(),
		"GetColor", [](ECS::Components::CLight& p_this) { const AmberRendering::Data::Color& color = p_this.GetColor(); return glm::vec3(color.r, color.g, color.b); },
		"GetIntensity", &ECS::Components::CLight::GetIntensity,
		"SetColor", [](ECS::Components::CLight& p_this, const glm::vec3& p_color) { p_this.SetColor(AmberRendering::Data::Color(p_color)); },
		"SetIntensity", &ECS::Components::CLight::SetIntensity
		);

	p_luaState.new_usertype<ECS::Components::CPointLight>("PointLight",
		sol::base_classes, sol::bases<ECS::Components::CLight>(),
		"GetConstant", &ECS::Components::CPointLight::GetConstant,
		"GetLinear", &ECS::Components::CPointLight::GetLinear,
		"GetQuadratic", &ECS::Components::CPointLight::GetQuadratic,
		"SetConstant", &ECS::Components::CPointLight::SetConstant,
		"SetLinear", &ECS::Components::CPointLight::SetLinear,
		"SetQuadratic", &ECS::Components::CPointLight::SetQuadratic
		);

	p_luaState.new_usertype<ECS::Components::CSpotLight>("SpotLight",
		sol::base_classes, sol::bases<ECS::Components::CLight>(),
		"GetConstant", &ECS::Components::CSpotLight::GetConstant,
		"GetLinear", &ECS::Components::CSpotLight::GetLinear,
		"GetQuadratic", &ECS::Components::CSpotLight::GetQuadratic,
		"GetCutOff", &ECS::Components::CSpotLight::GetCutoff,
		"GetOuterCutOff", &ECS::Components::CSpotLight::GetOuterCutoff,
		"SetConstant", &ECS::Components::CSpotLight::SetConstant,
		"SetLinear", &ECS::Components::CSpotLight::SetLinear,
		"SetQuadratic", &ECS::Components::CSpotLight::SetQuadratic,
		"SetCutOff", &ECS::Components::CSpotLight::SetCutoff,
		"SetOuterCutOff", &ECS::Components::CSpotLight::SetOuterCutoff
		);

	p_luaState.new_usertype<ECS::Components::CAmbientBoxLight>("AmbientBoxLight",
		sol::base_classes, sol::bases<ECS::Components::CLight>(),
		"GetSize", &ECS::Components::CAmbientBoxLight::GetSize,
		"SetSize", &ECS::Components::CAmbientBoxLight::SetSize
		);

	p_luaState.new_usertype<ECS::Components::CAmbientSphereLight>("AmbientSphereLight",
		sol::base_classes, sol::bases<ECS::Components::CLight>(),
		"GetRadius", &ECS::Components::CAmbientSphereLight::GetRadius,
		"SetRadius", &ECS::Components::CAmbientSphereLight::SetRadius
		);

	p_luaState.new_usertype<ECS::Components::CDirectionalLight>("DirectionalLight",
		sol::base_classes, sol::bases<ECS::Components::CLight>()
		);

	p_luaState.new_usertype<ECS::Components::CAudioSource>("AudioSource",
		sol::base_classes, sol::bases<ECS::Components::AComponent>(),
		"Play", &ECS::Components::CAudioSource::Play,
		"Stop", &ECS::Components::CAudioSource::Stop,
		"Pause", &ECS::Components::CAudioSource::Pause,
		"Resume", &ECS::Components::CAudioSource::Resume,
		"GetSound", &ECS::Components::CAudioSource::GetSound,
		"GetVolume", &ECS::Components::CAudioSource::GetVolume,
		"GetPan", &ECS::Components::CAudioSource::GetPan,
		"IsLooped", &ECS::Components::CAudioSource::IsLooped,
		"GetPitch", &ECS::Components::CAudioSource::GetPitch,
		"IsFinished", &ECS::Components::CAudioSource::IsFinished,
		"IsSpatial", &ECS::Components::CAudioSource::IsSpatial,
		"GetAttenuationThreshold", &ECS::Components::CAudioSource::GetAttenuationThreshold,
		"SetSound", &ECS::Components::CAudioSource::SetSound,
		"SetVolume", &ECS::Components::CAudioSource::SetVolume,
		"SetPan", &ECS::Components::CAudioSource::SetPan,
		"SetLooped", &ECS::Components::CAudioSource::SetLooped,
		"SetPitch", &ECS::Components::CAudioSource::SetPitch,
		"SetSpatial", &ECS::Components::CAudioSource::SetSpatial,
		"SetAttenuationThreshold", &ECS::Components::CAudioSource::SetAttenuationThreshold
		);

	p_luaState.new_usertype<ECS::Components::CAudioListener>("AudioListener",
		sol::base_classes, sol::bases<ECS::Components::AComponent>()
		);
}
