#include "pch.h"

#include "Chunk.h"
#include "Perlin.h"

AmberCraft::Chunk::Chunk()
{
}

void AmberCraft::Chunk::FillChunk(BlockType p_blockType)
{
	//temporary perlin
	for (uint16_t i = 0; i < CHUNK_ELEMENTS_COUNT; ++i)
	{
		blocks[i].type = BlockType::AIR;
	}

	FastNoise noise;
	noise.SetSeed(428);

	for (uint16_t x = 0; x < CHUNK_SIZE; ++x)
	{
		for (uint16_t z = 0; z < CHUNK_SIZE; ++z)
		{
			double heightScale = 8 + noise.GetPerlin(x * 10, z * 10) * 8;

			//std::cout << "Height: " << heightScale << std::endl;
			for (uint16_t y = heightScale; y > 0; --y)
			{
				blocks[From3Dto1D(x, y, z)].type = p_blockType;
			}
		}
	}

	/*for (uint16_t i = 0; i < CHUNK_ELEMENTS_COUNT; ++i)
	{
		auto chunksCoordinates = From1Dto3D(i);
		bool isAir = chunksCoordinates[0] > chunksCoordinates[1];
		m_blocks[i].type = static_cast<BlockType>(isAir ? 0 : 1);

		blocks[i].type = p_blockType;
	}*/
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

void AmberCraft::Chunk::CheckNeighbors()
{
	m_isOccluded = (m_chunksNeighbors.left
		&& m_chunksNeighbors.right
		&& m_chunksNeighbors.bot
		&& m_chunksNeighbors.top
		&& m_chunksNeighbors.back
		&&  m_chunksNeighbors.front);
}

bool AmberCraft::Chunk::IsOccluded()
{
	return m_isOccluded;
}

AmberCraft::BlockData* AmberCraft::Chunk::GetBlock(uint8_t p_x, uint8_t p_y, uint8_t p_z, ChunkSides p_chunkSide)
{
	const int min = 0;
	const int max = CHUNK_SIZE - 1;

	switch (p_chunkSide)
	{
	case ChunkSides::LEFT:
		return (int(p_x - 1) >= min)
		? &blocks[From3Dto1D(p_x - 1, p_y, p_z)] 
		: (m_chunksNeighbors.left ? &m_chunksNeighbors.left->blocks[From3Dto1D(max, p_y, p_z)] : nullptr);

	case ChunkSides::RIGHT:
		return (int(p_x + 1) <= max)
		? &blocks[From3Dto1D(p_x + 1, p_y, p_z)]
		: (m_chunksNeighbors.right ? &m_chunksNeighbors.right->blocks[From3Dto1D(min, p_y, p_z)] : nullptr);

	case ChunkSides::BOT:
		return (int(p_y - 1) >= min)
		? &blocks[From3Dto1D(p_x, p_y - 1, p_z)]
		: (m_chunksNeighbors.bot ? &m_chunksNeighbors.bot->blocks[From3Dto1D(p_x, max, p_z)] : nullptr);

	case ChunkSides::TOP:
		return (int(p_y + 1) <= max)
			? &blocks[From3Dto1D(p_x, p_y + 1, p_z)]
			: (m_chunksNeighbors.top ? &m_chunksNeighbors.top->blocks[From3Dto1D(p_x, min, p_z)] : nullptr);

	case ChunkSides::BACK:
		return (int(p_z - 1) >= min)
			? &blocks[From3Dto1D(p_x, p_y, p_z - 1)]
			: (m_chunksNeighbors.back ? &m_chunksNeighbors.back->blocks[From3Dto1D(p_x, p_y, max)] : nullptr);

	case ChunkSides::FRONT:
		return (int(p_z + 1) <= max)
			? &blocks[From3Dto1D(p_x, p_y, p_z + 1)]
			: (m_chunksNeighbors.front ? &m_chunksNeighbors.front->blocks[From3Dto1D(p_x, p_y, min)] : nullptr);

	default:
		return nullptr;
	}
}

void AmberCraft::Chunk::Update()
{
	std::vector<GLuint> blocksToRender = FillBlocksToRender();
	m_chunkBuffers.SendBlocksToGPU(blocksToRender);
	m_blocksToRenderCount = static_cast<uint16_t>(blocksToRender.size());

	//CheckNeighbors();
}

void AmberCraft::Chunk::Draw()
{
	m_chunkBuffers.DrawChunk(m_blocksToRenderCount);
}

void AmberCraft::Chunk::SetChunksNeighbors(Chunk* p_left, Chunk* p_right, Chunk* p_top, Chunk* p_bot, Chunk* p_front, Chunk* p_back)
{
	m_chunksNeighbors.left	= p_left;
	m_chunksNeighbors.right = p_right;
	m_chunksNeighbors.top	= p_top;
	m_chunksNeighbors.bot	= p_bot;
	m_chunksNeighbors.front = p_front;
	m_chunksNeighbors.back	= p_back;
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
		blockData.bytes[3] = static_cast<uint8_t>(blocks[From3Dto1D(blockCoordinates[0], blockCoordinates[1], blockCoordinates[2])].type);

		if (!IsBlockOccluded(blockCoordinates[0], blockCoordinates[1], blockCoordinates[2]) && blockData.bytes[3] != 0)
		{
			blocksToRender.push_back(blockData.data);
		}
	}

	return blocksToRender;
}

bool AmberCraft::Chunk::IsBlockOccluded(uint8_t p_x, uint8_t p_y, uint8_t p_z)
{
	BlockData* left		= GetBlock(p_x, p_y, p_z, ChunkSides::LEFT);
	BlockData* right	= GetBlock(p_x, p_y, p_z, ChunkSides::RIGHT);
	BlockData* top		= GetBlock(p_x, p_y, p_z, ChunkSides::TOP);
	BlockData* bot		= GetBlock(p_x, p_y, p_z, ChunkSides::BOT);
	BlockData* front	= GetBlock(p_x, p_y, p_z, ChunkSides::FRONT);
	BlockData* back		= GetBlock(p_x, p_y, p_z, ChunkSides::BACK);

	return (!left ||  left->type != BlockType::AIR)
		&& (!right || right->type != BlockType::AIR)
		&& (!top || top->type != BlockType::AIR)
		&& (!bot || bot->type != BlockType::AIR)
		&& (!front || front->type != BlockType::AIR)
		&& (!back || back->type != BlockType::AIR);

	//DRAW WORLD WITH BOUNDS
	/*return (left &&  left->type		!= BlockType::AIR
		&& right &&  right->type	!= BlockType::AIR
		&& top &&  top->type		!= BlockType::AIR
		&& bot &&  bot->type		!= BlockType::AIR
		&& front &&  front->type	!= BlockType::AIR
		&& back &&  back->type		!= BlockType::AIR);*/
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