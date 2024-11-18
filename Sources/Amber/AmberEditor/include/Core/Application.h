#pragma once

#include <AmberEngine/Core/Context.h>
#include <AmberEngine/Core/Editor.h>

namespace Core
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
		AmberEngine::Core::Context m_context;
		AmberEngine::Core::Editor m_editor;

		bool isRunning;
	};
}