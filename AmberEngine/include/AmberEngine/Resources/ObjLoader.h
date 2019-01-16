#pragma once

#include <vector>

#include <glm/glm.hpp>

#include "AmberEngine/API/Export.h"

namespace AmberEngine::Resources
{
	struct API_AMBERENGINE IndexData
	{
		unsigned int vertexIndex;
		unsigned int uvIndex;
		unsigned int normalIndex;
		bool operator<(const IndexData& p_index) const { return vertexIndex < p_index.vertexIndex; }
	};

	struct API_AMBERENGINE Model
	{
		std::vector<glm::vec3> positionsIndices;
		std::vector<glm::vec2> texCoordsIndices;
		std::vector<glm::vec3> normalsIndices;
		std::vector<unsigned int> indices;
	};

	class API_AMBERENGINE ObjLoader
	{
	private:
		std::vector<IndexData> m_indices;
		std::vector<glm::vec3> m_vertices;
		std::vector<glm::vec2> m_uvs;
		std::vector<glm::vec3> m_normals;
		bool hasUVs;
		bool hasNormals;

	public:
		ObjLoader(const std::string& p_filePath);
		~ObjLoader() = default;

		void ParseFile(const std::string& p_filePath);
		Model LoadModel();

	private:
		void CreateObjFace(const std::string& p_line);
		void CalculateNormals(Model& p_model);
		IndexData ParseObjIndex(const std::string& p_source);
		glm::vec2 ParseObjVec2(const std::string& p_line);
		glm::vec3 ParseObjVec3(const std::string& p_line);
		std::vector<std::string> ParseString(const std::string& p_source, char p_character);
		float ParseObjFloatValue(const std::string& p_source, unsigned int p_start, unsigned int p_end);
		unsigned int ParseObjIndexValue(const std::string& p_source, unsigned int p_start, unsigned int p_end);

		unsigned int FindLastVertexIndex(const std::vector<IndexData*>& p_indexLookup, const IndexData* p_currentIndex, const Model& p_result);
		unsigned int FindNextChar(unsigned int p_start, const char* p_source, unsigned int p_length, char p_character);
	};
}