/*
*******************************************************************************

Copyright (C) 2008-2018 OpenMW Developers
Copyright (C) 2020 SugarBombEngine Developers

This file is part of SugarBombEngine

SugarBombEngine is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

SugarBombEngine is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with SugarBombEngine. If not, see <http://www.gnu.org/licenses/>.

*******************************************************************************
*/

/// @file

//*****************************************************************************

#include "AppFrameworks/SbApplication/SbApplication.hpp"

#ifdef _WIN32
//#	ifndef WIN32_LEAN_AND_MEAN
//#		define WIN32_LEAN_AND_MEAN
//#	endif
#	include <windows.h>
	// makes __argc and __argv available on windows
//#	include <cstdlib>
#endif

//*****************************************************************************

#ifdef ANDROID
extern "C" int SDL_main(int argc, char **argv)
#else
int main(int argc, char **argv)
#endif
{
	//return wrapApplication(&runApplication, argc, argv, "OpenMW");
	return sbe::SbApplication::Main(argc, argv);
};

// Platform specific for Windows when there is no console built into the executable
// Windows will call the WinMain function instead of main in this case, the normal
// main function is then called with the __argc and __argv parameters
#if defined(_WIN32) // && !defined(_CONSOLE)
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	return main(__argc, __argv);
};
#endif