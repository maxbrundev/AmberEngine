#include "Amberpch.h"

#include "AmberEngine/Resources/ObjLoader.h"

AmberEngine::Resources::ObjLoader::ObjLoader(const std::string& p_filePath) : hasUVs(false), hasNormals(false)
{
	ParseFile(p_filePath);
}

void AmberEngine::Resources::ObjLoader::ParseFile(const std::string& p_filePath)
{
	std::ifstream file;
	file.open(p_filePath.c_str());

	std::string line;

	if (file.is_open())
	{
		while (file.good())
		{
			getline(file, line);

			const auto lineLength = line.length();

			if (lineLength < 2)
				continue;

			const char* fileLine = line.c_str();

			switch (fileLine[0])
			{
			case 'v':
				if (fileLine[1] == 't')
					this->m_uvs.push_back(ParseObjVec2(line));
				else if (fileLine[1] == 'n')
					this->m_normals.push_back(ParseObjVec3(line));
				else if (fileLine[1] == ' ' || fileLine[1] == '\t')
					this->m_vertices.push_back(ParseObjVec3(line));
				break;
			case 'f':
				CreateObjFace(line);
				break;
			default:
				break;
			};
		}
	}
	else
	{
		std::cerr << "Unable to load Obj: " << p_filePath << std::endl;
	}
}

AmberEngine::Resources::ObjModel AmberEngine::Resources::ObjLoader::LoadModel()
{
	ObjModel finalModel;
	ObjModel normalModel;

	const auto numIndices = m_indices.size();

	std::vector<ObjIndexData*> indexLookup;

	for (int i = 0; i < numIndices; i++)
		indexLookup.push_back(&m_indices[i]);

	std::sort(indexLookup.begin(), indexLookup.end());

	std::map<ObjIndexData, unsigned int> normalModelIndexMap;
	std::map<unsigned int, unsigned int> indexMap;

	for (int i = 0; i < numIndices; i++)
	{
		ObjIndexData* currentIndex = &m_indices[i];

		glm::vec3 currentPosition = m_vertices[currentIndex->vertexIndex];
		glm::vec2 currentTexCoord;
		glm::vec3 currentNormal;

		if (hasUVs)
			currentTexCoord = m_uvs[currentIndex->uvIndex];
		else
			currentTexCoord = glm::vec2(0, 0);

		if (hasNormals)
			currentNormal = m_normals[currentIndex->normalIndex];
		else
			currentNormal = glm::vec3(0, 0, 0);

		unsigned int normalModelIndex;
		unsigned int resultModelIndex;

		std::map<ObjIndexData, unsigned int>::iterator it = normalModelIndexMap.find(*currentIndex);
		if (it == normalModelIndexMap.end())
		{
			normalModelIndex = normalModel.positionsIndices.size();

			normalModelIndexMap.insert(std::pair<ObjIndexData, unsigned int>(*currentIndex, normalModelIndex));
			normalModel.positionsIndices.push_back(currentPosition);
			normalModel.texCoordsIndices.push_back(currentTexCoord);
			normalModel.normalsIndices.push_back(currentNormal);
		}
		else
			normalModelIndex = it->second;

		const unsigned int previousVertexLocation = FindLastVertexIndex(indexLookup, currentIndex, finalModel);

		if (previousVertexLocation == -1)
		{
			resultModelIndex = finalModel.positionsIndices.size();

			finalModel.positionsIndices.push_back(currentPosition);
			finalModel.texCoordsIndices.push_back(currentTexCoord);
			finalModel.normalsIndices.push_back(currentNormal);
		}
		else
			resultModelIndex = previousVertexLocation;

		normalModel.indices.push_back(normalModelIndex);
		finalModel.indices.push_back(resultModelIndex);
		indexMap.insert(std::pair<unsigned int, unsigned int>(resultModelIndex, normalModelIndex));
	}

	if (!hasNormals)
	{
		CalculateNormals(normalModel);

		for (int i = 0; i < finalModel.positionsIndices.size(); i++)
			finalModel.normalsIndices[i] = normalModel.normalsIndices[indexMap[i]];
	}

	return finalModel;
}

