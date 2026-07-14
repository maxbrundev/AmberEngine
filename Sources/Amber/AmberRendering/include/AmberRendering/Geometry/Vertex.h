#pragma once

namespace AmberRendering::Geometry
{
	struct Vertex
	{
		float Position[3];
		float TexCoords[2];
		float Normals[3];
		float Tangent[3];
		float Bitangent[3];
	};
}