#pragma once

#include "AmberEngine/API/Export.h"

#include "AmberEngine/Context/Driver.h"

#include "AmberEngine/Buffers/EAccessSpecifier.h"

namespace AmberEngine::Buffers
{
	class API_AMBERENGINE ShaderStorageBuffer
	{
	public:
		ShaderStorageBuffer(EAccessSpecifier p_accessSpecifier);

		~ShaderStorageBuffer();

		void Bind(uint32_t p_bindingPoint);
		void Unbind();

		template<typename T>
		void SendBlocks(T* p_data, size_t p_size)
		{
			glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_bufferID);
			glBufferData(GL_SHADER_STORAGE_BUFFER, p_size, p_data, GL_DYNAMIC_DRAW);
			glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
		}

	private:
		uint32_t m_bufferID;
		uint32_t m_bindingPoint = 0;
	};
}