unsigned int AmberEngine::Resources::ObjLoader::FindNextChar(unsigned int p_start, const char* p_source, unsigned int p_length, char p_character)
{
	unsigned int result = p_start;

	while (result < p_length)
	{
		result++;

		if (p_source[result] == p_character)
			break;
	}

	return result;
}

unsigned int AmberEngine::Resources::ObjLoader::ParseObjIndexValue(const std::string& p_source, unsigned int p_start, unsigned int p_end)
{
	return std::atoi(p_source.substr(p_start, p_end - p_start).c_str()) - 1;
}

float AmberEngine::Resources::ObjLoader::ParseObjFloatValue(const std::string& p_source, unsigned int p_start, unsigned int p_end)
{
	return std::atof(p_source.substr(p_start, p_end - p_start).c_str());
}

std::vector<std::string> AmberEngine::Resources::ObjLoader::ParseString(const std::string& p_source, char p_character)
{
	std::vector<std::string> result;

	std::stringstream ss(p_source);
	std::string data;

	while (std::getline(ss, data, p_character))
	{
		result.push_back(data);
	}

	return result;
}

unsigned int AmberEngine::Resources::ObjLoader::FindLastVertexIndex(const std::vector<ObjIndexData*>& p_indexLookup, const ObjIndexData* p_currentIndex, const ObjModel& p_result)
{
	unsigned int start = 0;
	auto end = p_indexLookup.size();
	unsigned int current = (end - start) * 0.5f + start;
	unsigned int previous = start;

	while (current != previous)
	{
		ObjIndexData* testIndex = p_indexLookup[current];

		if (testIndex->vertexIndex == p_currentIndex->vertexIndex)
		{
			unsigned int countStart = current;

			for (int i = 0; i < current; i++)
			{
				ObjIndexData* possibleIndex = p_indexLookup[current - i];

				if (possibleIndex == p_currentIndex)
					continue;
				if (possibleIndex->vertexIndex != p_currentIndex->vertexIndex)
					break;

				countStart--;
			}

			for (int i = countStart; i < p_indexLookup.size() - countStart; i++)
			{
				ObjIndexData* possibleIndex = p_indexLookup[current + i];

				if (possibleIndex == p_currentIndex)
					continue;
				if (possibleIndex->vertexIndex != p_currentIndex->vertexIndex)
					break;

				if ((!hasUVs || possibleIndex->uvIndex == p_currentIndex->uvIndex) && (!hasNormals || possibleIndex->normalIndex == p_currentIndex->normalIndex))
				{
					glm::vec3 currentPosition = m_vertices[p_currentIndex->vertexIndex];
					glm::vec2 currentTexCoord;
					glm::vec3 currentNormal;

					if (hasUVs)
						currentTexCoord = m_uvs[p_currentIndex->uvIndex];
					else
						currentTexCoord = glm::vec2(0, 0);

					if (hasNormals)
						currentNormal = m_normals[p_currentIndex->normalIndex];
					else
						currentNormal = glm::vec3(0, 0, 0);

					for (int j = 0; j < p_result.positionsIndices.size(); j++)
					{
						if (currentPosition == p_result.positionsIndices[j] && ((!hasUVs || currentTexCoord == p_result.texCoordsIndices[j]) && (!hasNormals || currentNormal == p_result.normalsIndices[j])))
						{
							return j;
						}
					}
				}
			}

			return -1;
		}

		if (testIndex->vertexIndex < p_currentIndex->vertexIndex)
			start = current;
		else
			end = current;

		previous = current;
		current = (end - start) * 0.5f + start;
	}

	return -1;
}

void AmberEngine::Resources::ObjLoader::CreateObjFace(const std::string& p_line)
{
	std::vector<std::string> data = ParseString(p_line, ' ');

	this->m_indices.push_back(ParseObjIndex(data[1]));
	this->m_indices.push_back(ParseObjIndex(data[2]));
	this->m_indices.push_back(ParseObjIndex(data[3]));

	if (data.size() > 4)
	{
		this->m_indices.push_back(ParseObjIndex(data[1]));
		this->m_indices.push_back(ParseObjIndex(data[3]));
		this->m_indices.push_back(ParseObjIndex(data[4]));
	}
}

