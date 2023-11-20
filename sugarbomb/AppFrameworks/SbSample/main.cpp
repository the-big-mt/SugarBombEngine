/*
*******************************************************************************

Copyright (C) 2019-2020, 2022 SugarBombEngine Developers

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

namespace sbe
{

constexpr auto DEFAULT_WINDOW_TITLE{"SugarBombEngine Sample App"};
constexpr auto DEFAULT_WINDOW_WIDTH{1280};
constexpr auto DEFAULT_WINDOW_HEIGHT{600};
constexpr auto DEFAULT_WINDOW_FULLSCREEN{false};

//extern sbe::ISystem *CreateSystem();
//extern sbe::IRenderSystem *CreateRenderSystem();
//extern sbe::IInputSystem *CreateInputSystem();

// TODO: delete mpInputSystem; mpInputSystem = nullptr;
// TODO: delete mpRenderSystem; mpRenderSystem = nullptr;
// TODO: delete mpSystem; mpSystem = nullptr;

ISystem *CreateSystem()
{
#ifndef SBE_SYSTEM_HARD_LINKED
	static SbSystemExternal SbSystemModule;
	return SbSystemModule.GetSystem();
#else
	return new SbSystem::SbSystem();
#endif
};

IRenderSystem *CreateRenderSystem(const char *asModuleName, ISystem &aSystem)
{
#ifndef SBE_RENDERER_HARD_LINKED
	static SbRenderSystemExternal SbRenderModule(asModuleName);
	return SbRenderModule.GetRenderSystem();
#else
	return new SbRenderer::SbRenderSystem(aSystem);
#endif
};

IInputSystem *CreateInputSystem(ISystem &aSystem)
{
#ifndef SBE_INPUT_HARD_LINKED
	static SbInputSystemExternal SbInputModule();
	return SbInputModule.GetInputSystem();
#else
	return new SbInput::SbInputSystem(aSystem);
#endif
};

int SbApplication::Main(int argc, char **argv)
{
	// Render module name to load
	const char *sRendererModuleName{"SbGLCoreRenderer"};
	
	ISystem &System = *CreateSystem();
	IRenderSystem &RenderSystem = *CreateRenderSystem(sRendererModuleName, System);
	IInputSystem &InputSystem = *CreateInputSystem(System);
	
	const char *sWindowTitle{DEFAULT_WINDOW_TITLE};
	int nWindowWidth{DEFAULT_WINDOW_WIDTH};
	int nWindowHeight{DEFAULT_WINDOW_HEIGHT};
	bool bWindowFullScreen{DEFAULT_WINDOW_FULLSCREEN};
	
	SbClientApp App(sWindowTitle, nWindowWidth, nWindowHeight, bWindowFullScreen, RenderSystem, InputSystem, System, argc, argv);
	App.Run();
	return EXIT_SUCCESS;
};

}; // namespace sbe