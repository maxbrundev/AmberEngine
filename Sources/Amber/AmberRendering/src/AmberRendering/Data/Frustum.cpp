#include "Amberpch.h"

#include "AmberRendering/Data/Frustum.h"

#include <glm/gtc/type_ptr.hpp>

enum FrustumSide
{
	RIGHT = 0,
	LEFT = 1,
	BOTTOM = 2,
	TOP = 3,
	BACK = 4,
	FRONT = 5
};

enum PlaneData
{
	A = 0,
	B = 1,
	C = 2,
	D = 3
};

void NormalizePlane(float p_frustum[6][4], int p_side)
{
	float magnitude = std::sqrt
	(
		p_frustum[p_side][A] * p_frustum[p_side][A] +
		p_frustum[p_side][B] * p_frustum[p_side][B] +
		p_frustum[p_side][C] * p_frustum[p_side][C]
	);

	p_frustum[p_side][A] /= magnitude;
	p_frustum[p_side][B] /= magnitude;
	p_frustum[p_side][C] /= magnitude;
	p_frustum[p_side][D] /= magnitude;
}

void AmberRendering::Data::Frustum::CalculateFrustum(const glm::mat4& p_viewProjection)
{
	const float* clip = glm::value_ptr(p_viewProjection);

	m_frustum[RIGHT][A] = clip[3] - clip[0];
	m_frustum[RIGHT][B] = clip[7] - clip[4];
	m_frustum[RIGHT][C] = clip[11] - clip[8];
	m_frustum[RIGHT][D] = clip[15] - clip[12];

	NormalizePlane(m_frustum, RIGHT);

	m_frustum[LEFT][A] = clip[3] + clip[0];
	m_frustum[LEFT][B] = clip[7] + clip[4];
	m_frustum[LEFT][C] = clip[11] + clip[8];
	m_frustum[LEFT][D] = clip[15] + clip[12];

	NormalizePlane(m_frustum, LEFT);

	m_frustum[BOTTOM][A] = clip[3] + clip[1];
	m_frustum[BOTTOM][B] = clip[7] + clip[5];
	m_frustum[BOTTOM][C] = clip[11] + clip[9];
	m_frustum[BOTTOM][D] = clip[15] + clip[13];

	NormalizePlane(m_frustum, BOTTOM);

	m_frustum[TOP][A] = clip[3] - clip[1];
	m_frustum[TOP][B] = clip[7] - clip[5];
	m_frustum[TOP][C] = clip[11] - clip[9];
	m_frustum[TOP][D] = clip[15] - clip[13];

	NormalizePlane(m_frustum, TOP);

	m_frustum[BACK][A] = clip[3] - clip[2];
	m_frustum[BACK][B] = clip[7] - clip[6];
	m_frustum[BACK][C] = clip[11] - clip[10];
	m_frustum[BACK][D] = clip[15] - clip[14];

	NormalizePlane(m_frustum, BACK);

	m_frustum[FRONT][A] = clip[3] + clip[2];
	m_frustum[FRONT][B] = clip[7] + clip[6];
	m_frustum[FRONT][C] = clip[11] + clip[10];
	m_frustum[FRONT][D] = clip[15] + clip[14];

	NormalizePlane(m_frustum, FRONT);
}

bool AmberRendering::Data::Frustum::PointInFrustum(float p_x, float p_y, float p_z) const
{
	for (int i = 0; i < 6; i++)
	{
		if (m_frustum[i][A] * p_x + m_frustum[i][B] * p_y + m_frustum[i][C] * p_z + m_frustum[i][D] <= 0)
		{
			return false;
		}
	}

	return true;
}

bool AmberRendering::Data::Frustum::SphereInFrustum(float p_x, float p_y, float p_z, float p_radius) const
{
	for (int i = 0; i < 6; i++)
	{
		if (m_frustum[i][A] * p_x + m_frustum[i][B] * p_y + m_frustum[i][C] * p_z + m_frustum[i][D] <= -p_radius)
		{
			return false;
		}
	}

	return true;
}

bool AmberRendering::Data::Frustum::CubeInFrustum(float p_x, float p_y, float p_z, float p_size) const
{
	for (int i = 0; i < 6; i++)
	{
		if (m_frustum[i][A] * (p_x - p_size) + m_frustum[i][B] * (p_y - p_size) + m_frustum[i][C] * (p_z - p_size) + m_frustum[i][D] > 0)
			continue;
		if (m_frustum[i][A] * (p_x + p_size) + m_frustum[i][B] * (p_y - p_size) + m_frustum[i][C] * (p_z - p_size) + m_frustum[i][D] > 0)
			continue;
		if (m_frustum[i][A] * (p_x - p_size) + m_frustum[i][B] * (p_y + p_size) + m_frustum[i][C] * (p_z - p_size) + m_frustum[i][D] > 0)
			continue;
		if (m_frustum[i][A] * (p_x + p_size) + m_frustum[i][B] * (p_y + p_size) + m_frustum[i][C] * (p_z - p_size) + m_frustum[i][D] > 0)
			continue;
		if (m_frustum[i][A] * (p_x - p_size) + m_frustum[i][B] * (p_y - p_size) + m_frustum[i][C] * (p_z + p_size) + m_frustum[i][D] > 0)
			continue;
		if (m_frustum[i][A] * (p_x + p_size) + m_frustum[i][B] * (p_y - p_size) + m_frustum[i][C] * (p_z + p_size) + m_frustum[i][D] > 0)
			continue;
		if (m_frustum[i][A] * (p_x - p_size) + m_frustum[i][B] * (p_y + p_size) + m_frustum[i][C] * (p_z + p_size) + m_frustum[i][D] > 0)
			continue;
		if (m_frustum[i][A] * (p_x + p_size) + m_frustum[i][B] * (p_y + p_size) + m_frustum[i][C] * (p_z + p_size) + m_frustum[i][D] > 0)
			continue;

		return false;
	}

	return true;
}

bool AmberRendering::Data::Frustum::BoundingSphereInFrustum(const AmberRendering::Geometry::BoundingSphere& p_boundingSphere, const AmberMaths::Transform& p_transform) const
{
	const glm::vec3& position = p_transform.GetWorldPosition();
	const glm::quat& rotation = p_transform.GetWorldRotation();
	const glm::vec3& scale = p_transform.GetWorldScale();

	float maxScale = std::max(std::max(std::max(scale.x, scale.y), scale.z), 0.0f);
	float scaledRadius = p_boundingSphere.Radius * maxScale;
	glm::vec3 sphereOffset = rotation * p_boundingSphere.Position * maxScale;

	glm::vec3 worldCenter = position + sphereOffset;

	return SphereInFrustum(worldCenter.x, worldCenter.y, worldCenter.z, scaledRadius);
}

std::array<float, 4> AmberRendering::Data::Frustum::GetNearPlane() const
{
	return { m_frustum[FRONT][0], m_frustum[FRONT][1], m_frustum[FRONT][2], m_frustum[FRONT][3] };
}

std::array<float, 4> AmberRendering::Data::Frustum::GetFarPlane() const
{
	return { m_frustum[BACK][0], m_frustum[BACK][1], m_frustum[BACK][2], m_frustum[BACK][3] };
}
