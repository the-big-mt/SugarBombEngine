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

#include "idlib/sys/sys_types.h"
#include "idlib/BitMsg.h"

//namespace sbe
//{

struct IPlayerProfile;
class idSerializer;

enum onlineCaps_t
{
	CAP_IS_ONLINE			= BIT( 0 ),
	CAP_BLOCKED_PERMISSION	= BIT( 1 ),
	CAP_CAN_PLAY_ONLINE		= BIT( 2 ),
};

/*
================================================
localUserHandle_t
================================================
*/
struct localUserHandle_t
{
	using userHandleType_t = uint32;
	
	localUserHandle_t() = default;
	
	explicit localUserHandle_t( userHandleType_t handle_ ) : handle( handle_ ) {}
	
	bool operator == ( const localUserHandle_t& other ) const
	{
		return handle == other.handle;
	}
	
	bool operator < ( const localUserHandle_t& other ) const
	{
		return handle < other.handle;
	}
	
	bool IsValid() const
	{
		return handle > 0;
	}
	
	void WriteToMsg( idBitMsg& msg )
	{
		msg.WriteLong( handle );
	}
	
	void ReadFromMsg( const idBitMsg& msg )
	{
		handle = msg.ReadLong();
	}
	
	void Serialize( idSerializer& ser );
	
private:
	userHandleType_t	handle{0};
};

/*
================================================
idLocalUser
An idLocalUser is a user holding a controller.
It represents someone controlling the menu or game.
They may not necessarily be in a game (which would be a session user of TYPE_GAME).
A controller user references an input device (which is a gamepad, keyboard, etc).
================================================
*/
struct ILocalUser
{
	virtual IPlayerProfile *GetProfile() const = 0;
};

//}; // namespace sbe