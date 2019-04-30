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

SbClientApp::~SbClientApp()
{
	ShutdownInputSystem();
	ShutdownRenderSystem();
	//ShutdownSoundSystem();
};

bool SbClientApp::Init()
{
	CreateMainWindow();
	
	if(!InitRenderSystem())
		return false;
	
	if(!InitInputSystem())
		return false;
	
	//if(!InitSoundSystem())
		//return false;

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
idCommonLocal::LoadRenderDLL
=================
*/
void SbClientApp::InitRenderSystem()
{
#ifndef SBE_SINGLE_BINARY
	char			dllPath[ MAX_OSPATH ];
	
	renderImport_t	renderImport;
	renderExport_t	renderExport;
	GetRenderAPI_t	GetRenderAPI;
	
	fileSystem->FindDLL( "SbGLRenderer", dllPath, true );
	
	if( !dllPath[ 0 ] )
	{
		common->FatalError( "couldn't find render dynamic library" );
		return;
	};
	common->DPrintf( "Loading render DLL: '%s'\n", dllPath );
	renderDLL = sys->DLL_Load( dllPath );
	if( !renderDLL )
	{
		common->FatalError( "couldn't load render dynamic library" );
		return;
	};
	
	const char* functionName = "GetRenderAPI";
	GetRenderAPI = ( GetRenderAPI_t ) Sys_DLL_GetProcAddress( renderDLL, functionName );
	if( !GetRenderAPI )
	{
		Sys_DLL_Unload( renderDLL );
		renderDLL = nullptr;
		common->FatalError( "couldn't find render DLL API" );
		return;
	};
	
	gameImport.version					= RENDER_API_VERSION;
	gameImport.sys						= ::sys;
	gameImport.common					= ::common;
	gameImport.cmdSystem				= ::cmdSystem;
	gameImport.cvarSystem				= ::cvarSystem;
	gameImport.fileSystem				= ::fileSystem;
	renderImport.soundSystem				= ::soundSystem;
	renderImport.renderModelManager		= ::renderModelManager;
	renderImport.uiManager				= ::uiManager;
	renderImport.declManager				= ::declManager;
	
	renderExport							= *GetRenderAPI( &renderImport );
	
	if( renderExport.version != RENDER_API_VERSION )
	{
		Sys_DLL_Unload( renderDLL );
		renderDLL = nullptr;
		common->FatalError( "wrong render DLL API version" );
		return;
	};
	
	renderSystem								= renderExport.renderSystem;
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
		renderSystem->Shutdown();
		renderSystem = nullptr;
	};
	
#ifndef SBE_SINGLE_BINARY
	if( renderDLL )
	{
		Sys_DLL_Unload( renderDLL );
		renderDLL = nullptr;
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
	char			dllPath[ MAX_OSPATH ];
	
	inputImport_t	inputImport;
	inputExport_t	inputExport;
	GetInputAPI_t	GetInputAPI;
	
	fileSystem->FindDLL( "SbInput", dllPath, true );
	
	if( !dllPath[ 0 ] )
	{
		common->FatalError( "couldn't find input dynamic library" );
		return;
	};
	common->DPrintf( "Loading input DLL: '%s'\n", dllPath );
	inputDLL = sys->DLL_Load( dllPath );
	if( !inputDLL )
	{
		common->FatalError( "couldn't load input dynamic library" );
		return;
	};
	
	const char* functionName = "GetInputAPI";
	GetInputAPI = ( GetInputAPI_t ) Sys_DLL_GetProcAddress( inputDLL, functionName );
	if( !GetRenderAPI )
	{
		Sys_DLL_Unload( inputDLL );
		inputDLL = nullptr;
		common->FatalError( "couldn't find input DLL API" );
		return;
	};
	
	inputImport.version					= INPUT_API_VERSION;
	inputImport.sys						= ::sys;
	inputImport.common					= ::common;
	inputImport.cmdSystem				= ::cmdSystem;
	inputImport.cvarSystem				= ::cvarSystem;
	inputImport.fileSystem				= ::fileSystem;
	inputImport.declManager				= ::declManager;
	
	inputExport							= *GetRenderAPI( &inputImport );
	
	if( inputExport.version != INPUT_API_VERSION )
	{
		Sys_DLL_Unload( inputDLL );
		inputDLL = nullptr;
		common->FatalError( "wrong input DLL API version" );
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
		inputSystem->Shutdown();
	
#ifndef SBE_SINGLE_BINARY
	if( inputDLL )
	{
		Sys_DLL_Unload( inputDLL );
		inputDLL = nullptr;
	};
	inputSystem = nullptr;
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