// RendererEngine.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"

#include "Systems/Application.h"

int main()
{
	RenderEngine::Systems::Application Renderer;
	Renderer.Setup();
	Renderer.Run();

	return 0;
}