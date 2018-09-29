// RendererEngine.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"

#include "Systems/RenderSystem.h"

int main()
{
	RenderEngine::Systems::RenderSystem Renderer;

	Renderer.Run();

	return 0;
}