/*
===========================================================================

Doom 3 BFG Edition GPL Source Code
Copyright (C) 1993-2012 id Software LLC, a ZeniMax Media company.
Copyright (C) 2012-2014 Robert Beckebans
Copyright (C) 2019 BlackPhrase

This file is part of the Doom 3 BFG Edition GPL Source Code ("Doom 3 BFG Edition Source Code").

Doom 3 BFG Edition Source Code is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

Doom 3 BFG Edition Source Code is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with Doom 3 BFG Edition Source Code.  If not, see <http://www.gnu.org/licenses/>.

In addition, the Doom 3 BFG Edition Source Code is also subject to certain additional terms. You should have received a copy of these additional terms immediately following the terms and conditions of the GNU General Public License which accompanied the Doom 3 BFG Edition Source Code.  If not, please request a copy in writing from id Software at the address below.

If you have questions concerning this license or the applicable additional terms, you may contact in writing id Software LLC, c/o ZeniMax Media Inc., Suite 120, Rockville, Maryland 20850 USA.

===========================================================================
*/

/// @file

#include "SbClientApp.hpp"
#include "framework/CmdSystem.h"
#include "framework/CVarSystem.h"
#include "sys/ISys.hpp"
#include "sys/IFileSystem.hpp"
#include "renderer/IRenderSystem.hpp"
#include "SbInput/IInputSystem.hpp"

SbClientApp::~SbClientApp()
{
	ShutdownInputSystem();
	ShutdownRenderSystem();
	
	// only shut down the log file after all output is done
	printf( "CloseLogFile();\n" );
	CloseLogFile();
	
	// shut down the file system
	printf( "fileSystem->Shutdown( false );\n" );
	mpFileSystem->Shutdown( false );
	
	ShutdownSystemModule();
	
	// shut down the cvar system
	printf( "cvarSystem->Shutdown();\n" );
	mpCVarSystem->Shutdown();
	
	// shut down the console command system
	printf( "cmdSystem->Shutdown();\n" );
	mpCmdSystem->Shutdown();
};

bool SbClientApp::Init()
{
	mpCmdSystem = std::make_unique<idCmdSystemLocal>();
	mpCVarSystem = std::make_unique<idCVarSystemLocal>();
	
	// init console command system
	mpCmdSystem->Init();
	
	// init CVar system
	mpCVarSystem->Init();
	
	InitSystemModule();
	
	if( Sys_AlreadyRunning() )
		mpSys->Quit();
	
	// initialize processor specific SIMD implementation
	InitSIMD();
	
	// initialize the file system
	mpFileSystem->Init();
	
	CreateMainWindow();
	
	InitRenderSystem();
	InitInputSystem();

	// init OpenGL, which will open a window and connect sound and input hardware
	renderSystem->InitOpenGL();
	
	// initialize the renderSystem data structures
	renderSystem->Init();
	
	return true;
};

void SbClientApp::Frame()
{
	if(PreInputUpdate())
	{
		InputUpdate();
		PostInputUpdate();
	};
	
	if(PreLogicUpdate())
	{
		LogicUpdate();
		PostLogicUpdate();
	};
	
	if(PreRender())
	{
		Render();
		PostRender();
	};
};

/*
=================
idCommonLocal::LoadSystemDLL
=================
*/
void SbClientApp::InitSystemModule()
{
#ifndef SBE_SINGLE_BINARY
	char			dllPath[ sbe::MAX_OSPATH ];
	
	sbe::sysImport_t	sysImport;
	sbe::sysExport_t	sysExport;
	sbe::GetSysAPI_t	GetSysAPI;
	
	strncpy(dllPath, "./SbSystem", sbe::MAX_OSPATH);
	
	if( !dllPath[ 0 ] )
	{
		mpSys->FatalError( "couldn't find system dynamic library" );
		return;
	}
	mpSys->DPrintf( "Loading system DLL: '%s'\n", dllPath );
	sysDLL = Sys_DLL_Load( dllPath );
	if( !sysDLL )
	{
		mpSys->FatalError( "couldn't load system dynamic library" );
		return;
	}
	
	const char* functionName = "GetSysAPI";
	GetSysAPI = ( sbe::GetSysAPI_t ) Sys_DLL_GetProcAddress( sysDLL, functionName );
	if( !GetSysAPI )
	{
		Sys_DLL_Unload( sysDLL );
		sysDLL = 0;
		mpSys->FatalError( "couldn't find system DLL API" );
		return;
	}
	
	sysImport.version					= sbe::SYS_API_VERSION;
	//sysImport.common					= ::common;
	sysImport.cmdSystem				= mpCmdSystem.get();
	sysImport.cvarSystem				= mpCVarSystem.get();
	
	sysExport = *GetSysAPI( &sysImport );
	
	if( sysExport.version != sbe::SYS_API_VERSION )
	{
		Sys_DLL_Unload( sysDLL );
		sysDLL = 0;
		mpSys->FatalError( "wrong system DLL API version" );
		return;
	};
	
	mpSys = sysExport.sys;
	mpFileSystem = sysExport.fileSystem;
	
#endif
	
	// initialize the system object
	// get architecture info
	if( mpSys != nullptr )
		mpSys->Init();
};

/*
=================
idCommonLocal::UnloadSystemDLL
=================
*/
void SbClientApp::ShutdownSystemModule()
{
	// shut down the system object
	if( mpSys != nullptr )
	{
		// shut down non-portable system services
		printf( "Sys_Shutdown();\n" );
		mpSys->Shutdown();
		mpSys = nullptr;
	};
	
#ifndef SBE_SINGLE_BINARY
	if( sysDLL )
	{
		Sys_DLL_Unload( sysDLL );
		sysDLL = 0;
	};
#endif
};

