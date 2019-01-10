#pragma once

#include "BlockData.h"
#include "ChunkBuffers.h"

#define CHUNK_SIZE 16
#define CHUNK_ELEMENTS_COUNT CHUNK_SIZE * CHUNK_SIZE * CHUNK_SIZE

namespace AmberCraft
{
	class Chunk
	{
		struct ChunksNeighbors
		{
			Chunk* left		= nullptr;
			Chunk* right	= nullptr;
			Chunk* bot		= nullptr;
			Chunk* top		= nullptr;
			Chunk* back		= nullptr;
			Chunk* front	= nullptr;
		};

	public:
		BlockData m_blocks[CHUNK_ELEMENTS_COUNT];
		ChunkBuffers m_chunkBuffers;
		ChunksNeighbors m_chunksNeighbors;

	private:
		uint16_t m_blocksToRenderCount;
		
	public:
		Chunk();
		~Chunk() = default;

		void FillChunk(BlockType p_blockType = BlockType::DIRT);
		void FillChunkRandomly(BlockType p_blockType = BlockType::DIRT);
		void Update();
		void Draw();

		void SetChunksNeighbors(Chunk* p_left, Chunk* p_right, Chunk* p_top, Chunk* p_bot, Chunk* p_front, Chunk* p_back);

		bool IsBlockOccluded(uint8_t p_x, uint8_t p_y, uint8_t p_z);
		bool IsInChunk(uint8_t p_index);
		std::vector<GLuint> FillBlocksToRender();

		std::array<uint8_t, 3> From1Dto3D(uint16_t p_index);
		uint16_t From3Dto1D(uint8_t p_x, uint8_t p_y, uint8_t p_z);
	};
}