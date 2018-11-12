// RendererEngine.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"

#include "Systems/AmberRenderer.h"

int main()
{
	RenderEngine::Systems::AmberRenderer Renderer;
	Renderer.Setup();
	Renderer.Run();

	return 0;
}