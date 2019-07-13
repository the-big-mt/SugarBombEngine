/*
*******************************************************************************

Copyright (C) 1993-2012 id Software LLC, a ZeniMax Media company.
Copyright (C) 2014-2016 Robert Beckebans
Copyright (C) 2014-2016 Kot in Action Creative Artel
Copyright (C) 2018-2019 BlackPhrase

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

#pragma once

/*
===============================================================================

	Public game interface with methods to run the game.

===============================================================================
*/

namespace sbe
{

class idDict;

struct idFile;
struct idRenderWorld;
struct idSoundWorld;

struct gameReturn_t
{
	gameReturn_t() = default;
	
	char		sessionCommand[MAX_STRING_CHARS]{};	// "map", "disconnect", "victory", etc
	bool		syncNextGameFrame{false};			// used when cinematics are skipped to prevent session from simulating several game frames to
	// keep the game time in sync with real time
	int			vibrationLow{0};
	int			vibrationHigh{0};
};

struct IGame
{
	// Initialize the game for the first time.
	virtual void				Init() = 0;
	
	// Shut down the entire game.
	virtual void				Shutdown() = 0;
	
	// Sets the serverinfo at map loads and when it changes.
	virtual void				SetServerInfo( const idDict& serverInfo ) = 0;
	
	// Gets the serverinfo, common calls this before saving the game
	virtual const idDict& 		GetServerInfo() = 0;
	
	// Interpolated server time
	virtual void				SetServerGameTimeMs( const int time ) = 0;
	
	// Interpolated server time
	virtual int					GetServerGameTimeMs() const = 0;
	
	virtual int					GetSSEndTime() const  = 0;
	virtual int					GetSSStartTime() const = 0;
	
	// common calls this before moving the single player game to a new level.
	virtual const idDict& 		GetPersistentPlayerInfo( int clientNum ) = 0;
	
	// common calls this right before a new level is loaded.
	virtual void				SetPersistentPlayerInfo( int clientNum, const idDict& playerInfo ) = 0;
	
	// Loads a map and spawns all the entities.
	virtual void				InitFromNewMap( const char* mapName, idRenderWorld* renderWorld, idSoundWorld* soundWorld, int gameMode, int randseed ) = 0;
	
	// Loads a map from a savegame file.
	virtual bool				InitFromSaveGame( const char* mapName, idRenderWorld* renderWorld, idSoundWorld* soundWorld, idFile* saveGameFile, idFile* stringTableFile, int saveGameVersion ) = 0;
	
	// Saves the current game state, common may have written some data to the file already.
	virtual void				SaveGame( idFile* saveGameFile, idFile* stringTableFile ) = 0;
	
	// Pulls the current player location from the game information
	virtual void				GetSaveGameDetails( idSaveGameDetails& gameDetails ) = 0;
	
	// Shut down the current map.
	virtual void				MapShutdown() = 0;
	
	// Caches media referenced from in key/value pairs in the given dictionary.
	virtual void				CacheDictionaryMedia( const idDict* dict ) = 0;
	
	virtual void				Preload( const idPreloadManifest& manifest ) = 0;
	
	// Runs a game frame, may return a session command for level changing, etc
	virtual void				RunFrame( idUserCmdMgr& cmdMgr, gameReturn_t& gameReturn ) = 0;
	
	// Makes rendering and sound system calls to display for a given clientNum.
	virtual bool				Draw( int clientNum ) = 0;
	
	virtual bool				HandlePlayerGuiEvent( const sysEvent_t* ev ) = 0;
	
	// Writes a snapshot of the server game state.
	virtual void				ServerWriteSnapshot( idSnapShot& ss ) = 0;
	
	// Processes a reliable message
	virtual void				ProcessReliableMessage( int clientNum, int type, const idBitMsg& msg ) = 0;
	
	virtual void				SetInterpolation( const float fraction, const int serverGameMS, const int ssStartTime, const int ssEndTime ) = 0;
	
	// Reads a snapshot and updates the client game state.
	virtual void				ClientReadSnapshot( const idSnapShot& ss ) = 0;
	