/*
=================
idCommonLocal::LoadRenderDLL
=================
*/
void SbClientApp::InitRenderSystem()
{
#ifndef SBE_SINGLE_BINARY
	char			dllPath[ sbe::MAX_OSPATH ];
	
	sbe::renderImport_t	renderImport;
	sbe::renderExport_t	renderExport;
	sbe::GetRenderAPI_t	GetRenderAPI;
	
	mpFileSystem->FindDLL( "SbGLRenderer", dllPath, true );
	
	if( !dllPath[ 0 ] )
	{
		mpSys->FatalError( "couldn't find render dynamic library" );
		return;
	};
	mpSys->DPrintf( "Loading render DLL: '%s'\n", dllPath );
	renderDLL = mpSys->DLL_Load( dllPath );
	if( !renderDLL )
	{
		mpSys->FatalError( "couldn't load render dynamic library" );
		return;
	};
	
	const char* functionName = "GetRenderAPI";
	GetRenderAPI = ( sbe::GetRenderAPI_t ) mpSys->DLL_GetProcAddress( renderDLL, functionName );
	if( !GetRenderAPI )
	{
		mpSys->DLL_Unload( renderDLL );
		renderDLL = 0;
		mpSys->FatalError( "couldn't find render DLL API" );
		return;
	};
	
	renderImport.version					= sbe::RENDER_API_VERSION;
	renderImport.sys						= mpSys;
	renderImport.common					= ::common;
	renderImport.cmdSystem				= mpCmdSystem.get();
	renderImport.cvarSystem				= mpCVarSystem.get();
	renderImport.fileSystem				= mpFileSystem;
	renderImport.soundSystem				= ::soundSystem;
	renderImport.uiManager				= ::uiManager;
	renderImport.declManager				= ::declManager;
	
	renderExport							= *GetRenderAPI( &renderImport );
	
	if( renderExport.version != sbe::RENDER_API_VERSION )
	{
		mpSys->DLL_Unload( renderDLL );
		renderDLL = 0;
		mpSys->FatalError( "wrong render DLL API version" );
		return;
	};
	
	renderSystem								= renderExport.renderSystem;
	renderModelManager		= renderExport.renderModelManager;
#else
	renderSystem = CreateRenderSystem();
#endif
	
	// initialize the render object
	if( renderSystem != nullptr )
		renderSystem->Init();
};

/*
=================
SbClientApp::UnloadRenderDLL
=================
*/
void SbClientApp::ShutdownRenderSystem()
{
	// shut down the render object
	if( renderSystem != nullptr )
	{
		// shut down the renderSystem
		printf( "renderSystem->Shutdown();\n" );
		renderSystem->Shutdown();
		renderSystem = nullptr;
	};
	
#ifndef SBE_SINGLE_BINARY
	if( renderDLL )
	{
		mpSys->DLL_Unload( renderDLL );
		renderDLL = 0;
	};
#endif
};

/*
=================
SbClientApp::LoadInputDLL
=================
*/
void SbClientApp::InitInputSystem()
{
#ifndef SBE_SINGLE_BINARY
	char			dllPath[ sbe::MAX_OSPATH ];
	
	sbe::inputImport_t	inputImport;
	sbe::inputExport_t	inputExport;
	sbe::GetInputAPI_t	GetInputAPI;
	
	mpFileSystem->FindDLL( "SbInput", dllPath, true );
	
	if( !dllPath[ 0 ] )
	{
		mpSys->FatalError( "couldn't find input dynamic library" );
		return;
	};
	mpSys->DPrintf( "Loading input DLL: '%s'\n", dllPath );
	inputDLL = mpSys->DLL_Load( dllPath );
	if( !inputDLL )
	{
		mpSys->FatalError( "couldn't load input dynamic library" );
		return;
	};
	
	const char* functionName = "GetInputAPI";
	GetInputAPI = ( sbe::GetInputAPI_t ) mpSys->DLL_GetProcAddress( inputDLL, functionName );
	if( !GetInputAPI )
	{
		mpSys->DLL_Unload( inputDLL );
		inputDLL = 0;
		mpSys->FatalError( "couldn't find input DLL API" );
		return;
	};
	
	inputImport.version					= sbe::INPUT_API_VERSION;
	inputImport.sys						= mpSys;
	//inputImport.common					= ::common;
	inputImport.cmdSystem				= mpCmdSystem.get();
	inputImport.cvarSystem				= mpCVarSystem.get();
	//inputImport.fileSystem				= mpFileSystem;
	//inputImport.declManager				= ::declManager;
	
	inputExport							= *GetInputAPI( &inputImport );
	
	if( inputExport.version != sbe::INPUT_API_VERSION )
	{
		mpSys->DLL_Unload( inputDLL );
		inputDLL = 0;
		mpSys->FatalError( "wrong input DLL API version" );
		return;
	};
	
	inputSystem								= inputExport.inputSystem;
#else
	inputSystem = CreateInputSystem();
#endif
	
	// initialize the input object
	if( inputSystem != nullptr )
		inputSystem->Init();
};

/*
=================
idCommonLocal::UnloadInputDLL
=================
*/
void SbClientApp::ShutdownInputSystem()
{
	// shut down the input object
	if( inputSystem != nullptr )
	{
		inputSystem->Shutdown();
		inputSystem = nullptr;
	};
	
#ifndef SBE_SINGLE_BINARY
	if( inputDLL )
	{
		mpSys->DLL_Unload( inputDLL );
		inputDLL = 0;
	};
#endif
};

void SbClientApp::InputUpdate()
{
	inputSystem->Frame();
};

void SbClientApp::Render()
{
	renderSystem->Frame();
};