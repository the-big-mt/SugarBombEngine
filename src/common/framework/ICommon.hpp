/*
*******************************************************************************

Copyright (C) 1993-2012 id Software LLC, a ZeniMax Media company.
Copyright (C) 2014-2016 Robert Beckebans
Copyright (C) 2014-2016 Kot in Action Creative Artel
Copyright (C) 2018-2020 SugarBombEngine Developers

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
/// @brief common interface

//*****************************************************************************

#pragma once

#include "idlib/sys/sys_types.h"
#include "idlib/Str.h"
/*
==============================================================

//*****************************************************************************
  Common

namespace sbe
{
==============================================================
*/

class idGame;
class idRenderWorld;
class idSoundWorld;
class idSession;
class idCommonDialog;
class idDemoFile;
class idUserInterface;
class idSaveLoadParms;
class idMatchParameters;

struct mpMap_t
{

	void operator=( const mpMap_t& src )
	{
		mapFile = src.mapFile;
		mapName = src.mapName;
		supportedModes = src.supportedModes;
	}
	
	idStr			mapFile;
	idStr			mapName;
	uint32			supportedModes;
};

struct idCommon
{
	virtual						~idCommon() {}
	
	// Initialize everything.
	// if the OS allows, pass argc/argv directly (without executable name)
	// otherwise pass the command line in a single string (without executable name)
	virtual void				Init( int argc, const char* const* argv, const char* cmdline ) = 0;
	
	// Shuts down everything.
	virtual void				Shutdown() = 0;
	virtual bool				IsShuttingDown() const = 0;
	
	virtual	void				CreateMainMenu() = 0;
	
	// Shuts down everything.
	virtual void				Quit() = 0;
	
	// Returns true if common initialization is complete.
	virtual bool				IsInitialized() const = 0;
	
	// Called repeatedly as the foreground thread for rendering and game logic.
	virtual void				Frame() = 0;
	
	// DG: added possibility to *not* release mouse in UpdateScreen(), it fucks up the view angle for screenshots
	// Redraws the screen, handling games, guis, console, etc
	// in a modal manner outside the normal frame loop
	virtual void				UpdateScreen( bool captureToImage, bool releaseMouse = true ) = 0;
	// DG end
	
	virtual void				UpdateLevelLoadPacifier() = 0;
	//virtual void				UpdateLevelLoadPacifier( int mProgress ) = 0;
	//virtual void				UpdateLevelLoadPacifier( bool updateSecondary ) = 0;
	//virtual void				UpdateLevelLoadPacifier( bool updateSecondary, int Progress ) = 0;
	
	// Checks for and removes command line "+set var arg" constructs.
	// If match is nullptr, all set commands will be executed, otherwise
	// only a set with the exact name.
	virtual void				StartupVariable( const char* match ) = 0;
	
	// Begins redirection of console output to the given buffer.
	virtual void				BeginRedirect( char* buffer, int buffersize, void ( *flush )( const char* ) ) = 0;
	
	// Stops redirection of console output.
	virtual void				EndRedirect() = 0;
	
	// Update the screen with every message printed.
	virtual void				SetRefreshOnPrint( bool set ) = 0;
	
	// Prints message to the console, which may cause a screen update if com_refreshOnPrint is set.
	virtual void				Printf( VERIFY_FORMAT_STRING const char* fmt, ... ) = 0;
	
	// Same as Printf, with a more usable API - Printf pipes to this.
	virtual void				VPrintf( const char* fmt, va_list arg ) = 0;
	
	// Prints message that only shows up if the "developer" cvar is set,
	// and NEVER forces a screen update, which could cause reentrancy problems.
	virtual void				DPrintf( VERIFY_FORMAT_STRING const char* fmt, ... ) = 0;
	
	// Prints WARNING %s message and adds the warning message to a queue for printing later on.
	virtual void				Warning( VERIFY_FORMAT_STRING const char* fmt, ... ) = 0;
	
	// Prints WARNING %s message in yellow that only shows up if the "developer" cvar is set.
	virtual void				DWarning( VERIFY_FORMAT_STRING const char* fmt, ... ) = 0;
	