	// Runs prediction on entities at the client.
	virtual void				ClientRunFrame( idUserCmdMgr& cmdMgr, bool lastPredictFrame, gameReturn_t& ret ) = 0;
	
	// Used to manage divergent time-lines
	virtual int					GetTimeGroupTime( int timeGroup ) = 0;
	
	// Returns a list of available multiplayer game modes
	virtual int					GetMPGameModes( const char** * gameModes, const char** * gameModesDisplay ) = 0;
	
	// Returns a summary of stats for a given client
	virtual void				GetClientStats( int clientNum, char* data, const int len ) = 0;
	
	virtual bool				IsInGame() const = 0;
	
	// Get the player entity number for a network peer.
	virtual int					MapPeerToClient( int peer ) const = 0;
	
	// Get the player entity number of the local player.
	virtual int					GetLocalClientNum() const = 0;
	
	// compute an angle offset to be applied to the given client's aim
	virtual void				GetAimAssistAngles( idAngles& angles ) = 0;
	virtual float				GetAimAssistSensitivity() = 0;
	
	// Release the mouse when the PDA is open
	virtual bool				IsPDAOpen() const = 0;
	virtual bool				IsPlayerChatting() const = 0;
	
	// Creates leaderboards for each map/mode defined.
	virtual void				Leaderboards_Init() = 0;
	virtual void				Leaderboards_Shutdown() = 0;
	
	// MAIN MENU FUNCTIONS
	virtual bool				InhibitControls() = 0;
	virtual void				Shell_Init( const char* filename, idSoundWorld* sw ) = 0;
	virtual void				Shell_Cleanup() = 0;
	virtual void				Shell_CreateMenu( bool inGame ) = 0;
	virtual void				Shell_ClosePause() = 0;
	virtual void				Shell_Show( bool show ) = 0;
	virtual bool				Shell_IsActive() const = 0;
	virtual bool				Shell_HandleGuiEvent( const sysEvent_t* sev ) = 0;
	virtual void				Shell_Render() = 0;
	virtual void				Shell_ResetMenu() = 0;
	virtual void				Shell_SyncWithSession() = 0;
	virtual void				Shell_UpdateSavedGames() = 0;
	virtual void				Shell_SetCanContinue( bool valid ) = 0;
	virtual void				Shell_UpdateClientCountdown( int countdown ) = 0;
	virtual void				Shell_UpdateLeaderboard( const idLeaderboardCallback* callback ) = 0;
	virtual void				Shell_SetGameComplete() = 0;
	virtual bool				SkipCinematicScene() = 0;
	virtual bool				CheckInCinematic() = 0;
	
	// Demo helper functions
	virtual void				StartDemoPlayback( idRenderWorld* renderworld ) = 0;
	
	virtual bool				ProcessDemoCommand( idDemoFile* readDemo ) = 0;
};

struct idGame : public IGame
{
	virtual						~idGame() {}
};

/*
===============================================================================

	Game API.

===============================================================================
*/

const int GAME_API_VERSION		= 8;

struct gameImport_t
{
	int							version;				// API version
	ISys* 						sys;					// non-portable system services
	IGameFramework* 					common;					// common
	idCmdSystem* 				cmdSystem;				// console command system
	idCVarSystem* 				cvarSystem;				// console variable system
	IFileSystem* 				fileSystem;				// file system
	IRenderSystem* 			renderSystem;			// render system
	ISoundSystem* 				soundSystem;			// sound system
	idRenderModelManager* 		renderModelManager;		// render model manager
	idUserInterfaceManager* 	uiManager;				// user interface manager
	idDeclManager* 				declManager;			// declaration manager
	idAASFileManager* 			AASFileManager;			// AAS file manager
	idCollisionModelManager* 	collisionModelManager;	// collision model manager
};

struct gameExport_t
{
	int							version;				// API version
	idGame* 					game;					// interface to run the game
	idGameEdit* 				gameEdit;				// interface for in-game editing
};

extern "C" using GetGameAPI_t = gameExport_t *(*)( gameImport_t* import );

}; // namespace sbe