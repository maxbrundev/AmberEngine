#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>

#include "AmberEngine/API/Export.h"

namespace AmberEngine::Resources
{
	class API_AMBERENGINE Shader
	{
	public:
		Shader(std::string p_filePath, uint32_t p_id);
		~Shader();

		void Bind() const;
		void Unbind() const;

		void SetUniformIntPointer(const std::string_view p_name, int p_count, const int* p_value);
		void SetUniform1i(const std::string_view p_name, const int p_value);
		void SetUniform1f(const std::string_view p_name, const float p_value);
		void SetUniform4f(const std::string_view p_name, const float p_v0, const float p_v1, const float p_v2, const float p_v3);
		void SetUniformVec3(const std::string_view p_name, const glm::vec3& p_vec3);
		void SetUniformVec4(const std::string_view p_name, const glm::vec4& p_vec4);
		void SetUniformMat4(const std::string_view p_name, const glm::mat4& p_mat4);

	private:
		uint32_t GetUniformLocation(const std::string_view p_name);

	public:
		const std::string path;
		const uint32_t id;

	private:
		std::unordered_map<std::string_view, uint32_t> m_uniformLocationCache;
	};
}