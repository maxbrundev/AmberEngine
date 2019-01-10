#include "pch.h"

#include "World.h"

AmberCraft::World::World() : m_chunks(WORLD_ELEMENTS_COUNT)
{
	for (uint16_t i = 0; i < WORLD_ELEMENTS_COUNT; ++i)
	{
		m_chunks[i].Update();
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
		auto coordinates = From1Dto3D(i);
		chunkShader.SetUniformMat4("model", glm::translate(glm::mat4(1.0f), glm::vec3(coordinates[0] * CHUNK_SIZE, coordinates[1] * CHUNK_SIZE, coordinates[2] * CHUNK_SIZE)));
		m_chunks[i].Draw();
	}
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