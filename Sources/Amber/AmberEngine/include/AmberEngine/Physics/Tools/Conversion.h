#pragma once

#include <bullet/btBulletCollisionCommon.h>

#include "AmberEngine/Maths/Transform.h"

namespace AmberEngine::Physics::Tools
{
	class Conversion
	{
	public:
		Conversion() = delete;

		static btTransform ToBtTransform(const Maths::Transform& p_transform);

		static btVector3 ToBtVector3(const glm::vec3& p_vector);

		static btQuaternion ToBtQuaternion(const glm::quat& p_quaternion);

		static Maths::Transform ToOvTransform(const btTransform& p_transform);

		static glm::vec3 ToOvVector3(const btVector3& p_vector);

		static glm::quat ToOvQuaternion(const btQuaternion& p_quaternion);
	};
}
