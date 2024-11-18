#include "Amberpch.h"

#include "AmberEngine/Physics/Tools/Conversion.h"

#include "AmberEngine/Maths/Transform.h"

btTransform AmberEngine::Physics::Tools::Conversion::ToBtTransform(const Maths::Transform& p_transform)
{
	return btTransform(ToBtQuaternion(p_transform.GetWorldRotation()), ToBtVector3(p_transform.GetWorldPosition()));
}

btVector3 AmberEngine::Physics::Tools::Conversion::ToBtVector3(const glm::vec3& p_vector)
{
	return btVector3(p_vector.x, p_vector.y, p_vector.z);
}

btQuaternion AmberEngine::Physics::Tools::Conversion::ToBtQuaternion(const glm::quat& p_quaternion)
{
	return btQuaternion(p_quaternion.x, p_quaternion.y, p_quaternion.z, p_quaternion.w);
}

AmberEngine::Maths::Transform AmberEngine::Physics::Tools::Conversion::ToOvTransform(const btTransform& p_transform)
{
	glm::quat q = ToOvQuaternion(p_transform.getRotation());
	glm::vec3 v = ToOvVector3(p_transform.getOrigin());

	return Maths::Transform(v, glm::degrees(glm::eulerAngles(q)));
}

glm::vec3 AmberEngine::Physics::Tools::Conversion::ToOvVector3(const btVector3& p_vector)
{
	return glm::vec3(p_vector.getX(), p_vector.getY(), p_vector.getZ());
}

glm::quat AmberEngine::Physics::Tools::Conversion::ToOvQuaternion(const btQuaternion& p_quaternion)
{
	return glm::quat(p_quaternion.getW(), p_quaternion.getX(), p_quaternion.getY(), p_quaternion.getZ());
}
