#pragma once

namespace AmberCraft
{
	class ChunkBuffers
	{
	private:
		GLuint vao{};
		GLuint vbo{};
		GLuint ssbo{};

	public:
		ChunkBuffers();
		~ChunkBuffers() = default;

		void InitBuffers();
		void SendBlocksToGPU(const std::vector<GLuint>& p_blocksToRender);
		void DrawChunk(uint16_t p_blocksToRenderCount);
	};
}