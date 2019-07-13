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

/// @file

#include "SbSaveGameManager.hpp"

SbSaveGameManager::SbSaveGameManager() = default;
SbSaveGameManager::~SbSaveGameManager() = default;

/*
===============
idCommonLocal::SaveGame
===============
*/
bool SbSaveGameManager::SaveGame( const char* saveName )
{
	if( pipelineFile != nullptr )
	{
		// We're already in the middle of a save. Leave us alone.
		return false;
	}
	
	if( com_disableAllSaves.GetBool() || ( com_disableAutoSaves.GetBool() && ( idStr::Icmp( saveName, "autosave" ) == 0 ) ) )
	{
		return false;
	}
	
	if( IsMultiplayer() )
	{
		mpSys->Printf( "Can't save during net play.\n" );
		return false;
	}
	
	if( mapSpawnData.savegameFile != nullptr )
	{
		return false;
	}
	
	const idDict& persistentPlayerInfo = game->GetPersistentPlayerInfo( 0 );
	if( persistentPlayerInfo.GetInt( "health" ) <= 0 )
	{
		mpSys->Printf( "You must be alive to save the game\n" );
		return false;
	}
	
	soundWorld->Pause();
	soundSystem->SetPlayingSoundWorld( menuSoundWorld );
	soundSystem->Render();
	
	Dialog().ShowSaveIndicator( true );
	if( insideExecuteMapChange )
	{
		UpdateLevelLoadPacifier();
	}
	else
	{
		// Heremake sure we pump the gui enough times to show the 'saving' dialog
		const bool captureToImage = false;
		for( int i = 0; i < NumScreenUpdatesToShowDialog; ++i )
		{
			UpdateScreen( captureToImage );
		}
		renderSystem->BeginAutomaticBackgroundSwaps( AUTORENDER_DIALOGICON );
	}
	
	// Make sure the file is writable and the contents are cleared out (Set to write from the start of file)
	saveFile.MakeWritable();
	saveFile.Clear( false );
	stringsFile.MakeWritable();
	stringsFile.Clear( false );
	
	// Setup the save pipeline
	pipelineFile = new( TAG_SAVEGAMES ) idFile_SaveGamePipelined();
	pipelineFile->OpenForWriting( &saveFile );
	
	// Write SaveGame Header:
	// Game Name / Version / Map Name / Persistant Player Info
	
	// game
	const char* gamename = GAME_NAME;
	saveFile.WriteString( gamename );
	
	// map
	saveFile.WriteString( currentMapName );
	
	saveFile.WriteBool( consoleUsed );
	
	mpGame->GetServerInfo().WriteToFileHandle( &saveFile );
	
	// let the game save its state
	mpGame->SaveGame( pipelineFile, &stringsFile );
	
	pipelineFile->Finish();
	
	idSaveGameDetails gameDetails;
	mpGame->GetSaveGameDetails( gameDetails );
	
	gameDetails.descriptors.Set( SAVEGAME_DETAIL_FIELD_LANGUAGE, sys_lang.GetString() );
	gameDetails.descriptors.SetInt( SAVEGAME_DETAIL_FIELD_CHECKSUM, ( int )gameDetails.descriptors.Checksum() );
	
	gameDetails.slotName = saveName;
	ScrubSaveGameFileName( gameDetails.slotName );
	
	saveFileEntryList_t files;
	files.Append( &stringsFile );
	files.Append( &saveFile );
	
	session->SaveGameSync( gameDetails.slotName, files, gameDetails );
	
	if( !insideExecuteMapChange )
	{
		renderSystem->EndAutomaticBackgroundSwaps();
	}
	
	syncNextGameFrame = true;
	
	return true;
};

