/*
*******************************************************************************

Copyright (C) 1993-2012 id Software LLC, a ZeniMax Media company.
Copyright (C) 2012-2014 Robert Beckebans
Copyright (C) 2019 BlackPhrase

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

#include "F3ClientApp.hpp"
#include "framework/IGameFramework.hpp"

namespace f3bfg
{

CClientApp::CClientApp(int argc, char **argv) : SbClientApp(argc, argv)
{
};

CClientApp::~CClientApp()
{
	Shutdown();
	
	ShutdownFrameworkModule();
	ShutdownSoundSystem();
};

void CClientApp::PostInit()
{
	// start the sound system, but don't do any hardware operations yet
	InitSoundSystem();
	
	InitFrameworkModule();
	
	mpFramework->Init(0, nullptr, lpCmdLine);
	
#if TEST_FPU_EXCEPTIONS != 0
	mpSys->Printf(Sys_FPU_GetState());
#endif
};

void CClientApp::PostFrame()
{
	// run the game
	mpFramework->Frame();
};

bool CClientApp::PreLogicUpdate()
{
	return true;
};

void CClientApp::LogicUpdate()
{
	
};

void CClientApp::Shutdown()
{
	// kill sound first
	printf( "soundSystem->StopAllSounds();\n" );
	soundSystem->StopAllSounds();
	
	mpFramework->Shutdown();
	
	// shut down the sound system
	printf( "soundSystem->Shutdown();\n" );
	soundSystem->Shutdown();
};

/*
=================
idCommonLocal::LoadSoundModule
=================
*/
void CClientApp::InitSoundSystem()
{
#ifndef SBE_SINGLE_BINARY
	char			dllPath[ sbe::MAX_OSPATH ];
	
	sbe::soundImport_t	soundImport;
	sbe::soundExport_t	soundExport;
	sbe::GetSoundAPI_t	GetSoundAPI;
	
	mpFileSystem->FindDLL( "SbSound", dllPath/*, true*/ );
	
	if( !dllPath[ 0 ] )
	{
		mpSys->FatalError( "couldn't find sound dynamic library" );
		return;
	};
	mpSys->DPrintf( "Loading sound DLL: '%s'\n", dllPath );
	soundDLL = mpSys->DLL_Load( dllPath );
	if( !soundDLL )
	{
		mpSys->FatalError( "couldn't load sound dynamic library" );
		return;
	};
	
	const char* functionName = "GetSoundAPI";
	GetSoundAPI = ( GetSoundAPI_t ) mpSys->DLL_GetProcAddress( soundDLL, functionName );
	if( !GetSoundAPI )
	{
		mpSys->DLL_Unload( soundDLL );
		soundDLL = 0;
		mpSys->FatalError( "couldn't find sound DLL API" );
		return;
	};
	
	soundImport.version					= sbe::SOUND_API_VERSION;
	soundImport.sys						= mpSys;
	soundImport.common					= ::common;
	soundImport.cmdSystem				= ::cmdSystem;
	soundImport.cvarSystem				= ::cvarSystem;
	soundImport.fileSystem				= mpFileSystem;
	soundImport.renderSystem				= renderSystem;
	soundImport.declManager				= ::declManager;
	
	soundExport							= *GetSoundAPI( &soundImport );
	
	if( soundExport.version != SOUND_API_VERSION )
	{
		mpSys->DLL_Unload( soundDLL );
		soundDLL = 0;
		common->FatalError( "wrong sound DLL API version" );
		return;
	};
	
	soundSystem								= soundExport.soundSystem;
	
#endif
	
	// initialize the sound object
	if( soundSystem != nullptr )
		soundSystem->Init();
};

/*
=================
idCommonLocal::UnloadGameModule
=================
*/
void CClientApp::ShutdownSoundSystem()
{
	// shut down the sound object
	if( soundSystem != nullptr )
	{
		soundSystem->Shutdown();
		soundSystem = nullptr;
	};
	
#ifndef SBE_SINGLE_BINARY
	if( soundDLL )
	{
		mpSys->DLL_Unload( soundDLL );
		soundDLL = 0;
	};
#endif
};

/*
=================
idCommonLocal::LoadSoundModule
=================
*/
void CClientApp::InitFrameworkModule()
{
#ifndef SBE_SINGLE_BINARY
	char			dllPath[ sbe::MAX_OSPATH ];
	
	sbe::frameworkImport_t	frameworkImport;
	sbe::frameworkExport_t	frameworkExport;
	sbe::GetFrameworkAPI_t	GetFrameworkAPI;
	
	mpFileSystem->FindDLL( "SbFramework", dllPath/*, true*/ );
	
	if( !dllPath[ 0 ] )
	{
		mpSys->FatalError( "couldn't find framework dynamic library" );
		return;
	};
	mpSys->DPrintf( "Loading framework DLL: '%s'\n", dllPath );
	frameworkDLL = mpSys->DLL_Load( dllPath );
	if( !frameworkDLL )
	{
		mpSys->FatalError( "couldn't load framework dynamic library" );
		return;
	};
	
	const char* functionName = "GetFrameworkAPI";
	GetFrameworkAPI = ( GetFrameworkAPI_t ) mpSys->DLL_GetProcAddress( frameworkDLL, functionName );
	if( !GetFrameworkAPI )
	{
		mpSys->DLL_Unload( frameworkDLL );
		frameworkDLL = 0;
		mpSys->FatalError( "couldn't find framework DLL API" );
		return;
	};
	
	frameworkImport.version					= sbe::FRAMEWORK_API_VERSION;
	frameworkImport.sys						= mpSys;
	frameworkImport.cmdSystem				= mpCmdSystem.get();
	frameworkImport.cvarSystem				= mpCVarSystem.get();
	frameworkImport.fileSystem				= mpFileSystem;
	frameworkImport.renderSystem				= renderSystem;
	frameworkImport.declManager				= ::declManager;
	
	frameworkExport							= *GetFrameworkAPI( &frameworkImport );
	
	if( frameworkExport.version != sbe::FRAMEWORK_API_VERSION )
	{
		mpSys->DLL_Unload( frameworkDLL );
		frameworkDLL = 0;
		mpSys->FatalError( "wrong framework DLL API version" );
		return;
	};
	
	mpFramework								= frameworkExport.framework;
	
#endif
	
	// initialize the sound object
	if( mpFramework != nullptr )
		mpFramework->Init();
};

/*
=================
idCommonLocal::UnloadFrameworkModule
=================
*/
void CClientApp::ShutdownFrameworkModule()
{
	// shut down the framework object
	if( mpFramework != nullptr )
	{
		mpFramework->Shutdown();
		mpFramework = nullptr;
	};
	
#ifndef SBE_SINGLE_BINARY
	if( frameworkDLL )
	{
		Sys_DLL_Unload( frameworkDLL );
		frameworkDLL = 0;
	};
#endif
};

}; // namespace f3bfg