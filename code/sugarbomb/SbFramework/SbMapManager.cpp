/*
*******************************************************************************

Copyright (C) 1993-2012 id Software LLC, a ZeniMax Media company.
Copyright (C) 2014-2016 Robert Beckebans
Copyright (C) 2014-2016 Kot in Action Creative Artel
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

const char *SbMapManager::GetCurrentMapName() const
{
	return currentMapName.c_str();
};

/*
===============
idCommonLocal::MoveToNewMap
Single player transition from one map to another
===============
*/
void SbMapManager::MoveToNewMap( const char* mapName, bool devmap )
{
	idMatchParameters matchParameters;
	matchParameters.numSlots = 1;
	matchParameters.gameMode = GAME_MODE_SINGLEPLAYER;
	matchParameters.gameMap = GAME_MAP_SINGLEPLAYER;
	matchParameters.mapName = mapName;
	cvarSystem->MoveCVarsToDict( CVAR_SERVERINFO, matchParameters.serverInfo );
	if( devmap )
	{
		matchParameters.serverInfo.Set( "devmap", "1" );
		mapSpawnData.persistentPlayerInfo.Clear();
	}
	else
	{
		matchParameters.serverInfo.Delete( "devmap" );
		mapSpawnData.persistentPlayerInfo = game->GetPersistentPlayerInfo( 0 );
	}
	session->QuitMatchToTitle();
	if( WaitForSessionState( idSession::IDLE ) )
	{
		session->CreatePartyLobby( matchParameters );
		if( WaitForSessionState( idSession::PARTY_LOBBY ) )
		{
			session->CreateMatch( matchParameters );
			if( WaitForSessionState( idSession::GAME_LOBBY ) )
			{
				session->StartMatch();
			}
		}
	}
};

