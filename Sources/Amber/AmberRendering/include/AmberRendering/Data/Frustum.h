#pragma once

#include <array>

#include <glm/glm.hpp>

#include "AmberMaths/Transform.h"

#include "AmberRendering/Geometry/BoundingSphere.h"

namespace AmberRendering::Data
{
	class Frustum
	{
	public:
		void CalculateFrustum(const glm::mat4& p_viewProjection);

		bool PointInFrustum(float p_x, float p_y, float p_z) const;
		bool SphereInFrustum(float p_x, float p_y, float p_z, float p_radius) const;
		bool CubeInFrustum(float p_x, float p_y, float p_z, float p_size) const;
		bool BoundingSphereInFrustum(const AmberRendering::Geometry::BoundingSphere& p_boundingSphere, const AmberMaths::Transform& p_transform) const;

		std::array<float, 4> GetNearPlane() const;
		std::array<float, 4> GetFarPlane() const;

	private:
		float m_frustum[6][4];
	};
}
