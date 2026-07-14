#include "Amberpch.h"

#include "AmberPhysics/Tools/Conversion.h"

#include "AmberMaths/Transform.h"

btTransform AmberPhysics::Tools::Conversion::ToBtTransform(const AmberMaths::Transform& p_transform)
{
	return btTransform(ToBtQuaternion(p_transform.GetWorldRotation()), ToBtVector3(p_transform.GetWorldPosition()));
}

btVector3 AmberPhysics::Tools::Conversion::ToBtVector3(const glm::vec3& p_vector)
{
	return btVector3(p_vector.x, p_vector.y, p_vector.z);
}

btQuaternion AmberPhysics::Tools::Conversion::ToBtQuaternion(const glm::quat& p_quaternion)
{
	return btQuaternion(p_quaternion.x, p_quaternion.y, p_quaternion.z, p_quaternion.w);
}

AmberMaths::Transform AmberPhysics::Tools::Conversion::ToOvTransform(const btTransform& p_transform)
{
	glm::quat q = ToOvQuaternion(p_transform.getRotation());
	glm::vec3 v = ToOvVector3(p_transform.getOrigin());

	return AmberMaths::Transform(v, glm::degrees(glm::eulerAngles(q)));
}

glm::vec3 AmberPhysics::Tools::Conversion::ToOvVector3(const btVector3& p_vector)
{
	return glm::vec3(p_vector.getX(), p_vector.getY(), p_vector.getZ());
}

glm::quat AmberPhysics::Tools::Conversion::ToOvQuaternion(const btQuaternion& p_quaternion)
{
	return glm::quat(p_quaternion.getW(), p_quaternion.getX(), p_quaternion.getY(), p_quaternion.getZ());
}
