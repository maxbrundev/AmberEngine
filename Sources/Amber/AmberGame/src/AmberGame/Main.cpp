#include "Amberpch.h"

#include "AmberGame/Core/Application.h"

#ifdef _DEBUG
int main()
#else
#undef APIENTRY
#include <Windows.h>
INT WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR lpCmdLine, INT nCmdShow)
#endif
{
	AmberGame::Core::Application app;
	app.Run();

	return EXIT_SUCCESS;
}
