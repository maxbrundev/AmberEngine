#pragma once

#include <glm/glm.hpp>

namespace AmberRendering::Geometry
{
	struct BoundingSphere
	{
		glm::vec3 Position;
		float Radius;
	};
}
