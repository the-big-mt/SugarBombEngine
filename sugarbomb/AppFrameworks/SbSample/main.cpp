/*
*******************************************************************************

Copyright (C) 2019 SugarBombEngine Developers

This file is part of SugarBombEngine

SugarBombEngine is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

SugarBombEngine is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with SugarBombEngine. If not, see <http://www.gnu.org/licenses/>.

*******************************************************************************
*/

/// @file

//*****************************************************************************

#include <cstdlib>

#ifdef _WIN32
#	include <windows.h>
#endif

#include "AppFrameworks/SbClientApp/SbClientApp.hpp"

//*****************************************************************************

//sbe::ISystem *CreateSystem();
//sbe::IRenderSystem *CreateRenderSystem();
//sbe::IInputSystem *CreateInputSystem();

// TODO: delete mpInputSystem; mpInputSystem = nullptr;
// TODO: delete mpRenderSystem; mpRenderSystem = nullptr;

sbe::ISystem *CreateSystem()
{
#ifndef SBE_SINGLE_BINARY
	static sbe::SbSystemExternal SbSystemModule();
	return SbSystemModule.GetSystem();
#else
	return new sbe::SbSystem::SbSystem();
#endif
};

sbe::IRenderSystem *SbClientApp::CreateRenderSystem()
{
#ifndef SBE_SINGLE_BINARY
	return new sbe::SbRenderSystemExternal(*mpSystem);
#else
	return new sbe::SbRenderer::SbRenderSystem();
#endif
};

sbe::IInputSystem *SbClientApp::CreateInputSystem()
{
#ifndef SBE_SINGLE_BINARY
	return new sbe::SbInputSystemExternal(*mpSystem);
#else
	return new sbe::SbInput::SbInputSystem();
#endif
};

int main(int argc, char **argv)
{
	SbClientApp App(CreateRenderSystem(), CreateInputSystem(), CreateSystem(), argc, argv);
	App.Run();
	return EXIT_SUCCESS;
};

#ifdef _WIN32
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	return main(__argc, __argv);
};
#endif