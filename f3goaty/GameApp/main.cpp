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

#include "iniparser.h"

#include "GameApp.hpp"
#include "SbSystemExternal.hpp"
#include "SbRenderSystemExternal.hpp"
#include "SbInputSystemExternal.hpp"
#include "SbSoundSystemExternal.hpp"
#include "SbGameFrameworkExternal.hpp"
#include "SbGameExternal.hpp"

//*****************************************************************************

//sbe::ISoundSystem *CreateSoundSystem();
//sbe::IGameFramework *CreateGameFramework();

// TODO: delete mpSoundSystem; mpSoundSystem = nullptr;
// TODO: delete mpFramework; mpFramework = nullptr;

sbe::ISystem *CreateSystem()
{
#ifndef SBE_SYSTEM_HARD_LINKED
	static sbe::SbSystemExternal SbSystemModule;
	return SbSystemModule.GetSystem();
#else
	return new sbe::SbSystem::SbSystem();
#endif
};

sbe::IRenderSystem *CreateRenderSystem(sbe::ISystem &aSystem)
{
#ifndef SBE_RENDER_HARD_LINKED
	static sbe::SbRenderSystemExternal SbRenderModule(aSystem);
	return SbRenderModule.GetRenderSystem();
#else
	return new sbe::SbRenderer::SbRenderSystem(aSystem);
#endif
};

sbe::IInputSystem *CreateInputSystem(sbe::ISystem &aSystem)
{
#ifndef SBE_INPUT_HARD_LINKED
	static sbe::SbInputSystemExternal SbInputModule(aSystem);
	return SbInputModule.GetInputSystem();
#else
	return new sbe::SbInput::SbInputSystem(aSystem);
#endif
};

sbe::ISoundSystem *CreateSoundSystem(sbe::ISystem &aSystem)
{
#ifndef SBE_SOUND_HARD_LINKED
	static sbe::SbSoundSystemExternal SbSoundModule(aSystem);
	return SbSoundModule.GetSoundSystem();
#else
	return new sbe::SbSound::SbSoundSystem(aSystem);
#endif
};

sbe::INetworkSystem *CreateNetworkSystem(sbe::ISystem &aSystem)
{
#ifndef SBE_NETWORK_HARD_LINKED
	static sbe::SbNetworkSystemExternal SbNetworkModule(aSystem);
	return SbNetworkModule.GetNetworkSystem();
#else
	return new sbe::SbNetwork::SbNetworkSystem(aSystem);
#endif
};

sbe::IPhysicsSystem *CreatePhysicsSystem(sbe::ISystem &aSystem)
{
#ifndef SBE_PHYSICS_HARD_LINKED
	static sbe::SbPhysicsSystemExternal SbPhysicsModule(aSystem);
	return SbPhysicsModule.GetPhysicsSystem();
#else
	return new sbe::SbPhysics::SbPhysicsSystem(aSystem);
#endif
};

sbe::IGameFramework *CreateGameFramework(sbe::ISystem &aSystem, sbe::IRenderSystem *apRenderSystem, sbe::ISoundSystem *apSoundSystem)
{
#ifndef SBE_GAMEFRAMEWORK_HARD_LINKED
	static sbe::SbGameFrameworkExternal SbGameFrameworkModule(aSystem, apRenderSystem, apSoundSystem);
	return SbGameFrameworkModule.GetGameFramework();
#else
	return new sbe::SbGameFramework::SbGameFramework();
#endif
};

sbe::IGame *CreateGame(sbe::ISystem &aSystem)
{
#ifndef SBE_GAME_HARD_LINKED
	static sbe::SbGameExternal SbGameModule(aSystem);
	return SbGameModule.GetGame();
#else
	return new sbe::SbGame::SbGame();
#endif
};

int sbe::SbApplication::Main(int argc, char **argv)
{
	sbe::ISystem &System = *CreateSystem();
	sbe::ISoundSystem *pSoundSystem = CreateSoundSystem(System);
	sbe::IRenderSystem *pRenderSystem = CreateRenderSystem(System);
	sbe::IInputSystem *pInputSystem = CreateInputSystem(System);
	sbe::INetworkSystem *pNetworkSystem = CreateNetworkSystem(System);
	sbe::IPhysicsSystem *pPhysicsSystem = CreatePhysicsSystem(System);
	sbe::IGameFramework *pGameFramework = CreateGameFramework(System, pRenderSystem, pSoundSystem, pNetworkSystem, pPhysicsSystem);
	sbe::IGame &Game = CreateGame(System);
	
	
	f3goaty::CGameApp App(sWindowTitle, nWindowWidth, nWindowHeight, bWindowFullScreen, pGameFramework, pSoundSystem, pRenderSystem, pInputSystem, pSystem, argc, argv);
	App.Run();
	return EXIT_SUCCESS;
};