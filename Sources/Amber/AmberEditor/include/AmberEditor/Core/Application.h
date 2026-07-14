#pragma once

#include <AmberEditor/Core/Context.h>
#include <AmberEditor/Core/Editor.h>

namespace AmberEditor::Core
{
	class Application
	{
	public:
		Application(const std::string& p_projectPath, const std::string& p_projectName);
		~Application() = default;

		void Initialize();
		void Run();
		bool IsRunning() const;

	private:
		AmberEditor::Core::Context m_context;
		AmberEditor::Core::Editor m_editor;

		bool isRunning;
	};
}