/*
===============
idCommonLocal::ExecuteMapChange

Performs the initialization of a game based on session match parameters, used for both single
player and multiplayer, but not for renderDemos, which don't create a game at all.
Exits with mapSpawned = true
===============
*/
void SbMapManager::ExecuteMapChange()
{
	if( session->GetState() != idSession::LOADING )
	{
		idLib::Warning( "Session state is not LOADING in ExecuteMapChange" );
		return;
	}
	
	// Clear all dialogs before beginning the load
	common->Dialog().ClearDialogs( true );
	
	// Remember the current load ID.
	// This is so we can tell if we had a new loadmap request from within an existing loadmap call
	const int cachedLoadingID = session->GetLoadingID();
	
	const idMatchParameters& matchParameters = session->GetActingGameStateLobbyBase().GetMatchParms();
	
	if( matchParameters.numSlots <= 0 )
	{
		idLib::Warning( "numSlots <= 0 in ExecuteMapChange" );
		return;
	}
	
	insideExecuteMapChange = true;
	
	mpSys->Printf( "--------- Execute Map Change ---------\n" );
	mpSys->Printf( "Map: %s\n", matchParameters.mapName.c_str() );
	
	// ensure that r_znear is reset to the default value
	// this fixes issues with the projection matrix getting messed up when switching maps or loading a saved game
	// while an in-game cinematic is playing.
	cvarSystem->SetCVarFloat( "r_znear", 3.0f );
	
	// reset all cheat cvars for a multiplayer game
	if( IsMultiplayer() )
	{
		cvarSystem->ResetFlaggedVariables( CVAR_CHEAT );
	}
	
	int start = Sys_Milliseconds();
	
	for( int i = 0; i < MAX_INPUT_DEVICES; i++ )
	{
		Sys_SetRumble( i, 0, 0 );
	}
	
	// close console and remove any prints from the notify lines
	console->Close();
	
	// clear all menu sounds
	soundWorld->Pause();
	menuSoundWorld->ClearAllSoundEmitters();
	soundSystem->SetPlayingSoundWorld( menuSoundWorld );
	soundSystem->Render();
	
	// extract the map name from serverinfo
	currentMapName = matchParameters.mapName;
	currentMapName.StripFileExtension();
	
	idStrStatic< MAX_OSPATH > fullMapName = "maps/";
	fullMapName += currentMapName;
	fullMapName.SetFileExtension( "map" );
	
	if( mapSpawnData.savegameFile )
	{
		mpFileSystem->BeginLevelLoad( currentMapName, nullptr, 0 );
	}
	else
	{
		mpFileSystem->BeginLevelLoad( currentMapName, saveFile.GetDataPtr(), saveFile.GetAllocated() );
	}
	
	// capture the current screen and start a wipe
	// immediately complete the wipe to fade out the level transition
	// run the wipe to completion
	StartWipe( "wipeMaterial", true );
	CompleteWipe();
	
	int sm = Sys_Milliseconds();
	// shut down the existing game if it is running
	UnloadMap();
	int ms = Sys_Milliseconds() - sm;
	mpSys->Printf( "%6d msec to unload map\n", ms );
	
	// Free media from previous level and
	// note which media we are going to need to load
	sm = Sys_Milliseconds();
	renderSystem->BeginLevelLoad();
	soundSystem->BeginLevelLoad();
	declManager->BeginLevelLoad();
	uiManager->BeginLevelLoad();
	ms = Sys_Milliseconds() - sm;
	mpSys->Printf( "%6d msec to free assets\n", ms );
	
	//Sys_DumpMemory( true );
	
	// load / program a gui to stay up on the screen while loading
	// set the loading gui that we will wipe to
	bool hellMap = false;
	LoadLoadingGui( currentMapName, hellMap );
	
	// Stop rendering the wipe
	ClearWipe();
	
	if( fileSystem->UsingResourceFiles() )
	{
		idStrStatic< MAX_OSPATH > manifestName = currentMapName;
		manifestName.Replace( "game/", "maps/" );
		manifestName.Replace( "/mp/", "/" );
		manifestName += ".preload";
		idPreloadManifest manifest;
		manifest.LoadManifest( manifestName );
		renderSystem->Preload( manifest, currentMapName );
		soundSystem->Preload( manifest );
		mpGame->Preload( manifest );
	}
	
	if( common->IsMultiplayer() )
	{
		// In multiplayer, make sure the player is either 60Hz or 120Hz
		// to avoid potential issues.
		const float mpEngineHz = ( com_engineHz.GetFloat() < 90.0f ) ? 60.0f : 120.0f;
		com_engineHz_denominator = 100LL * mpEngineHz;
		com_engineHz_latched = mpEngineHz;
	}
	else
	{
		// allow com_engineHz to be changed between map loads
		com_engineHz_denominator = 100LL * com_engineHz.GetFloat();
		com_engineHz_latched = com_engineHz.GetFloat();
	}
	
	// note any warning prints that happen during the load process
	common->ClearWarnings( currentMapName );
	
	// release the mouse cursor
	// before we do this potentially long operation
	Sys_GrabMouseCursor( false );
	
	// let the renderSystem load all the geometry
	if( !renderWorld->InitFromMap( fullMapName ) )
	{
		mpSys->Error( "couldn't load %s", fullMapName.c_str() );
	}
	
	// for the synchronous networking we needed to roll the angles over from
	// level to level, but now we can just clear everything
	usercmdGen->InitForNewMap();
	
	// load and spawn all other entities ( from a savegame possibly )
	if( mapSpawnData.savegameFile )
	{
		if( !mpGame->InitFromSaveGame( fullMapName, renderWorld, soundWorld, mapSpawnData.savegameFile, mapSpawnData.stringTableFile, mapSpawnData.savegameVersion ) )
		{
			// If the loadgame failed, end the session, which will force us to go back to the main menu
			session->QuitMatchToTitle();
		}
	}
	else
	{
		if( !IsMultiplayer() )
		{
			assert( mpGame->GetLocalClientNum() == 0 );
			assert( matchParameters.gameMode == GAME_MODE_SINGLEPLAYER );
			assert( matchParameters.gameMap == GAME_MAP_SINGLEPLAYER );
			mpGame->SetPersistentPlayerInfo( 0, mapSpawnData.persistentPlayerInfo );
		}
		mpGame->SetServerInfo( matchParameters.serverInfo );
		mpGame->InitFromNewMap( fullMapName, renderWorld, soundWorld, matchParameters.gameMode, Sys_Milliseconds() );
	}
	
	mpGame->Shell_CreateMenu( true );
	
	// Reset some values important to multiplayer
	ResetNetworkingState();
	
	// If the session state is not loading here, something went wrong.
	if( session->GetState() == idSession::LOADING && session->GetLoadingID() == cachedLoadingID )
	{
		// Notify session we are done loading
		session->LoadingFinished();
		
		while( session->GetState() == idSession::LOADING )
		{
			Sys_GenerateEvents();
			session->UpdateSignInManager();
			session->Pump();
			Sys_Sleep( 10 );
		}
	}
	
	if( !mapSpawnData.savegameFile )
	{
		// run a single frame to catch any resources that are referenced by events posted in spawn
		idUserCmdMgr emptyCommandManager;
		gameReturn_t emptyGameReturn;
		for( int playerIndex = 0; playerIndex < MAX_PLAYERS; ++playerIndex )
		{
			emptyCommandManager.PutUserCmdForPlayer( playerIndex, usercmd_t() );
		}
		if( IsClient() )
		{
			mpGame->ClientRunFrame( emptyCommandManager, false, emptyGameReturn );
		}
		else
		{
			mpGame->RunFrame( emptyCommandManager, emptyGameReturn );
		}
	}
	
	renderSystem->EndLevelLoad();
	soundSystem->EndLevelLoad();
	declManager->EndLevelLoad();
	uiManager->EndLevelLoad( currentMapName );
	fileSystem->EndLevelLoad();
	
	if( !mapSpawnData.savegameFile && !IsMultiplayer() )
	{
		mpSys->Printf( "----- Running initial game frames -----\n" );
		
		// In single player, run a bunch of frames to make sure ragdolls are settled
		idUserCmdMgr emptyCommandManager;
		gameReturn_t emptyGameReturn;
		for( int i = 0; i < 100; i++ )
		{
			for( int playerIndex = 0; playerIndex < MAX_PLAYERS; ++playerIndex )
			{
				emptyCommandManager.PutUserCmdForPlayer( playerIndex, usercmd_t() );
			}
			mpGame->RunFrame( emptyCommandManager, emptyGameReturn );
		}
		
		// kick off an auto-save of the game (so we can always continue in this map if we die before hitting an autosave)
		mpSys->Printf( "----- Saving Game -----\n" );
		SaveGame( "autosave" );
	}
	
	mpSys->Printf( "----- Generating Interactions -----\n" );
	
	// let the renderSystem generate interactions now that everything is spawned
	renderWorld->GenerateAllInteractions();
	
	{
		int vertexMemUsedKB = vertexCache.staticData.vertexMemUsed.GetValue() / 1024;
		int indexMemUsedKB = vertexCache.staticData.indexMemUsed.GetValue() / 1024;
		idLib::Printf( "Used %dkb of static vertex memory (%d%%)\n", vertexMemUsedKB, vertexMemUsedKB * 100 / ( STATIC_VERTEX_MEMORY / 1024 ) );
		idLib::Printf( "Used %dkb of static index memory (%d%%)\n", indexMemUsedKB, indexMemUsedKB * 100 / ( STATIC_INDEX_MEMORY / 1024 ) );
	}
	
	if( common->JapaneseCensorship() )
	{
		if( currentMapName.Icmp( "game/mp/d3xpdm3" ) == 0 )
		{
			const idMaterial* gizpool2 = declManager->FindMaterial( "textures/hell/gizpool2" );
			idMaterial* chiglass1bluex = const_cast<idMaterial*>( declManager->FindMaterial( "textures/sfx/chiglass1bluex" ) );
			idTempArray<char> text( gizpool2->GetTextLength() );
			gizpool2->GetText( text.Ptr() );
			chiglass1bluex->Parse( text.Ptr(), text.Num(), false );
		}
	}
	
	common->PrintWarnings();
	
	session->Pump();
	
	if( session->GetState() != idSession::INGAME )
	{
		// Something went wrong, don't process stale reliables that have been queued up.
		reliableQueue.Clear();
	}
	
	usercmdGen->Clear();
	
	// remove any prints from the notify lines
	console->ClearNotifyLines();
	
	Sys_SetPhysicalWorkMemory( -1, -1 );
	
	// at this point we should be done with the loading gui so we kill it
	delete loadGUI;
	loadGUI = nullptr;
	
	// capture the current screen and start a wipe
	StartWipe( "wipe2Material" );
	
	// we are valid for game draws now
	insideExecuteMapChange = false;
	mapSpawned = true;
	Sys_ClearEvents();
	
	int	msec = Sys_Milliseconds() - start;
	common->Printf( "%6d msec to load %s\n", msec, currentMapName.c_str() );
	//Sys_DumpMemory( false );
	
	// Issue a render at the very end of the load process to update soundTime before the first frame
	soundSystem->Render();
};

