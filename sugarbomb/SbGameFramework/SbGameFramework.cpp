/*
*******************************************************************************

Copyright (C) 1993-2012 id Software LLC, a ZeniMax Media company.
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

In addition, SugarBombEngine is using id Tech 4 (BFG) pieces and thus
subject to certain additional terms (all header and source files which 
contains such pieces has this additional part appended to the license 
header). You should have received a copy of these additional terms 
stated in a separate file (LICENSE-idTech4) which accompanied the 
SugarBombEngine source code. If not, please request a copy in 
writing from id Software at the address below.

If you have questions concerning this license or the applicable additional
terms, you may contact in writing id Software LLC, c/o ZeniMax Media Inc.,
Suite 120, Rockville, Maryland 20850 USA.

*******************************************************************************
*/

/// @file

//*****************************************************************************

#include "SbGameFramework.hpp"

#include "CoreLibs/SbSystem/ISystem.hpp"

#include "SbGame/IGame.hpp"

#include "SbNetwork/INetworkSystem.hpp"

#include "CoreLibs/SbRenderer/IRenderSystem.hpp"

#include "CoreLibs/SbSound/ISoundSystem.hpp"

#include "SbSystemExternal.hpp"
#include "SbRenderSystemExternal.hpp"
#include "SbInputSystemExternal.hpp"
#include "SbSoundSystemExternal.hpp"
//*****************************************************************************

namespace sbe
{
class idUserCmdMgr
{
};

struct gameReturn_t
{
	int stub;
};
};

