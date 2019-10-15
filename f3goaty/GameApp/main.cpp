/// @file

#include <cstdlib>

#ifdef _WIN32
#	include <windows.h>
#endif

#include "GameApp.hpp"

int main(int argc, char **argv)
{
	f3goaty::CGameApp App(argc, argv);
	App.Run();
	return EXIT_SUCCESS;
};

#ifdef _WIN32
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	return main(__argc, __argv);
};
#endif