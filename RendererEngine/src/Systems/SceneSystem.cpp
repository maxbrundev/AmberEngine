#include "pch.h"

#include "Systems/SceneSystem.h"

RenderEngine::Systems::SceneSystem::SceneSystem(Core::Context& p_context) : m_SceneContext(p_context)
{
	std::cout << "Scene Created" << std::endl;
}