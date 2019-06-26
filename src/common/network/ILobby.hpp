/*
===========================================================================

Doom 3 BFG Edition GPL Source Code
Copyright (C) 1993-2012 id Software LLC, a ZeniMax Media company.

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

#pragma once

/*
================================================
idLobbyBase
================================================
*/
class idLobbyBase
{
public:
	// General lobby functionality
	virtual bool						IsHost() const = 0;
	virtual bool						IsPeer() const = 0;
	virtual bool						HasActivePeers() const = 0;
	virtual int							GetNumLobbyUsers() const = 0;
	virtual int							GetNumActiveLobbyUsers() const = 0;
	virtual bool						IsLobbyUserConnected( int index ) const = 0;
	
	virtual lobbyUserID_t				GetLobbyUserIdByOrdinal( int userIndex ) const = 0;
	virtual	int							GetLobbyUserIndexFromLobbyUserID( lobbyUserID_t lobbyUserID ) const = 0;
	
	virtual void						SendReliable( int type, idBitMsg& msg, bool callReceiveReliable = true, peerMask_t sessionUserMask = MAX_UNSIGNED_TYPE( peerMask_t ) ) = 0;
	virtual void						SendReliableToLobbyUser( lobbyUserID_t lobbyUserID, int type, idBitMsg& msg ) = 0;
	virtual void						SendReliableToHost( int type, idBitMsg& msg ) = 0;
	
	// Lobby user access
	virtual const char* 				GetLobbyUserName( lobbyUserID_t lobbyUserID ) const = 0;
	virtual void						KickLobbyUser( lobbyUserID_t lobbyUserID ) = 0;
	virtual bool						IsLobbyUserValid( lobbyUserID_t lobbyUserID ) const = 0;
	virtual bool						IsLobbyUserLoaded( lobbyUserID_t lobbyUserID ) const = 0;
	virtual bool						LobbyUserHasFirstFullSnap( lobbyUserID_t lobbyUserID ) const = 0;
	virtual void						EnableSnapshotsForLobbyUser( lobbyUserID_t lobbyUserID ) = 0;
	
	virtual int							GetLobbyUserSkinIndex( lobbyUserID_t lobbyUserID ) const = 0;
	virtual bool						GetLobbyUserWeaponAutoReload( lobbyUserID_t lobbyUserID ) const = 0;
	virtual bool						GetLobbyUserWeaponAutoSwitch( lobbyUserID_t lobbyUserID ) const = 0;
	virtual int							GetLobbyUserLevel( lobbyUserID_t lobbyUserID ) const = 0;
	virtual int							GetLobbyUserQoS( lobbyUserID_t lobbyUserID ) const = 0;
	virtual int							GetLobbyUserTeam( lobbyUserID_t lobbyUserID ) const = 0;
	virtual bool						SetLobbyUserTeam( lobbyUserID_t lobbyUserID, int teamNumber ) = 0;
	virtual int							GetLobbyUserPartyToken( lobbyUserID_t lobbyUserID ) const = 0;
	virtual idPlayerProfile* 			GetProfileFromLobbyUser( lobbyUserID_t lobbyUserID ) = 0;
	virtual idLocalUser* 				GetLocalUserFromLobbyUser( lobbyUserID_t lobbyUserID ) = 0;
	virtual int							GetNumLobbyUsersOnTeam( int teamNumber ) const = 0;
	
	virtual int							PeerIndexFromLobbyUser( lobbyUserID_t lobbyUserID ) const = 0;
	virtual int							GetPeerTimeSinceLastPacket( int peerIndex ) const = 0;
	virtual int							PeerIndexForHost() const = 0;
	
	virtual lobbyUserID_t				AllocLobbyUserSlotForBot( const char* botName ) = 0;
	virtual void						RemoveBotFromLobbyUserList( lobbyUserID_t lobbyUserID ) = 0;
	virtual bool						GetLobbyUserIsBot( lobbyUserID_t lobbyUserID ) const = 0;
	
	virtual const char* 				GetHostUserName() const = 0;
	virtual const idMatchParameters& 	GetMatchParms() const = 0;
	virtual bool						IsLobbyFull() const = 0;
	
	// Peer access
	virtual bool						EnsureAllPeersHaveBaseState() = 0;
	virtual bool						AllPeersInGame() const = 0;
	virtual int							GetNumConnectedPeers() const = 0;
	virtual int							GetNumConnectedPeersInGame() const = 0;
	virtual int							PeerIndexOnHost() const = 0;
	virtual bool						IsPeerDisconnected( int peerIndex ) const = 0;
	
	// Snapshots
	virtual bool						AllPeersHaveStaleSnapObj( int objId ) = 0;
	virtual bool						AllPeersHaveExpectedSnapObj( int objId ) = 0;
	virtual void						RefreshSnapObj( int objId ) = 0;
	virtual void						MarkSnapObjDeleted( int objId ) = 0;
	virtual void						AddSnapObjTemplate( int objID, idBitMsg& msg ) = 0;
	
	// Debugging
	virtual void						DrawDebugNetworkHUD() const = 0;
	virtual void						DrawDebugNetworkHUD2() const = 0;
	virtual void						DrawDebugNetworkHUD_ServerSnapshotMetrics( bool draw ) = 0;

	virtual ~idLobbyBase() {}
};