/*
===============
idCommonLocal::LoadGame
===============
*/
bool SbSaveGameManager::LoadGame( const char* saveName )
{
	if( IsMultiplayer() )
	{
		mpSys->Printf( "Can't load during net play.\n" );
		if( wipeForced )
		{
			ClearWipe();
		}
		return false;
	}
	
	if( session->GetSignInManager().GetMasterLocalUser() == nullptr )
	{
		return false;
	}
	if( mapSpawnData.savegameFile != nullptr )
	{
		return false;
	}
	
	bool found = false;
	const saveGameDetailsList_t& sgdl = session->GetSaveGameManager().GetEnumeratedSavegames();
	for( int i = 0; i < sgdl.Num(); i++ )
	{
		if( sgdl[i].slotName == saveName )
		{
			if( sgdl[i].GetLanguage() != sys_lang.GetString() )
			{
				idStaticList< idSWFScriptFunction*, 4 > callbacks;
				idStaticList< idStrId, 4 > optionText;
				optionText.Append( idStrId( "#str_swf_continue" ) );
				idStrStatic<256> langName = "#str_lang_" + sgdl[i].GetLanguage();
				idStrStatic<256> msg;
				msg.Format( idLocalization::GetString( "#str_dlg_wrong_language" ), idLocalization::GetString( langName ) );
				Dialog().AddDynamicDialog( GDM_SAVEGAME_WRONG_LANGUAGE, callbacks, optionText, true, msg, false, true );
				if( wipeForced )
				{
					ClearWipe();
				}
				return false;
			}
			found = true;
			break;
		}
	}
	if( !found )
	{
		mpSys->Printf( "Could not find save '%s'\n", saveName );
		if( wipeForced )
		{
			ClearWipe();
		}
		return false;
	}
	
	mapSpawnData.savegameFile = &saveFile;
	mapSpawnData.stringTableFile = &stringsFile;
	
	saveFileEntryList_t files;
	files.Append( mapSpawnData.stringTableFile );
	files.Append( mapSpawnData.savegameFile );
	
	idStr slotName = saveName;
	ScrubSaveGameFileName( slotName );
	saveFile.Clear( false );
	stringsFile.Clear( false );
	
	saveGameHandle_t loadGameHandle = session->LoadGameSync( slotName, files );
	if( loadGameHandle != 0 )
	{
		return true;
	}
	mapSpawnData.savegameFile = nullptr;
	if( wipeForced )
	{
		ClearWipe();
	}
	return false;
};

/*
========================
idCommonLocal::OnSaveCompleted
========================
*/
void SbSaveGameManager::OnSaveCompleted( idSaveLoadParms& parms )
{
	assert( pipelineFile != nullptr );
	delete pipelineFile;
	pipelineFile = nullptr;
	
	if( parms.GetError() == SAVEGAME_E_NONE )
	{
		mpGame->Shell_UpdateSavedGames();
	}
	
	if( !HandleCommonErrors( parms ) )
	{
		common->Dialog().AddDialog( GDM_ERROR_SAVING_SAVEGAME, DIALOG_CONTINUE, nullptr, nullptr, false );
	}
};

/*
========================
idCommonLocal::OnLoadCompleted
========================
*/
void SbSaveGameManager::OnLoadCompleted( idSaveLoadParms& parms )
{
	if( !HandleCommonErrors( parms ) )
	{
		common->Dialog().AddDialog( GDM_ERROR_LOADING_SAVEGAME, DIALOG_CONTINUE, nullptr, nullptr, false );
	}
};

/*
========================
idCommonLocal::OnLoadFilesCompleted
========================
*/
void SbSaveGameManager::OnLoadFilesCompleted( idSaveLoadParms& parms )
{
	if( ( mapSpawnData.savegameFile != nullptr ) && ( parms.GetError() == SAVEGAME_E_NONE ) )
	{
		// just need to make the file readable
		( ( idFile_Memory* )mapSpawnData.savegameFile )->MakeReadOnly();
		( ( idFile_Memory* )mapSpawnData.stringTableFile )->MakeReadOnly();
		
		idStr gamename;
		idStr mapname;
		
		mapSpawnData.savegameVersion = parms.description.GetSaveVersion();
		mapSpawnData.savegameFile->ReadString( gamename );
		mapSpawnData.savegameFile->ReadString( mapname );
		
		if( ( gamename != GAME_NAME ) || ( mapname.IsEmpty() ) || ( parms.description.GetSaveVersion() > BUILD_NUMBER ) )
		{
			// if this isn't a savegame for the correct game, abort loadgame
			mpSys->Warning( "Attempted to load an invalid savegame" );
		}
		else
		{
			mpSys->DPrintf( "loading savegame\n" );
			
			mapSpawnData.savegameFile->ReadBool( consoleUsed );
			consoleUsed = consoleUsed || com_allowConsole.GetBool();
			
			idMatchParameters matchParameters;
			matchParameters.numSlots = 1;
			matchParameters.gameMode = GAME_MODE_SINGLEPLAYER;
			matchParameters.gameMap = GAME_MAP_SINGLEPLAYER;
			matchParameters.mapName = mapname;
			matchParameters.serverInfo.ReadFromFileHandle( mapSpawnData.savegameFile );
			
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
						return;
					}
				}
			}
		}
	}
	// If we got here then we didn't actually load the save game for some reason
	mapSpawnData.savegameFile = nullptr;
};

/*
========================
idCommonLocal::OnEnumerationCompleted
========================
*/
void SbSaveGameManager::OnEnumerationCompleted( idSaveLoadParms& parms )
{
	if( parms.GetError() == SAVEGAME_E_NONE )
	{
		mpGame->Shell_UpdateSavedGames();
	}
};

