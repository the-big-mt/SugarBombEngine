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

#include "GameFramework.hpp"

#ifndef SBE_SINGLE_BINARY
idGame* 		game = nullptr;
idGameEdit* 	gameEdit = nullptr;
#endif

/*
===========
GetFrameworkAPI
============
*/
#ifndef SBE_SINGLE_BINARY
static sbe::frameworkExport_t frameworkExport;

sbe::ISys *sys{nullptr};
idCmdSystem *cmdSystem{nullptr};
idCVarSystem *cvarSystem{nullptr};
sbe::IFileSystem *fileSystem{nullptr};
idDeclManager *declManager{nullptr};

idCVar *idCVar::staticVars{nullptr};

C_EXPORT sbe::frameworkExport_t *GetFrameworkAPI(sbe::frameworkImport_t *import)
{
	if(import->version == sbe::FRAMEWORK_API_VERSION)
	{
		// set interface pointers used by the module
		sys							= import->sys;
		cmdSystem					= import->cmdSystem;
		cvarSystem					= import->cvarSystem;
		fileSystem					= import->fileSystem;
		declManager					= import->declManager;
	};
	
	// set interface pointers used by idLib
	idLib::sys					= sys;
	idLib::common				= common;
	idLib::cvarSystem			= cvarSystem;
	idLib::fileSystem			= fileSystem;
	
	// setup export interface
	frameworkExport.version = sbe::FRAMEWORK_API_VERSION;
	frameworkExport.common = common; // TODO: CreateFramework();
	
	return &frameworkExport;
};
#endif // SBE_SINGLE_BINARY

