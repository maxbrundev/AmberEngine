#include "pch.h"

#include <filesystem>
#include <fstream>

#include <AmberEngine/Tools/Utils/Defines.h>

#include "Core/Application.h"
#include "AmberEngine/Core/ProjectHub.h"
#include "AmberEngine/Tools/Utils/String.h"

#undef APIENTRY
#include "Windows.h"

FORCE_DEDICATED_GPU

/*#define EDITOR_CPP
#define PROJECTS_FILE std::string(std::string(getenv("APPDATA")) + "\\AmberTech\\ABEditor\\projects.ini")

void CreateProject(const std::string& p_path)
{
	if (!std::filesystem::exists(p_path))
	{
		std::filesystem::create_directory(p_path);
		std::filesystem::create_directory(p_path + "Assets\\");
		std::filesystem::create_directory(p_path + "Scripts\\");
		std::ofstream projectFile(p_path + '\\' + AmberEngine::Tools::Utils::PathParser::GetElementName(std::string(p_path.data(), p_path.data() + p_path.size() - 1)) + ".abproject");
	}
}

void RegisterProject(const std::string& p_path)
{
	bool pathAlreadyRegistered = false;

	{
		std::string line;
		std::ifstream myfile(PROJECTS_FILE);
		if (myfile.is_open())
		{
			while (getline(myfile, line))
			{
				if (line == p_path)
				{
					pathAlreadyRegistered = true;
					break;
				}
			}
			myfile.close();
		}
	}

	if (!pathAlreadyRegistered)
	{
		std::ofstream projectsFile(PROJECTS_FILE, std::ios::app);
		projectsFile << p_path << std::endl;
	}
}

std::tuple<bool, std::string, std::string> OpenProject(const std::string& p_path)
{
	bool isReady = std::filesystem::exists(p_path);
	std::string path;
	std::string projectName;
	if (!isReady)
	{

		//AmberEngine::Windowing::Dialogs::MessageBox errorMessage("Project not found", "The selected project does not exists", AmberEngine::Windowing::Dialogs::MessageBox::EMessageType::ERROR, AmberEngine::Windowing::Dialogs::MessageBox::EButtonLayout::OK);
	}
	else
	{
		path = p_path;
		projectName = AmberEngine::Tools::Utils::PathParser::GetElementName(path);
	}

	return { isReady, path, projectName };

}*/

void UpdateWorkingDirectory(const std::string& p_executablePath)
{
	if (!IsDebuggerPresent())
	{
		std::filesystem::current_path(AmberEngine::Tools::Utils::PathParser::GetContainingFolder(p_executablePath));
	}
}

int main(int argc, char** argv);
static void TryRun(const std::string& projectPath, const std::string& projectName);


#ifndef _DEBUG
INT WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR lpCmdLine, INT nCmdShow)
{
	main(__argc, __argv);
}
#endif

int main(int argc, char** argv)
{
	UpdateWorkingDirectory(argv[0]);

	bool ready = false;
	std::string projectPath;
	std::string projectName;

	{
/*#ifdef EDITOR_CPP

		std::string path = "cppTest\\";
		CreateProject(path);
		RegisterProject(path);
		std::tie(ready, projectPath, projectName) = OpenProject(path);
#else*/
		AmberEngine::Core::ProjectHub hub;

		if (argc < 2)
		{
			// No project file given as argument ==> Open the ProjectHub
			std::tie(ready, projectPath, projectName) = hub.Run();
		}
		else
		{
			// Project file given as argument ==> Open the project
			std::string projectFile = argv[1];

			if (AmberEngine::Tools::Utils::PathParser::GetExtension(projectFile) == "abproject")
			{
				ready = true;
				projectPath = AmberEngine::Tools::Utils::PathParser::GetContainingFolder(projectFile);
				projectName = AmberEngine::Tools::Utils::PathParser::GetElementName(projectFile);
				AmberEngine::Tools::Utils::String::Replace(projectName, ".abproject", "");
			}

			hub.RegisterProject(projectPath);
		}
//#endif
	}

	if (ready)
		TryRun(projectPath, projectName);

	return EXIT_SUCCESS;
}


static void TryRun(const std::string& projectPath, const std::string& projectName)
{
	auto errorEvent = [](AmberEngine::Context::EDeviceError, std::string errMsg)
	{
		errMsg = "AmberEngine requires OpenGL 4.3 or newer.\r\n" + errMsg;
		MessageBox(0, errMsg.c_str(), "Amber", MB_OK | MB_ICONSTOP);
	};

	std::unique_ptr<Core::Application> app;

	try
	{
		auto listenerId = AmberEngine::Context::Device::ErrorEvent += errorEvent;
		app = std::make_unique<Core::Application>(projectPath, projectName);
		AmberEngine::Context::Device::ErrorEvent -= listenerId;
	}
	catch (...) {}

	if (app)
	{
		app->Initialize();
		app->Run();
	}
}