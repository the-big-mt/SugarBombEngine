/*
===========================================================================

Doom 3 BFG Edition GPL Source Code
Copyright (C) 1993-2012 id Software LLC, a ZeniMax Media company.
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

#pragma once

/*
================================================
idSession
================================================
*/
struct ISession
{
	enum sessionState_t
	{
		PRESS_START,
		IDLE,
		SEARCHING,
		CONNECTING,
		PARTY_LOBBY,
		GAME_LOBBY,
		LOADING,
		INGAME,
		BUSY,
		
		MAX_STATES
	};
	
	enum sessionOption_t
	{
		OPTION_LEAVE_WITH_PARTY			= BIT( 0 ),		// As a party leader, whether or not to drag your party members with you when you leave a game lobby
		OPTION_ALL						= 0xFFFFFFFF
	};
	
	//=====================================================================================================
	// Lobby management
	//=====================================================================================================
	
	virtual void FindOrCreateMatch( const idMatchParameters& parms_ ) = 0;
	virtual void CreateMatch( const idMatchParameters& parms_ ) = 0;
	
	virtual void UpdateMatchParms( const idMatchParameters& parms_ ) = 0;
	virtual void UpdatePartyParms( const idMatchParameters& parms_ ) = 0;
	
	virtual void Cancel() = 0;
	
	virtual void ShowPartySessions() = 0;
	
	virtual bool IsPlatformPartyInLobby() = 0;
	
	// Lobby user/peer access
	// The party and game lobby are the two platform lobbies that notify the backends (Steam/PSN/LIVE of changes)
	virtual idLobbyBase &GetPartyLobbyBase() = 0;
	virtual idLobbyBase &GetGameLobbyBase() = 0;
	
	// GetActivePlatformLobbyBase will return either the game or party lobby, it won't return the game state lobby
	// This function is generally used for menus, in-game code should refer to GetActingGameStateLobby
	virtual idLobbyBase &GetActivePlatformLobbyBase() = 0;
	
	virtual IPlayerProfile *GetProfileFromMasterLocalUser() = 0;
	
	virtual sessionState_t GetState() const = 0;
	
	//=====================================================================================================
	// Title Storage Vars
	//=====================================================================================================
	
	virtual int GetTitleStorageInt( const char* name, int defaultInt ) const = 0;
	
	//=====================================================================================================
	//	GamerCard UI
	//=====================================================================================================
	
	virtual void ShowLobbyUserGamerCardUI( lobbyUserID_t lobbyUserID ) = 0;
	
	//=====================================================================================================
	
	virtual ISignInManagerBase &GetSignInManager() const = 0;
	
	//=====================================================================================================
	//	Voice chat
	//=====================================================================================================
	
	virtual void ToggleLobbyUserVoiceMute( lobbyUserID_t lobbyUserID ) = 0;
};