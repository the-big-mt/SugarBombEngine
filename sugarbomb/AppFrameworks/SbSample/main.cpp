/*
*******************************************************************************

Copyright (C) 2019-2020 SugarBombEngine Developers

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

#include "SbSystemExternal.hpp"
#include "SbRenderSystemExternal.hpp"
#include "SbInputSystemExternal.hpp"

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
	static sbe::SbSystemExternal SbSystemModule;
	return SbSystemModule.GetSystem();
#else
	return new sbe::SbSystem::SbSystem();
#endif
};

sbe::IRenderSystem *CreateRenderSystem(const char *asModuleName, sbe::ISystem &aSystem)
{
#ifndef SBE_SINGLE_BINARY
	static sbe::SbRenderSystemExternal SbRenderModule(asModuleName, aSystem);
	return SbRenderModule.GetRenderSystem();
#else
	return new sbe::SbRenderer::SbRenderSystem();
#endif
};

sbe::IInputSystem *CreateInputSystem(sbe::ISystem &aSystem)
{
#ifndef SBE_SINGLE_BINARY
	static sbe::SbInputSystemExternal SbInputModule(aSystem);
	return SbInputModule.GetInputSystem();
#else
	return new sbe::SbInput::SbInputSystem();
#endif
};

int SbApplication::Main(int argc, char **argv)
{
	// Render module name to load
	const char *sRenderModuleName{"SbGLCoreRenderer"};
	
	sbe::ISystem &System = *CreateSystem();
	sbe::IRenderSystem &RenderSystem = *CreateRenderSystem(sRenderModuleName, System);
	sbe::IInputSystem &InputSystem = *CreateInputSystem(System);
	
	const char *sWindowTitle{"SugarBombEngine Sample App"};
	int nWindowWidth{1280};
	int nWindowHeight{600};
	bool bWindowFullScreen{false};
	
	SbClientApp App(sWindowTitle, nWindowWidth, nWindowHeight, bWindowFullScreen, RenderSystem, InputSystem, System, argc, argv);
	App.Run();
	return EXIT_SUCCESS;
};