/*
=================
idCommonLocal::Init
=================
*/
void SbGameFramework::Init( int argc, const char* const* argv, const char* cmdline )
{
	try
	{
		// set interface pointers used by idLib
		idLib::sys			= sys;
		idLib::common		= common;
		idLib::cvarSystem	= cvarSystem;
		idLib::fileSystem	= fileSystem;
		
		// initialize idLib
		idLib::Init();
		
		// clear warning buffer
		ClearWarnings( GAME_NAME " initialization" );
		
		idLib::Printf( "Command line: %s\n", cmdline );
		//::MessageBox( nullptr, cmdline, "blah", MB_OK );
		
		if( idStr::Icmp( argv[ i ], "+connect_lobby" ) == 0 )
		{
			// Handle Steam bootable invites.
			
			// RB begin
#if defined(_WIN32)
			session->HandleBootableInvite( _atoi64( argv[ i + 1 ] ) );
#else
			session->HandleBootableInvite( atol( argv[ i + 1 ] ) );
#endif
			// RB end
		}
		
		// register all static CVars
		idCVar::RegisterStaticVars();
		
		idLib::Printf( "QA Timing INIT: %06dms\n", Sys_Milliseconds() );
		
		// print engine version
		Printf( "%s\n", version.string );
		
		// initialize key input/binding, done early so bind command exists
		idKeyInput::Init();
		
		// init the console so we can take prints
		console->Init();
		
		// initialize networking
		Sys_InitNetworking();
		
		consoleUsed = com_allowConsole.GetBool();
		
		const char* defaultLang = Sys_DefaultLanguage();
		com_isJapaneseSKU = ( idStr::Icmp( defaultLang, ID_LANG_JAPANESE ) == 0 );
		
		// Allow the system to set a default lanugage
		Sys_SetLanguageFromSystem();
		
		// Pre-allocate our 20 MB save buffer here on time, instead of on-demand for each save....
		
		saveFile.SetNameAndType( SAVEGAME_CHECKPOINT_FILENAME, SAVEGAMEFILE_BINARY );
		saveFile.PreAllocate( MIN_SAVEGAME_SIZE_BYTES );
		
		stringsFile.SetNameAndType( SAVEGAME_STRINGS_FILENAME, SAVEGAMEFILE_BINARY );
		stringsFile.PreAllocate( MAX_SAVEGAME_STRING_TABLE_SIZE );
		
		mpFileSystem->BeginLevelLoad( "_startup", saveFile.GetDataPtr(), saveFile.GetAllocated() );
		
		// initialize the declaration manager
		declManager->Init();
		
		// init the parallel job manager
		parallelJobManager->Init();
		
		// exec the startup scripts
		mpCmdSystem->BufferCommandText( CMD_EXEC_APPEND, "exec default.cfg\n" );
		
#ifdef CONFIG_FILE
		// skip the config file if "safe" is on the command line
		if( !SafeMode() && !g_demoMode.GetBool() )
			mpCmdSystem->BufferCommandText( CMD_EXEC_APPEND, "exec " CONFIG_FILE "\n" );
#endif
		
		mpCmdSystem->BufferCommandText( CMD_EXEC_APPEND, "exec autoexec.cfg\n" );
		
		// run cfg execution
		mpCmdSystem->ExecuteCommandBuffer();
		
		// re-override anything from the config files with command line args
		StartupVariable( nullptr );
		
		// if any archived cvars are modified after this, we will trigger a writing of the config file
		mpCvarSystem->ClearModifiedFlags( CVAR_ARCHIVE );
		
		// Support up to 2 digits after the decimal point
		com_engineHz_denominator = 100LL * com_engineHz.GetFloat();
		com_engineHz_latched = com_engineHz.GetFloat();
		
		whiteMaterial = declManager->FindMaterial( "_white" );
		
		if( idStr::Icmp( sys_lang.GetString(), ID_LANG_FRENCH ) == 0 )
		{
			// If the user specified french, we show french no matter what SKU
			splashScreen = declManager->FindMaterial( "guis/assets/splash/legal_french" );
		}
		else if( idStr::Icmp( defaultLang, ID_LANG_FRENCH ) == 0 )
		{
			// If the lead sku is french (ie: europe), display figs
			splashScreen = declManager->FindMaterial( "guis/assets/splash/legal_figs" );
		}
		else
		{
			// Otherwise show it in english
			splashScreen = declManager->FindMaterial( "guis/assets/splash/legal_english" );
		};
		
		const int legalMinTime = 4000;
		const bool showVideo = ( !com_skipIntroVideos.GetBool() && fileSystem->UsingResourceFiles() );
		if( showVideo )
		{
			RenderBink( "video\\loadvideo.bik" );
			RenderSplash();
			RenderSplash();
		}
		else
		{
			idLib::Printf( "Skipping Intro Videos!\n" );
			// display the legal splash screen
			// No clue why we have to render this twice to show up...
			RenderSplash();
			RenderSplash();
		};
		
		int legalStartTime = Sys_Milliseconds();
		declManager->Init2();
		
		// initialize string database so we can use it for loading messages
		InitLanguageDict();
		
		// spawn the game thread, even if we are going to run without SMP
		// one meg stack, because it can parse decls from gui surfaces (unfortunately)
		// use a lower priority so job threads can run on the same core
		gameThread.StartWorkerThread( "Game/Draw", CORE_1B, THREAD_BELOW_NORMAL, 0x100000 );
		// boost this thread's priority, so it will prevent job threads from running while
		// the render back end still has work to do
		
		// init the user command input code
		usercmdGen->Init();
		
		Sys_SetRumble( 0, 0, 0 );
		
		// initialize the user interfaces
		uiManager->Init();
		
		// startup the script debugger
		// DebuggerServerInit();
		
		// Init tool commands
		InitCommands();
		
		// init the game module
		InitGameModule();
		
		// On the PC touch them all so they get included in the resource build
		if( !mpFileSystem->UsingResourceFiles() )
		{
			declManager->FindMaterial( "guis/assets/splash/legal_english" );
			declManager->FindMaterial( "guis/assets/splash/legal_french" );
			declManager->FindMaterial( "guis/assets/splash/legal_figs" );
			// register the japanese font so it gets included
			renderSystem->RegisterFont( "DFPHeiseiGothicW7" );
			// Make sure all videos get touched because you can bring videos from one map to another, they need to be included in all maps
			for( int i = 0; i < declManager->GetNumDecls( DECL_VIDEO ); i++ )
				declManager->DeclByIndex( DECL_VIDEO, i );
		};
		
		mpFileSystem->UnloadResourceContainer( "_ordered" );
		
		// the same idRenderWorld will be used for all games
		// and demos, insuring that level specific models
		// will be freed
		renderWorld = renderSystem->AllocRenderWorld();
		soundWorld = soundSystem->AllocSoundWorld( renderWorld );
		
		menuSoundWorld = soundSystem->AllocSoundWorld( nullptr );
		menuSoundWorld->PlaceListener( vec3_origin, mat3_identity, 0 );
		
		// init the session
		session->Initialize();
		session->InitializeSoundRelatedSystems();
		
		InitializeMPMapsModes();
		
		// leaderboards need to be initialized after InitializeMPMapsModes, which populates the MP Map list.
		if( game != nullptr )
			game->Leaderboards_Init();
		
		CreateMainMenu();
		
		commonDialog.Init();
		
		// load the console history file
		consoleHistory.LoadHistoryFile();
		
		AddStartupCommands(); // TODO: PostInit
		
		StartMenu( true );
		
		while( Sys_Milliseconds() - legalStartTime < legalMinTime )
		{
			RenderSplash();
			Sys_GenerateEvents();
			Sys_Sleep( 10 );
		};
		
		// print all warnings queued during initialization
		PrintWarnings();
		
		// remove any prints from the notify lines
		console->ClearNotifyLines();
		
		CheckStartupStorageRequirements();
		
		if( preload_CommonAssets.GetBool() && mpFileSystem->UsingResourceFiles() )
		{
			idPreloadManifest manifest;
			manifest.LoadManifest( "_common.preload" );
			globalImages->Preload( manifest, false );
			mpSoundSystem->Preload( manifest );
		};
		
		mpFileSystem->EndLevelLoad();
		
		com_fullyInitialized = true;
		
		// No longer need the splash screen
		if( splashScreen != nullptr )
		{
			for( int i = 0; i < splashScreen->GetNumStages(); i++ )
			{
				idImage* image = splashScreen->GetStage( i )->texture.image;
				if( image != nullptr )
					image->PurgeImage();
			};
		};
		
		Printf( "--- Common Initialization Complete ---\n" );
		
		idLib::Printf( "QA Timing IIS: %06dms\n", Sys_Milliseconds() );
	}
	catch( idException& )
		mpSys->Error( "Error during initialization" );
};