namespace sbe::SbGameFramework
{

SbGameFramework::SbGameFramework(IRenderSystem *apRenderSystem, ISoundSystem *apSoundSystem, INetworkSystem &aNetworkSystem, IGame &aGame, ISystem &aSystem)
	: mpRenderSystem(apRenderSystem), mpSoundSystem(apSoundSystem), mNetworkSystem(aNetworkSystem), mGame(aGame), mSystem(aSystem){}

void SbGameFramework::Init()
{
	
	mSystem = *CreateSystem();
	mSoundSystem = *CreateSoundSystem(mSystem);
	mRenderSystem = *CreateRenderSystem(mSystem);
	mInputSystem = *CreateInputSystem(mSystem);
	mNetworkSystem = *CreateNetworkSystem(mSystem);
	mPhysicsSystem = *CreatePhysicsSystem(mSystem);
	
	
	auto pDict{iniparser_load("FalloutPrefs.ini")}; // Fallout_default
	
	const char *sWindowTitle{"F3GOATY"};
	int nWindowWidth{1280};
	int nWindowHeight{600};
	bool bWindowFullScreen{false};
	
	if(pDict != nullptr)
	{
		nWindowWidth = iniparser_getint(pDict, "Display:iSize W", 1280);
		nWindowHeight = iniparser_getint(pDict, "Display:iSize H", 600);
		bWindowFullScreen = iniparser_getboolean(pDict, "Display:bFull Screen", false);
		bWindowBorder = iniparser_getboolean(pDict, "Display:bBorder", true);
	};
	
	IWindow::Props WinProps(.msTitle = sWindowTitle, .mnWidth = nWindowWidth, .mnHeight = nWindowHeight, .mbFullScreen = bWindowFullScreen);
	
	mSoundSystem.Init();
	mRenderSystem.Init(WinProps);
	mInputSystem.Init();
	mNetworkSystem.Init();
	mPhysicsSystem.Init();
	
	mGame.Init();
	
	CreateMainMenu();
};

void SbGameFramework::Shutdown()
{
	//if( com_shuttingDown) // TODO
		//return; // TODO

	//com_shuttingDown = true; // TODO
	
	// Kill any pending saves...
	printf( "session->GetSaveGameManager().CancelToTerminate();\n" );
	//session->GetSaveGameManager().CancelToTerminate(); // TODO
	
	// shutdown the script debugger
	//DebuggerServerShutdown();
	
	//if(aviCaptureMode) // TODO
	{
		printf("EndAVICapture();\n");
		//EndAVICapture(); // TODO
	};
	
	printf("Stop();\n");
	//Stop(); // TODO
	
	printf("CleanupShell();\n");
	CleanupShell();
	
	mGame.Shutdown();
	
	mPhysicsSystem.Shutdown();
	mNetworkSystem.Shutdown();
	mSoundSystem.Shutdown();
	
};

void SbGameFramework::Frame(float afTimeStep)
{
	mpSoundSystem->Update(GetTimeStep());
	
	idUserCmdMgr UserCmdMgrStub;
	gameReturn_t GameReturnStub;
	
	mGame.RunFrame(UserCmdMgrStub, GameReturnStub);
	mGame.ClientRunFrame(UserCmdMgrStub, false, GameReturnStub);
};

void SbGameFramework::Draw()
{
	if(mGame.Shell_IsActive())
	{
		bool bGameDraw{mGame.Draw(0)};
		if(!bGameDraw)
		{
		};
		mGame.Shell_Render();
	}
	else
	{
	};
};

/*
========================
idCommonLocal::CreateMainMenu
========================
*/
void SbGameFramework::CreateMainMenu()
{
	//if(mpGame != nullptr) // TODO
	{
		// note which media we are going to need to load
		//declManager->BeginLevelLoad(); // TODO
		mpRenderSystem->BeginLevelLoad();
		mpSoundSystem->BeginLevelLoad();
		//uiManager->BeginLevelLoad(); // TODO
		
		// create main inside an "empty" game level load - so assets get
		// purged automagically when we transition to a "real" map
		mGame.Shell_CreateMenu(false);
		mGame.Shell_Show(true);
		mGame.Shell_SyncWithSession();
		
		// load
		mpRenderSystem->EndLevelLoad();
		mpSoundSystem->EndLevelLoad();
		//declManager->EndLevelLoad(); // TODO
		//uiManager->EndLevelLoad(""); // TODO
	};
};

/*
==============
idCommonLocal::StartMainMenu
==============
*/
void SbGameFramework::StartMenu(bool abPlayIntro)
{
	if(/*mGame &&*/ mGame.Shell_IsActive())
		return;
	
	//if(readDemo) // TODO
	{
		// if we're playing a demo, esc kills it
		//UnloadMap(); // TODO
	};
	
	//if(mGame)
	{
		mGame.Shell_Show(true);
		mGame.Shell_SyncWithSession();
	};
	
	//console->Close(); // TODO
};

/*
===============
idCommonLocal::ExitMenu
===============
*/
void SbGameFramework::ExitMenu()
{
	//if(mGame)
		mGame.Shell_Show(false);
};

/*
=================
idCommonLocal::CleanupShell
=================
*/
void SbGameFramework::CleanupShell()
{
	mGame.Shell_Cleanup();
};

ISystem *SbGameFramework::CreateSystem()
{
#ifndef SBE_SYSTEM_HARD_LINKED
	static SbSystemExternal SbSystemModule;
	return SbSystemModule.GetSystem();
#else
	return new SbSystem::SbSystem();
#endif
};

IRenderSystem *SbGameFramework::CreateRenderSystem(ISystem &aSystem)
{
#ifndef SBE_RENDER_HARD_LINKED
	static SbRenderSystemExternal SbRenderModule(aSystem);
	return SbRenderModule.GetRenderSystem();
#else
	return new SbRenderer::SbRenderSystem(aSystem);
#endif
};

IInputSystem *SbGameFramework::CreateInputSystem(ISystem &aSystem)
{
#ifndef SBE_INPUT_HARD_LINKED
	static SbInputSystemExternal SbInputModule(aSystem);
	return SbInputModule.GetInputSystem();
#else
	return new SbInput::SbInputSystem(aSystem);
#endif
};

ISoundSystem *SbGameFramework::CreateSoundSystem(ISystem &aSystem)
{
#ifndef SBE_SOUND_HARD_LINKED
	static SbSoundSystemExternal SbSoundModule(aSystem);
	return SbSoundModule.GetSoundSystem();
#else
	return new SbSound::SbSoundSystem(aSystem);
#endif
};

INetworkSystem *SbGameFramework::CreateNetworkSystem(ISystem &aSystem)
{
#ifndef SBE_NETWORK_HARD_LINKED
	static SbNetworkSystemExternal SbNetworkModule(aSystem);
	return SbNetworkModule.GetNetworkSystem();
#else
	return new SbNetwork::SbNetworkSystem(aSystem);
#endif
};

IPhysicsSystem *SbGameFramework::CreatePhysicsSystem(ISystem &aSystem)
{
#ifndef SBE_PHYSICS_HARD_LINKED
	static SbPhysicsSystemExternal SbPhysicsModule(aSystem);
	return SbPhysicsModule.GetPhysicsSystem();
#else
	return new SbPhysics::SbPhysicsSystem(aSystem);
#endif
};

}; // namespace sbe::SbGameFramework