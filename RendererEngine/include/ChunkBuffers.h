#pragma once

namespace AmberCraft
{
	class ChunkBuffers
	{
	private:
		GLuint m_vao{};
		GLuint m_vbo{};
		GLuint m_ssbo{};

	public:
		ChunkBuffers();
		~ChunkBuffers() = default;

		void InitBuffers();
		void SendBlocksToGPU(const std::vector<GLuint>& p_blocksToRender);
		void DrawChunk(uint16_t p_blocksToRenderCount);
	};
}