/*
*******************************************************************************

Copyright (C) 1993-2012 id Software LLC, a ZeniMax Media company.
Copyright (C) 2012-2014 Robert Beckebans
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

#include "AppFrameworks/SbClientApp/SbClientApp.hpp"
#include "EventLoop.h"
#include "framework/CmdSystem.h"
#include "framework/CVarSystem.h"

#include "CoreLibs/SbSystem/ISystem.hpp"
#include "CoreLibs/SbSystem/IFileSystem.hpp"

#include "CoreLibs/SbRenderer/IRenderSystem.hpp"

#include "AppFrameworks/UtilityLibs/SbInput/IInputSystem.hpp"

SbClientApp::~SbClientApp()
{
	ShutdownInputSystem();
	ShutdownRenderSystem();
	
	// shut down the event loop
	printf( "eventLoop->Shutdown();\n" );
	eventLoop->Shutdown();
};

bool SbClientApp::Init()
{
	// init journalling, etc
	eventLoop->Init();
	
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
	// pump all the events
	Sys_GenerateEvents();
	
	eventLoop->RunEventLoop();
	
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

bool SbClientApp::PreRender()
{
	return true;
};

void SbClientApp::Render()
{
	renderSystem->Frame();
};