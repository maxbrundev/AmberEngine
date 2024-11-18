#include "pch.h"

#include "Core/Application.h"

#include <AmberEngine/Tools/Time/Clock.h>

#include "AmberEngine/Core/EditorAction.h"
#include "AmberEngine/Core/SceneSystem/SceneManager.h"
#include "AmberEngine/Core/SceneSystem/Scene.h"

static char filePath[1024] = "";

Core::Application::Application(const std::string& p_projectPath, const std::string& p_projectName) : 
m_context(p_projectPath, p_projectName),
m_editor(m_context),
isRunning(true)
{
}

void Core::Application::Initialize()
{
	//EDITOR_CONTEXT(sceneManager).GetCurrentScene()->CreateActor("test");
}

void Core::Application::Run()
{
	Tools::Time::Clock clock;

	while (IsRunning())
	{
		m_editor.PreUpdate();
		m_editor.Update(clock.GetDeltaTime());
		m_editor.PostUpdate();
		
		clock.Update();
	}
}

bool Core::Application::IsRunning() const
{
	return isRunning && m_context.window->IsActive();
}