	// Prints all queued warnings.
	virtual void				PrintWarnings() = 0;
	
	// Removes all queued warnings.
	virtual void				ClearWarnings( const char* reason ) = 0;
	
	// Issues a C++ throw. Normal errors just abort to the game loop,
	// which is appropriate for media or dynamic logic errors.
	virtual void				Error( VERIFY_FORMAT_STRING const char* fmt, ... ) ID_INSTANCE_ATTRIBUTE_PRINTF( 1, 2 ) = 0;
	
	// Fatal errors quit all the way to a system dialog box, which is appropriate for
	// static internal errors or cases where the system may be corrupted.
	virtual void                FatalError( VERIFY_FORMAT_STRING const char* fmt, ... ) ID_INSTANCE_ATTRIBUTE_PRINTF( 1, 2 ) = 0;
	
	// Returns key bound to the command
	virtual const char* 		KeysFromBinding( const char* bind ) = 0;
	
	// Returns the binding bound to the key
	virtual const char* 		BindingFromKey( const char* key ) = 0;
	
	// Directly sample a button.
	virtual int					ButtonState( int key ) = 0;
	
	// Directly sample a keystate.
	virtual int					KeyState( int key ) = 0;
	
	// Returns true if a multiplayer game is running.
	// CVars and commands are checked differently in multiplayer mode.
	virtual bool				IsMultiplayer() = 0;
	virtual bool				IsServer() = 0;
	virtual bool				IsClient() = 0;
	
	// Returns true if the player has ever enabled the console
	virtual bool				GetConsoleUsed() = 0;
	
	// Returns the rate (in ms between snaps) that we want to generate snapshots
	virtual int					GetSnapRate() = 0;
	
	virtual void				NetReceiveReliable( int peer, int type, idBitMsg& msg ) = 0;
	virtual void				NetReceiveSnapshot( class idSnapShot& ss ) = 0;
	virtual void				NetReceiveUsercmds( int peer, idBitMsg& msg ) = 0;
	
	// Processes the given event.
	virtual	bool				ProcessEvent( const sysEvent_t* event ) = 0;
	
	virtual bool				LoadGame( const char* saveName ) = 0;
	virtual bool				SaveGame( const char* saveName ) = 0;
	
	virtual idDemoFile* 		ReadDemo() = 0;
	virtual idDemoFile* 		WriteDemo() = 0;
	
	virtual idGame* 			Game() = 0;
	virtual idRenderWorld* 		RW() = 0;
	virtual idSoundWorld* 		SW() = 0;
	virtual idSoundWorld* 		MenuSW() = 0;
	virtual idSession* 			Session() = 0;
	virtual idCommonDialog& 	Dialog() = 0;
	
	virtual void				OnSaveCompleted( idSaveLoadParms& parms ) = 0;
	virtual void				OnLoadCompleted( idSaveLoadParms& parms ) = 0;
	virtual void				OnLoadFilesCompleted( idSaveLoadParms& parms ) = 0;
	virtual void				OnEnumerationCompleted( idSaveLoadParms& parms ) = 0;
	virtual void				OnDeleteCompleted( idSaveLoadParms& parms ) = 0;
	virtual void				TriggerScreenWipe( const char* _wipeMaterial, bool hold ) = 0;
	
	virtual void				OnStartHosting( idMatchParameters& parms ) = 0;
	
	virtual int					GetGameFrame() = 0;
	
	virtual void				InitializeMPMapsModes() = 0;
	virtual const idStrList& 			GetModeList() const = 0;
	virtual const idStrList& 			GetModeDisplayList() const = 0;
	virtual const idList<mpMap_t>& 		GetMapList() const = 0;
	
	virtual void				ResetPlayerInput( int playerIndex ) = 0;
	
	virtual bool				JapaneseCensorship() const = 0;
	
	virtual void				QueueShowShell() = 0;		// Will activate the shell on the next frame.
	
	// RB begin
#if defined(USE_DOOMCLASSIC)
	virtual currentGame_t		GetCurrentGame() const = 0;
	virtual void				SwitchToGame( currentGame_t newGame ) = 0;
#endif
	// RB end
};

{
};
}; // namespace sbe