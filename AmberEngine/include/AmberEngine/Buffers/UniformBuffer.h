#pragma once

#include "AmberEngine/API/Export.h"

#include "AmberEngine/Context/Driver.h"

#include "AmberEngine/Buffers/EAccessSpecifier.h"

namespace AmberEngine
{
	namespace Resources
	{
		class Shader;
	}
}

namespace AmberEngine::Buffers
{
	class API_AMBERENGINE UniformBuffer
	{
	public:
		UniformBuffer(size_t p_size, uint32_t p_bindingPoint = 0, uint32_t p_offset = 0, EAccessSpecifier p_accessSpecifier = EAccessSpecifier::DYNAMIC_DRAW);

		~UniformBuffer();

		void Bind() const;
		void Unbind() const;

		template<typename T>
		void SetSubData(const T& p_data, size_t p_offset)
		{
			Bind();
			glBufferSubData(GL_UNIFORM_BUFFER, p_offset, sizeof(T), std::addressof(p_data));
			Unbind();
		}

		template<typename T>
		void SetSubData(const T& p_data, std::reference_wrapper<size_t> p_offsetInOut)
		{
			Bind();
			size_t dataSize = sizeof(T);
			glBufferSubData(GL_UNIFORM_BUFFER, p_offsetInOut.get(), dataSize, std::addressof(p_data));
			p_offsetInOut.get() += dataSize;
			Unbind();
		}

		uint32_t GetID() const;

		static void BindBlockToShader(Resources::Shader& p_shader, uint32_t p_uniformBlockLocation, uint32_t p_bindingPoint = 0);
		static void BindBlockToShader(Resources::Shader& p_shader, const std::string& p_name, uint32_t p_bindingPoint = 0);

		static uint32_t GetBlockLocation(Resources::Shader& p_shader, const std::string& p_name);

	private:
		uint32_t m_bufferID;
	};
}
