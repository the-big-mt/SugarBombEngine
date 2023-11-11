/*
*******************************************************************************

Copyright (C) 1993-2012 id Software LLC, a ZeniMax Media company.
Copyright (C) 2019-2020, 2022-2023 SugarBombEngine Developers

This file is part of SugarBombEngine

SugarBombEngine is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

SugarBombEngine is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with SugarBombEngine. If not, see <http://www.gnu.org/licenses/>.

In addition, SugarBombEngine is using id Tech 4 (BFG) pieces and thus subject to certain additional terms (all header and source files which 
contains such pieces has this additional part appended to the license header). You should have received a copy of these additional terms 
stated in a separate file (LICENSE-idTech4) which accompanied the SugarBombEngine source code. If not, please request a copy in 
writing from id Software at the address below.

If you have questions concerning this license or the applicable additional terms, you may contact in writing id Software LLC, c/o ZeniMax Media Inc.,
Suite 120, Rockville, Maryland 20850 USA.

*******************************************************************************
*/

/// @file

//*****************************************************************************

#pragma once

//*****************************************************************************

namespace sbe
{

class SbDict;
class idUserCmdMgr;

struct SbFile;
struct SbRenderWorld;
struct SbSoundWorld;

constexpr auto TIME_GROUP1{0};
constexpr auto TIME_GROUP2{1};

struct gameReturn_t;

struct SbGame
{
	/// Initialize the game for the first time
	virtual void Init() = 0;
	
	/// Shut down the entire game
	virtual void Shutdown() = 0;
	
	/// Runs a game frame, may return a session command for level changing, etc
	virtual void RunFrame(idUserCmdMgr &aCmdMgr, gameReturn_t &aGameReturn) = 0;
	
	/// Runs prediction on entities at the client
	virtual void ClientRunFrame(idUserCmdMgr &aCmdMgr, bool abLastPredictFrame, gameReturn_t &aGameReturn) = 0;
	
	/// Makes rendering and sound system calls to display for a given clientNum
	virtual bool Draw(int anClientNum) = 0;
	
	/// Sets the serverinfo at map loads and when it changes
	//virtual void SetServerInfo(const SbDict &aInfo) = 0;
	
	/// Gets the serverinfo, common calls this before saving the game
	//virtual const SbDict &GetServerInfo() const = 0;
	
	/// Interpolated server time
	//virtual void SetServerGameTimeMs(const int anTime) = 0;
	
	/// Interpolated server time
	//virtual int GetServerGameTimeMs() const = 0;
	
	//virtual int GetSSEndTime() const = 0;
	//virtual int GetSSStartTime() const = 0;
	
	/// game framework calls this before moving the single player game to a new level
	//virtual const SbDict &GetPersistentPlayerInfo(int anClientNum) const = 0;
	
	/// game framework calls this right before a new level is loaded
	//virtual void SetPersistentPlayerInfo(int anClientNum, const SbDict &aPlayerInfo) = 0;
	
	/// Loads a map and spawns all the entities
	// TODO: why this is here and not part of the framework?
	//virtual void InitFromNewMap(const char *asMapName, SbRenderWorld *apRenderWorld, SbSoundWorld *apSoundWorld, int anGameMode, int anRandSeed) = 0;
	
	/// Loads a map from a savegame file
	// TODO: why this is here and not part of the framework?
	//virtual bool InitFromSaveGame(const char *asMapName, SbRenderWorld *apRenderWorld, SbSoundWorld *apSoundWorld, SbFile *apSaveGameFile, SbFile *apStringTableFile, int anSaveGameVersion) = 0;
	
	/// Saves the current game state, game framework may have written some data to the file already
	//virtual void SaveGame(SbFile *apSaveGameFile, SbFile *apStringTableFile) = 0;
	
	/// Pulls the current player location from the game information
	//virtual void GetSaveGameDetails(idSaveGameDetails &aGameDetails) const = 0;
	
	/// Shut down the current map
	virtual void MapShutdown() = 0;
	
	/*
	/// Caches media referenced from in key/value pairs in the given dictionary
	virtual void CacheDictionaryMedia(const SbDict *dict) = 0;
	
	virtual void Preload(const idPreloadManifest &manifest) = 0;
	
	virtual bool HandlePlayerGuiEvent(const sysEvent_t *ev) = 0;
	
	/// Writes a snapshot of the server game state
	virtual void ServerWriteSnapshot(idSnapShot &ss) = 0;
	
	/// Processes a reliable message
	virtual void ProcessReliableMessage(int clientNum, int type, const idBitMsg &msg) = 0;
	
	virtual void SetInterpolation(const float fraction, const int serverGameMS, const int ssStartTime, const int ssEndTime) = 0;
	
	/// Reads a snapshot and updates the client game state
	virtual void ClientReadSnapshot(const idSnapShot &ss) = 0;
	
	/// Used to manage divergent time-lines
	virtual int GetTimeGroupTime(int anTimeGroup) const = 0;
	
	/// Returns a list of available multiplayer game modes
	virtual int GetMPGameModes(const char** * gameModes, const char** * gameModesDisplay) const = 0;
	
	/// Returns a summary of stats for a given client
	virtual void GetClientStats(int clientNum, char* data, const int len) const = 0;
	
	///
	virtual bool IsInGame() const = 0;
	
	/// Get the player entity number for a network peer
	virtual int MapPeerToClient(int peer) const = 0;
	
	/// Get the player entity number of the local player
	virtual int GetLocalClientNum() const = 0;
	
	/// compute an angle offset to be applied to the given client's aim
	virtual void GetAimAssistAngles(SbAngles &angles) const = 0;
	virtual float GetAimAssistSensitivity() const = 0;
	*/
	
	/// Release the mouse when the PDA is open
	virtual bool ShouldShowCursor() const = 0;
	//virtual bool IsPDAOpen() const = 0;
	//virtual bool IsPlayerChatting() const = 0;
	
	/// Creates leaderboards for each map/mode defined
	//virtual void Leaderboards_Init() = 0;
	//virtual void Leaderboards_Shutdown() = 0;
	
	/// MAIN MENU FUNCTIONS
	
	///
	//virtual bool InhibitControls() = 0;
	
	//virtual bool SkipCinematicScene() = 0;
	//virtual bool CheckInCinematic() = 0;
	
	/// Demo helper functions
	//virtual void StartDemoPlayback(SbRenderWorld *apRenderWorld) = 0;
	
	//virtual bool ProcessDemoCommand(idDemoFile *apReadDemo) = 0;
};

struct SbGameShell
{
	///
	//virtual void Init(const char *asFileName, SbSoundWorld *apSoundWorld) = 0;
	
	///
	virtual void Cleanup() = 0;
	
	///
	virtual void CreateMenu(bool abInGame) = 0;
	
	///
	//virtual void ClosePause() = 0;
	
	///
	virtual void Show(bool abShow) = 0;
	
	///
	virtual bool IsActive() const = 0;
	
	///
	//virtual bool HandleGuiEvent(const sysEvent_t *apEvent) = 0;
	
	///
	virtual void Render() = 0;
	
	///
	//virtual void ResetMenu() = 0;
	
	///
	virtual void SyncWithSession() = 0;
	
	/*
	virtual void UpdateSavedGames() = 0;
	virtual void SetCanContinue(bool abValid) = 0;
	virtual void UpdateClientCountdown(int anCountDown) = 0;
	virtual void UpdateLeaderboard(const idLeaderboardCallback *apCallback) = 0;
	virtual void SetGameComplete() = 0;
	*/
};

}; // namespace sbe