void AmberEngine::Resources::ObjLoader::CalculateNormals(ObjModel & p_model)
{
	for (int i = 0; i < p_model.indices.size(); i += 3)
	{
		const int i0 = p_model.indices[i];
		const int i1 = p_model.indices[i + 1];
		const int i2 = p_model.indices[i + 2];

		glm::vec3 v1 = p_model.positionsIndices[i1] - p_model.positionsIndices[i0];
		glm::vec3 v2 = p_model.positionsIndices[i2] - p_model.positionsIndices[i0];

		const glm::vec3 normal = glm::normalize(glm::cross(v1, v2));

		p_model.normalsIndices[i0] += normal;
		p_model.normalsIndices[i1] += normal;
		p_model.normalsIndices[i2] += normal;
	}

	for (int i = 0; i < p_model.positionsIndices.size(); i++)
		p_model.normalsIndices[i] = glm::normalize(p_model.normalsIndices[i]);
}

glm::vec2 AmberEngine::Resources::ObjLoader::ParseObjVec2(const std::string& p_line)
{
	const auto lineLength = p_line.length();
	const char* line = p_line.c_str();

	unsigned int vertIndexStart = 3;

	while (vertIndexStart < lineLength)
	{
		if (line[vertIndexStart] != ' ')
			break;
		vertIndexStart++;
	}

	unsigned int vertIndexEnd = FindNextChar(vertIndexStart, line, lineLength, ' ');

	const float xValue = ParseObjFloatValue(p_line, vertIndexStart, vertIndexEnd);

	vertIndexStart = vertIndexEnd + 1;
	vertIndexEnd = FindNextChar(vertIndexStart, line, lineLength, ' ');

	const float yValue = ParseObjFloatValue(p_line, vertIndexStart, vertIndexEnd);

	return glm::vec2(xValue, yValue);
}

glm::vec3 AmberEngine::Resources::ObjLoader::ParseObjVec3(const std::string& p_line) 
{
	const auto lineLength = p_line.length();
	const char* line = p_line.c_str();

	unsigned int vertIndexStart = 2;

	while (vertIndexStart < lineLength)
	{
		if (line[vertIndexStart] != ' ')
			break;
		vertIndexStart++;
	}

	unsigned int vertIndexEnd = FindNextChar(vertIndexStart, line, lineLength, ' ');

	const float xValue = ParseObjFloatValue(p_line, vertIndexStart, vertIndexEnd);

	vertIndexStart = vertIndexEnd + 1;
	vertIndexEnd = FindNextChar(vertIndexStart, line, lineLength, ' ');

	const float yValue = ParseObjFloatValue(p_line, vertIndexStart, vertIndexEnd);

	vertIndexStart = vertIndexEnd + 1;
	vertIndexEnd = FindNextChar(vertIndexStart, line, lineLength, ' ');

	const float zValue = ParseObjFloatValue(p_line, vertIndexStart, vertIndexEnd);

	return glm::vec3(xValue, yValue, zValue);
}

AmberEngine::Resources::ObjIndexData AmberEngine::Resources::ObjLoader::ParseObjIndex(const std::string& p_source)
{
	const auto sourceLength = p_source.length();
	const char* source = p_source.c_str();

	unsigned int vertIndexStart = 0;
	unsigned int vertIndexEnd = FindNextChar(vertIndexStart, source, sourceLength, '/');

	ObjIndexData result;
	result.vertexIndex = ParseObjIndexValue(p_source, vertIndexStart, vertIndexEnd);
	result.uvIndex = 0;
	result.normalIndex = 0;

	if (vertIndexEnd >= sourceLength)
		return result;

	vertIndexStart = vertIndexEnd + 1;
	vertIndexEnd = FindNextChar(vertIndexStart, source, sourceLength, '/');

	result.uvIndex = ParseObjIndexValue(p_source, vertIndexStart, vertIndexEnd);
	hasUVs = true;

	if (vertIndexEnd >= sourceLength)
		return result;

	vertIndexStart = vertIndexEnd + 1;
	vertIndexEnd = FindNextChar(vertIndexStart, source, sourceLength, '/');

	result.normalIndex = ParseObjIndexValue(p_source, vertIndexStart, vertIndexEnd);
	hasNormals = true;

	return result;
}