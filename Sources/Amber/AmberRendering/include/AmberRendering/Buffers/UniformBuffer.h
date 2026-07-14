#pragma once


#include "AmberRendering/Context/Driver.h"

#include "AmberRendering/Buffers/EAccessSpecifier.h"

namespace AmberRendering::Resources
{
	class Shader;
}

namespace AmberRendering::Buffers
{
	class UniformBuffer
	{
	public:
		static void BindBlockToShader(AmberRendering::Resources::Shader& p_shader, uint32_t p_uniformBlockLocation, uint32_t p_bindingPoint = 0);
		static void BindBlockToShader(AmberRendering::Resources::Shader& p_shader, const std::string& p_name, uint32_t p_bindingPoint = 0);

		static uint32_t GetBlockLocation(AmberRendering::Resources::Shader& p_shader, const std::string& p_name);

	public:
		UniformBuffer(size_t p_size, uint32_t p_bindingPoint = 0, uint32_t p_offset = 0, EAccessSpecifier p_accessSpecifier = EAccessSpecifier::DYNAMIC_DRAW);
		~UniformBuffer();

		void Bind() const;
		void Unbind() const;

		template<typename T>
		void SetSubData(const T& p_data, size_t&& p_offset)
		{
			Bind();
			glBufferSubData(GL_UNIFORM_BUFFER, std::forward<size_t>(p_offset), sizeof(T), std::addressof(p_data));
			Unbind();
		}

		template<typename T>
		void SetSubData(const T& p_data, size_t& p_offsetInOut)
		{
			Bind();
			const size_t dataSize = sizeof(T);
			glBufferSubData(GL_UNIFORM_BUFFER, p_offsetInOut, dataSize, std::addressof(p_data));
			p_offsetInOut += dataSize;
			Unbind();
		}

		uint32_t GetID() const;

	private:
		uint32_t m_bufferID;
	};
}
