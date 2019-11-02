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

#include <cstdlib>

#ifdef _WIN32
#	include <windows.h>
#endif

#include "GameApp.hpp"
#include "SbSystemExternal.hpp"
#include "SbRenderSystemExternal.hpp"
#include "SbInputSystemExternal.hpp"
#include "SbSoundSystemExternal.hpp"
#include "SbGameFrameworkExternal.hpp"

//sbe::ISoundSystem *CreateSoundSystem();
//sbe::IGameFramework *CreateGameFramework();

// TODO: delete mpSoundSystem; mpSoundSystem = nullptr;
// TODO: delete mpFramework; mpFramework = nullptr;

sbe::ISystem *CreateSystem()
{
#ifndef SBE_SINGLE_BINARY
	static sbe::SbSystemExternal SbSystemModule;
	return SbSystemModule.GetSystem();
#else
	return new sbe::SbSystem::SbSystem();
#endif
};

sbe::IRenderSystem *CreateRenderSystem(sbe::ISystem &aSystem)
{
#ifndef SBE_SINGLE_BINARY
	static sbe::SbRenderSystemExternal SbRenderModule(aSystem);
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

sbe::ISoundSystem *CreateSoundSystem(sbe::ISystem &aSystem)
{
#ifndef SBE_SINGLE_BINARY
	static sbe::SbSoundSystemExternal SbSoundModule(aSystem);
	return SbSoundModule.GetSoundSystem();
#else
	return new sbe::SbSound::SbSound();
#endif
};

sbe::IGameFramework *CreateGameFramework(sbe::ISystem &aSystem)
{
#ifndef SBE_SINGLE_BINARY
	static sbe::SbGameFrameworkExternal SbGameFrameworkModule(aSystem);
	return SbGameFrameworkModule.GetGameFramework();
#else
	return new sbe::SbGameFramework::SbGameFramework();
#endif
};

int main(int argc, char **argv)
{
	sbe::ISystem *pSystem = CreateSystem();
	sbe::IGameFramework *pGameFramework = CreateGameFramework(*pSystem);
	sbe::ISoundSystem *pSoundSystem = CreateSoundSystem(*pSystem);
	sbe::IRenderSystem *pRenderSystem = CreateRenderSystem(*pSystem);
	sbe::IInputSystem *pInputSystem = CreateInputSystem(*pSystem);
	f3goaty::CGameApp App(pGameFramework, pSoundSystem, pRenderSystem, pInputSystem, pSystem, argc, argv);
	App.Run();
	return EXIT_SUCCESS;
};

#ifdef _WIN32
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	return main(__argc, __argv);
};
#endif