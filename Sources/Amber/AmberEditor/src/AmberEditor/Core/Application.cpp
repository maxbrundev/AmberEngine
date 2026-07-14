#include "Amberpch.h"

#include "AmberEditor/Core/Application.h"

#include <AmberTools/Time/Clock.h>

#include "AmberEditor/Core/EditorAction.h"
#include "AmberCore/SceneSystem/SceneManager.h"
#include "AmberCore/SceneSystem/Scene.h"

static char filePath[1024] = "";

AmberEditor::Core::Application::Application(const std::string& p_projectPath, const std::string& p_projectName) : 
m_context(p_projectPath, p_projectName),
m_editor(m_context),
isRunning(true)
{
}

void AmberEditor::Core::Application::Initialize()
{
	//EDITOR_CONTEXT(sceneManager).GetCurrentScene()->CreateActor("test");
}

void AmberEditor::Core::Application::Run()
{
	AmberTools::Time::Clock clock;

	while (IsRunning())
	{
		m_editor.PreUpdate();
		m_editor.Update(clock.GetDeltaTime());
		m_editor.PostUpdate();
		
		clock.Update();
	}
}

bool AmberEditor::Core::Application::IsRunning() const
{
	return isRunning && m_context.window->IsActive();
}