/*
=================
idCommonLocal::Shutdown
=================
*/
void SbGameFramework::Shutdown()
{
	if( com_shuttingDown )
		return;

	com_shuttingDown = true;
	
	// Kill any pending saves...
	printf( "session->GetSaveGameManager().CancelToTerminate();\n" );
	session->GetSaveGameManager().CancelToTerminate();
	
	// shutdown the script debugger
	// DebuggerServerShutdown();
	
	if( aviCaptureMode )
	{
		printf( "EndAVICapture();\n" );
		EndAVICapture();
	};
	
	printf( "Stop();\n" );
	Stop();
	
	printf( "CleanupShell();\n" );
	CleanupShell();
	
	printf( "delete loadGUI;\n" );
	delete loadGUI;
	loadGUI = nullptr;
	
#ifdef USE_CEGUI
	printf( "idCEGUI::Destroy();\n" );
	idCEGUI::Destroy();
#endif

	printf( "delete renderWorld;\n" );
	delete renderWorld;
	renderWorld = nullptr;
	
	printf( "delete soundWorld;\n" );
	delete soundWorld;
	soundWorld = nullptr;
	
	printf( "delete menuSoundWorld;\n" );
	delete menuSoundWorld;
	menuSoundWorld = nullptr;
	
	// shut down the session
	printf( "session->ShutdownSoundRelatedSystems();\n" );
	session->ShutdownSoundRelatedSystems();
	printf( "session->Shutdown();\n" );
	session->Shutdown();
	
	// shutdown, deallocate leaderboard definitions.
	if( game != nullptr )
	{
		printf( "game->Leaderboards_Shutdown();\n" );
		game->Leaderboards_Shutdown();
	};
	
	// shut down the user interfaces
	printf( "uiManager->Shutdown();\n" );
	uiManager->Shutdown();
	
	// shut down the user command input code
	printf( "usercmdGen->Shutdown();\n" );
	usercmdGen->Shutdown();
	
	// shutdown the decl manager
	printf( "declManager->Shutdown();\n" );
	declManager->Shutdown();
	
	printf( "commonDialog.Shutdown();\n" );
	commonDialog.Shutdown();
	
	// shutdown the game module
	printf( "ShutdownGameModule();\n" );
	ShutdownGameModule();
	
	printf( "saveFile.Clear( true );\n" );
	saveFile.Clear( true );
	printf( "stringsFile.Clear( true );\n" );
	stringsFile.Clear( true );
	
	// shut down the console
	printf( "console->Shutdown();\n" );
	console->Shutdown();
	
	// shut down the key system
	printf( "idKeyInput::Shutdown();\n" );
	idKeyInput::Shutdown();
	
	// free any buffered warning messages
	printf( "ClearWarnings( GAME_NAME \" shutdown\" );\n" );
	ClearWarnings( GAME_NAME " shutdown" );
	printf( "warningCaption.Clear();\n" );
	warningCaption.Clear();
	printf( "errorList.Clear();\n" );
	errorList.Clear();
	
	// shutdown idLib
	printf( "idLib::ShutDown();\n" );
	idLib::ShutDown();
};

/*
=================
idCommonLocal::IsInitialized
=================
*/
bool SbGameFramework::IsInitialized() const
{
	return com_fullyInitialized;
};

/*
========================
idCommonLocal::CreateMainMenu
========================
*/
void SbGameFramework::CreateMainMenu()
{
	if( game != nullptr )
	{
		// note which media we are going to need to load
		declManager->BeginLevelLoad();
		renderSystem->BeginLevelLoad();
		mpSoundSystem->BeginLevelLoad();
		uiManager->BeginLevelLoad();
		
		// create main inside an "empty" game level load - so assets get
		// purged automagically when we transition to a "real" map
		game->Shell_CreateMenu( false );
		game->Shell_Show( true );
		game->Shell_SyncWithSession();
		
		// load
		renderSystem->EndLevelLoad();
		mpSoundSystem->EndLevelLoad();
		declManager->EndLevelLoad();
		uiManager->EndLevelLoad( "" );
	};
};