/*
==================
Common_Map_f

Restart the server on a different map
==================
*/
CONSOLE_COMMAND_SHIP( map, "loads a map", idCmdSystem::ArgCompletion_MapName )
{
	commonLocal.StartNewGame( args.Argv( 1 ), false, GAME_MODE_SINGLEPLAYER );
};

/*
==================
Common_RestartMap_f
==================
*/
CONSOLE_COMMAND_SHIP( restartMap, "restarts the current map", nullptr )
{
	if( g_demoMode.GetBool() )
	{
		cmdSystem->AppendCommandText( va( "devmap %s %d\n", commonLocal.GetCurrentMapName(), 0 ) );
	}
};

/*
==================
Common_DevMap_f

Restart the server on a different map in developer mode
==================
*/
CONSOLE_COMMAND_SHIP( devmap, "loads a map in developer mode", idCmdSystem::ArgCompletion_MapName )
{
	commonLocal.StartNewGame( args.Argv( 1 ), true, GAME_MODE_SINGLEPLAYER );
};

/*
==================
Common_NetMap_f

Restart the server on a different map in multiplayer mode
==================
*/
CONSOLE_COMMAND_SHIP( netmap, "loads a map in multiplayer mode", idCmdSystem::ArgCompletion_MapName )
{
	int gameMode = 0; // Default to deathmatch
	if( args.Argc() > 2 )
	{
		gameMode = atoi( args.Argv( 2 ) );
	}
	commonLocal.StartNewGame( args.Argv( 1 ), true, gameMode );
};

/*
==================
Common_TestMap_f
==================
*/
CONSOLE_COMMAND( testmap, "tests a map", idCmdSystem::ArgCompletion_MapName )
{
	idStr map, string;
	
	map = args.Argv( 1 );
	if( !map.Length() )
	{
		return;
	}
	map.StripFileExtension();
	
	cmdSystem->BufferCommandText( CMD_EXEC_NOW, "disconnect" );
	
	sprintf( string, "dmap maps/%s.map", map.c_str() );
	cmdSystem->BufferCommandText( CMD_EXEC_NOW, string );
	
	sprintf( string, "devmap %s", map.c_str() );
	cmdSystem->BufferCommandText( CMD_EXEC_NOW, string );
};