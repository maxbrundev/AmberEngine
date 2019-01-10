#include "pch.h"

#include "Chunk.h"

AmberCraft::Chunk::Chunk()
{
	FillChunk();
}

void AmberCraft::Chunk::FillChunk(BlockType p_blockType)
{
	for (uint16_t i = 0; i < CHUNK_ELEMENTS_COUNT; ++i)
	{
		blocks[i].type = p_blockType;
	}
}

void AmberCraft::Chunk::FillChunkRandomly(BlockType p_blockType)
{
	std::random_device generator;
	const std::uniform_int_distribution<int> distribution(0, 1);

	for (uint16_t i = 0; i < CHUNK_ELEMENTS_COUNT; ++i)
	{
		blocks[i].type = static_cast<BlockType>(distribution(generator));
	}
}

void AmberCraft::Chunk::Update()
{
	std::vector<GLuint> blocksToRender = FillBlocksToRender();
	chunkBuffers.SendBlocksToGPU(blocksToRender);

	m_blocksToRenderCount = static_cast<uint16_t>(blocksToRender.size());
}

void AmberCraft::Chunk::Draw()
{
	chunkBuffers.DrawChunk(m_blocksToRenderCount);
}

std::vector<GLuint> AmberCraft::Chunk::FillBlocksToRender()
{
	std::vector<GLuint> blocksToRender;

	for (uint16_t i = 0; i < CHUNK_ELEMENTS_COUNT; ++i)
	{
		auto blockCoordinates = From1Dto3D(i);

		union
		{
			uint32_t data;
			uint8_t bytes[4];

		}blockData;

		blockData.bytes[0] = static_cast<uint8_t>(i % CHUNK_SIZE);
		blockData.bytes[1] = static_cast<uint8_t>((i / CHUNK_SIZE) % CHUNK_SIZE);
		blockData.bytes[2] = static_cast<uint8_t>(i / (CHUNK_SIZE * CHUNK_SIZE));

		if (!IsBlockOccluded(blockCoordinates[0], blockCoordinates[1], blockCoordinates[2]))
		{
			blockData.bytes[3] = static_cast<uint8_t>(blocks[From3Dto1D(blockCoordinates[0], blockCoordinates[1], blockCoordinates[2])].type);
			blocksToRender.push_back(blockData.data);
		}
	}

	return blocksToRender;
}


bool AmberCraft::Chunk::IsBlockOccluded(uint8_t p_x, uint8_t p_y, uint8_t p_z)
{
	bool HasRightNeighboor	= IsInChunk(p_x + 1) && blocks[From3Dto1D(p_x + 1, p_y + 0, p_z + 0)].type != BlockType::AIR;
	bool HasLeftNeighboor	= IsInChunk(p_x - 1) && blocks[From3Dto1D(p_x - 1, p_y + 0, p_z + 0)].type != BlockType::AIR;
	bool HasTopNeighboor	= IsInChunk(p_y + 1) && blocks[From3Dto1D(p_x + 0, p_y + 1, p_z + 0)].type != BlockType::AIR;
	bool HasBottomNeighboor = IsInChunk(p_y - 1) && blocks[From3Dto1D(p_x + 0, p_y - 1, p_z + 0)].type != BlockType::AIR;
	bool HasFrontNeighboor	= IsInChunk(p_z + 1) && blocks[From3Dto1D(p_x + 0, p_y + 0, p_z + 1)].type != BlockType::AIR;
	bool HasBackNeighboor	= IsInChunk(p_z - 1) && blocks[From3Dto1D(p_x + 0, p_y + 0, p_z - 1)].type != BlockType::AIR;

	return (HasRightNeighboor && HasLeftNeighboor && HasTopNeighboor && HasBottomNeighboor && HasFrontNeighboor && HasBackNeighboor);
}

bool AmberCraft::Chunk::IsInChunk(uint8_t p_index)
{
	return p_index >= 0 && p_index <= CHUNK_SIZE - 1;
}

std::array<uint8_t, 3> AmberCraft::Chunk::From1Dto3D(uint16_t p_index)
{
	std::array<uint8_t, 3> result;
	result[2] = p_index / (CHUNK_SIZE * CHUNK_SIZE);
	p_index -= (std::get<2>(result) * (CHUNK_SIZE * CHUNK_SIZE));
	result[1] = p_index / CHUNK_SIZE;
	result[0] = p_index % CHUNK_SIZE;
	return result;
}

uint16_t AmberCraft::Chunk::From3Dto1D(uint8_t p_x, uint8_t p_y, uint8_t p_z)
{
	return p_x + p_y * CHUNK_SIZE + p_z * CHUNK_SIZE * CHUNK_SIZE;
}