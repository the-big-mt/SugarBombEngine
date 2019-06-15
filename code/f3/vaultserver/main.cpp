// Copyright (C) 2008-2018 OpenMW Developers
// Copyright (C) 2019 BlackPhrase

/// @file

#include "F3ServerApp.hpp"

#ifdef _WIN32
#	ifndef WIN32_LEAN_AND_MEAN
#		define WIN32_LEAN_AND_MEAN
#	endif
#	include <windows.h>
	// makes __argc and __argv available on windows
//#	include <cstdlib>
#endif

#ifdef ANDROID
extern "C" int SDL_main(int argc, char **argv)
#else
int main(int argc, const char **argv)
#endif
{
	//return wrapApplication(&runApplication, argc, argv, "OpenMW");

	f3bfg::CServerApp App(argc, argv);
	
	App.Run();

	// never gets here
	return EXIT_SUCCESS;
};

// Platform specific for Windows when there is no console built into the executable
// Windows will call the WinMain function instead of main in this case, the normal
// main function is then called with the __argc and __argv parameters
#if defined(_WIN32) && !defined(_CONSOLE)
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    return main(__argc, __argv);
};
#endif