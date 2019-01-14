#include "pch.h"

#include "World.h"

AmberCraft::World::World() : m_chunks(WORLD_ELEMENTS_COUNT)
{
	SetNeighbors();
	GenerateWorld();

	for (uint16_t i = 0; i < WORLD_ELEMENTS_COUNT; ++i)
	{
		m_chunks[i].Update();
	}
}

void AmberCraft::World::SetNeighbors()
{
	for (uint16_t i = 0; i < WORLD_ELEMENTS_COUNT; ++i)
	{
		auto [x, y, z] = From1Dto3D(i);

		Chunk* left	 = IsInWorld(x - 1)	? &m_chunks[From3Dto1D(x - 1, y, z)] : nullptr;
		Chunk* right = IsInWorld(x + 1)	? &m_chunks[From3Dto1D(x + 1, y, z)] : nullptr;
		Chunk* bot	 = IsInWorld(y - 1)	? &m_chunks[From3Dto1D(x, y - 1, z)] : nullptr;
		Chunk* top	 = IsInWorld(y + 1) ? &m_chunks[From3Dto1D(x, y + 1, z)] : nullptr;
		Chunk* back	 = IsInWorld(z - 1)	? &m_chunks[From3Dto1D(x, y, z - 1)] : nullptr;
		Chunk* front = IsInWorld(z + 1)	? &m_chunks[From3Dto1D(x, y, z + 1)] : nullptr;

		m_chunks[i].SetChunksNeighbors(left, right, top, bot, front, back);
	}
}

bool AmberCraft::World::IsInWorld(uint8_t p_index)
{
	return p_index >= 0 && p_index <= WORLD_SIZE - 1;
}

void AmberCraft::World::GenerateWorld()
{
	for (uint16_t i = 0; i < WORLD_ELEMENTS_COUNT; ++i)
	{
		auto[x, y, z] = From1Dto3D(i);

		//m_chunks[i].FillChunk(y > WORLD_SIZE / 2 ? BlockType::AIR : BlockType::DIRT);
		m_chunks[i].FillChunk();
	}
}

void AmberCraft::World::Draw(RenderEngine::Managers::RenderingManager& p_renderingManager)
{
	glm::mat4 projectionMatrix = p_renderingManager.CalculateProjectionMatrix();
	glm::mat4 viewMatrix = p_renderingManager.CalculateViewMatrix();

	auto& chunkShader = p_renderingManager.GetResourcesManager()->GetShader("chunk");

	p_renderingManager.GetResourcesManager()->GetTexture("dirt").Bind();

	chunkShader.Bind();
	chunkShader.SetUniformMat4("projection", projectionMatrix);
	chunkShader.SetUniformMat4("view", viewMatrix);
	chunkShader.SetUniform1i("texture1", 0);
	
	for (uint16_t i = 0; i < WORLD_ELEMENTS_COUNT; ++i)
	{
		auto [x, y, z] = From1Dto3D(i);

		glm::vec3 chunkPosition(x * CHUNK_SIZE, y * CHUNK_SIZE, z * CHUNK_SIZE);

		if(glm::distance(chunkPosition, p_renderingManager.GetCamera()->GetPosition()) <= 200)
		{
			chunkShader.SetUniformMat4("model", glm::translate(glm::mat4(1.0f), chunkPosition));
			m_chunks[i].Draw();
		}
	}
}

AmberCraft::BlockData AmberCraft::World::GetBlock(uint64_t p_x, uint64_t p_y, uint64_t p_z)
{
	return m_chunks[From3Dto1D(p_x / CHUNK_SIZE, p_y / CHUNK_SIZE, p_z / CHUNK_SIZE)].blocks[From3Dto1D(p_x % CHUNK_SIZE, p_y % CHUNK_SIZE, p_z % CHUNK_SIZE)];
}

void AmberCraft::World::SetBlock(uint64_t p_x, uint64_t p_y, uint64_t p_z, BlockData p_blockData)
{
	m_chunks[From3Dto1D(p_x / CHUNK_SIZE, p_y / CHUNK_SIZE, p_z / CHUNK_SIZE)].blocks[From3Dto1D(p_x % CHUNK_SIZE, p_y % CHUNK_SIZE, p_z % CHUNK_SIZE)] = p_blockData;
}

std::array<uint8_t, 3> AmberCraft::World::From1Dto3D(uint16_t p_index)
{
	std::array<uint8_t, 3> result;
	result[2] = p_index / (WORLD_SIZE * WORLD_SIZE);
	p_index -= (std::get<2>(result) * (WORLD_SIZE * WORLD_SIZE));
	result[1] = p_index / WORLD_SIZE;
	result[0] = p_index % WORLD_SIZE;
	return result;
}

uint16_t AmberCraft::World::From3Dto1D(uint8_t p_x, uint8_t p_y, uint8_t p_z)
{
	return p_x + p_y * WORLD_SIZE + p_z * WORLD_SIZE * WORLD_SIZE;
}