/*
=================
idCommonLocal::LoadGameDLL
=================
*/
void SbGameFramework::InitGameModule()
{
#ifndef SBE_SINGLE_BINARY
	char			dllPath[ MAX_OSPATH ];
	
	gameImport_t	gameImport;
	gameExport_t	gameExport;
	GetGameAPI_t	GetGameAPI;
	
	mpFileSystem->FindDLL( "game", dllPath/*, true*/ );
	
	if( !dllPath[ 0 ] )
	{
		mpSys->FatalError( "couldn't find game dynamic library" );
		return;
	};
	mpSys->DPrintf( "Loading game DLL: '%s'\n", dllPath );
	gameDLL = mpSys->DLL_Load( dllPath );
	if( !gameDLL )
	{
		mpSys->FatalError( "couldn't load game dynamic library" );
		return;
	};
	
	const char* functionName = "GetGameAPI";
	GetGameAPI = ( GetGameAPI_t ) mpSys->DLL_GetProcAddress( gameDLL, functionName );
	if( !GetGameAPI )
	{
		mpSys->DLL_Unload( gameDLL );
		gameDLL = nullptr;
		mpSys->FatalError( "couldn't find game DLL API" );
		return;
	};
	
	gameImport.version					= sbe::GAME_API_VERSION;
	gameImport.sys						= mpSys;
	gameImport.common					= ::common;
	gameImport.cmdSystem				= mpCmdSystem;
	gameImport.cvarSystem				= ::cvarSystem;
	gameImport.fileSystem				= mpFileSystem;
	gameImport.renderSystem				= ::renderSystem;
	gameImport.soundSystem				= mpSoundSystem;
	gameImport.renderModelManager		= ::renderModelManager;
	gameImport.uiManager				= ::uiManager;
	gameImport.declManager				= ::declManager;
	gameImport.AASFileManager			= ::AASFileManager;
	gameImport.collisionModelManager	= ::collisionModelManager;
	
	gameExport							= *GetGameAPI( &gameImport );
	
	if( gameExport.version != GAME_API_VERSION )
	{
		mpSys->DLL_Unload( gameDLL );
		gameDLL = nullptr;
		mpSys->FatalError( "wrong game DLL API version" );
		return;
	};
	
	game								= gameExport.game;
	gameEdit							= gameExport.gameEdit;
	
#endif
	
	// initialize the game object
	if( game != nullptr )
		game->Init();
};

/*
=================
idCommonLocal::UnloadGameDLL
=================
*/
void SbGameFramework::ShutdownGameModule()
{
	// shut down the game object
	if( game != nullptr )
	{
		game->Shutdown();
		game = nullptr;
	};
	
#ifndef SBE_SINGLE_BINARY
	if( gameDLL )
	{
		mpSys->DLL_Unload( gameDLL );
		gameDLL = nullptr;
	};
	
	gameEdit = nullptr;
#endif
};

/*
===============
idCommonLocal::BusyWait
===============
*/
void SbGameFramework::BusyWait()
{
	Sys_GenerateEvents();
	
	const bool captureToImage = false;
	UpdateScreen( captureToImage );
	
	session->UpdateSignInManager();
	session->Pump();
};

/*
==================
idCommonLocal::WriteConfigToFile
==================
*/
void SbGameFramework::WriteConfigToFile( const char* filename )
{
	idFile* f = mpFileSystem->OpenFileWrite( filename );
	if( !f )
	{
		Printf( "Couldn't write %s.\n", filename );
		return;
	};
	
	idKeyInput::WriteBindings( f );
	mpCvarSystem->WriteFlaggedVariables( CVAR_ARCHIVE, "set", f );
	mpFileSystem->CloseFile( f );
};

/*
===============
idCommonLocal::WriteConfiguration

Writes key bindings and archived cvars to config file if modified
===============
*/
void SbGameFramework::WriteConfiguration()
{
	// if we are quiting without fully initializing, make sure
	// we don't write out anything
	if( !com_fullyInitialized )
		return;
	
	if( !( mpCvarSystem->GetModifiedFlags() & CVAR_ARCHIVE ) )
		return;
	mpCvarSystem->ClearModifiedFlags( CVAR_ARCHIVE );
	
	// save to the profile
	idLocalUser* user = session->GetSignInManager().GetMasterLocalUser();
	if( user != nullptr )
		user->SaveProfileSettings();
	
#ifdef CONFIG_FILE
	// disable printing out the "Writing to:" message
	bool developer = com_developer.GetBool();
	com_developer.SetBool( false );
	
	WriteConfigToFile( CONFIG_FILE );
	
	// restore the developer cvar
	com_developer.SetBool( developer );
#endif
};