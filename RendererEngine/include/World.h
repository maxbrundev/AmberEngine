#pragma once

#include "Chunk.h"
#include "Managers/RenderingManager.h"

#define WORLD_SIZE 5
#define WORLD_ELEMENTS_COUNT WORLD_SIZE * WORLD_SIZE * WORLD_SIZE

namespace AmberCraft
{
	class World
	{
	private:
		std::vector<Chunk> m_chunks;

	public:
		World();
		~World() = default;

		void Draw(RenderEngine::Managers::RenderingManager& p_renderingManager);
		std::array<uint8_t, 3> From1Dto3D(uint16_t p_index);
	};
}