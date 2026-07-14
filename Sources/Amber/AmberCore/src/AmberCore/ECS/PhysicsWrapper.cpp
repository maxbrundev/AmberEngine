#include "Amberpch.h"

#include "AmberCore/ECS/PhysicsWrapper.h"

#include "AmberPhysics/Core/PhysicsEngine.h"

#include "AmberTools/Global/ServiceLocator.h"

bool AmberCore::ECS::PhysicsWrapper::Raycast(glm::vec3 p_origin, glm::vec3 p_direction, float p_distance, RaycastHit& p_outHit)
{
	if (auto result = AmberTools::Global::ServiceLocator::Get<AmberPhysics::Core::PhysicsEngine>().Raycast(p_origin, p_direction, p_distance))
	{
		p_outHit.FirstResultObject = std::addressof(result.value().FirstResultObject->GetUserData<std::reference_wrapper<Components::CPhysicalObject>>().get());

		for (auto object : result.value().ResultObjects)
		{
			p_outHit.ResultObjects.push_back(std::addressof(object->GetUserData<std::reference_wrapper<Components::CPhysicalObject>>().get()));
		}

		return true;
	}

	return false;
}
