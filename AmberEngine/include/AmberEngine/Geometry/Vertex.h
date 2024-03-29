#pragma once

namespace AmberEngine::Geometry
{
	struct Vertex
	{
		float position[3];
		float texCoords[2];
		float normals[3];
		float tangent[3];
		float bitangent[3];
	};
}