/*
========================
idCommonLocal::OnDeleteCompleted
========================
*/
void SbSaveGameManager::OnDeleteCompleted( idSaveLoadParms& parms )
{
	if( parms.GetError() == SAVEGAME_E_NONE )
	{
		mpGame->Shell_UpdateSavedGames();
	}
};

/*
========================
HandleCommonErrors
========================
*/
bool SbSaveGameManager::HandleCommonErrors( const idSaveLoadParms& parms )
{
	common->Dialog().ShowSaveIndicator( false );
	
	if( parms.GetError() == SAVEGAME_E_NONE )
	{
		return true;
	}
	
	if( parms.GetError() & SAVEGAME_E_CORRUPTED )
	{
		// This one might need to be handled by the game
		common->Dialog().AddDialog( GDM_CORRUPT_CONTINUE, DIALOG_CONTINUE, nullptr, nullptr, false );
		
		// Find the game in the enumerated details, mark as corrupt so the menus can show as corrupt
		saveGameDetailsList_t& list = session->GetSaveGameManager().GetEnumeratedSavegamesNonConst();
		for( int i = 0; i < list.Num(); i++ )
		{
			if( idStr::Icmp( list[i].slotName, parms.description.slotName ) == 0 )
			{
				list[i].damaged = true;
			}
		}
		return true;
	}
	else if( parms.GetError() & SAVEGAME_E_INSUFFICIENT_ROOM )
	{
		HandleInsufficientStorage( parms );
		return true;
	}
	else if( parms.GetError() & SAVEGAME_E_UNABLE_TO_SELECT_STORAGE_DEVICE && saveGame_enable.GetBool() )
	{
		common->Dialog().AddDialog( GDM_UNABLE_TO_USE_SELECTED_STORAGE_DEVICE, DIALOG_CONTINUE, nullptr, nullptr, false );
		return true;
	}
	else if( parms.GetError() & SAVEGAME_E_INVALID_FILENAME )
	{
		idLib::Warning( "Invalid savegame filename [%s]!", parms.directory.c_str() );
		return true;
	}
	else if( parms.GetError() & SAVEGAME_E_DLC_NOT_FOUND )
	{
		common->Dialog().AddDialog( GDM_DLC_ERROR_MISSING_GENERIC, DIALOG_CONTINUE, nullptr, nullptr, false );
		return true;
	}
	else if( parms.GetError() & SAVEGAME_E_DISC_SWAP )
	{
		common->Dialog().AddDialog( GDM_DISC_SWAP, DIALOG_CONTINUE, nullptr, nullptr, false );
		return true;
	}
	else if( parms.GetError() & SAVEGAME_E_INCOMPATIBLE_NEWER_VERSION )
	{
		common->Dialog().AddDialog( GDM_INCOMPATIBLE_NEWER_SAVE, DIALOG_CONTINUE, nullptr, nullptr, false );
		return true;
	}
	
	return false;
};

/*
========================
HandleInsufficientStorage
========================
*/
void SbSaveGameManager::HandleInsufficientStorage( const idSaveLoadParms& parms )
{
	session->GetSaveGameManager().ShowRetySaveDialog( parms.directory, parms.requiredSpaceInBytes );
};

/*
===============
idCommonLocal::ScrubSaveGameFileName

Turns a bad file name into a good one or your money back
===============
*/
void SbSaveGameManager::ScrubSaveGameFileName( idStr& saveFileName ) const
{
	int i;
	idStr inFileName;
	
	inFileName = saveFileName;
	inFileName.RemoveColors();
	inFileName.StripFileExtension();
	
	saveFileName.Clear();
	
	int len = inFileName.Length();
	for( i = 0; i < len; i++ )
	{
		if( strchr( "',.~!@#$%^&*()[]{}<>\\|/=?+;:-\'\"", inFileName[i] ) )
		{
			// random junk
			saveFileName += '_';
		}
		else if( ( const unsigned char )inFileName[i] >= 128 )
		{
			// high ascii chars
			saveFileName += '_';
		}
		else if( inFileName[i] == ' ' )
		{
			saveFileName += '_';
		}
		else
		{
			saveFileName += inFileName[i];
		}
	}
};

/*
===============
LoadGame_f
===============
*/
CONSOLE_COMMAND_SHIP( loadGame, "loads a game", idCmdSystem::ArgCompletion_SaveGame )
{
	console->Close();
	commonLocal.LoadGame( ( args.Argc() > 1 ) ? args.Argv( 1 ) : "quick" );
};

/*
===============
SaveGame_f
===============
*/
CONSOLE_COMMAND_SHIP( saveGame, "saves a game", nullptr )
{
	const char* savename = ( args.Argc() > 1 ) ? args.Argv( 1 ) : "quick";
	if( commonLocal.SaveGame( savename ) )
	{
		common->Printf( "Saved: %s\n", savename );
	}
};