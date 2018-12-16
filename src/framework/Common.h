/*
===========================================================================

Doom 3 BFG Edition GPL Source Code
Copyright (C) 1993-2012 id Software LLC, a ZeniMax Media company.
Copyright (C) 2014-2016 Robert Beckebans
Copyright (C) 2014-2016 Kot in Action Creative Artel

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

#ifndef __COMMON_H__
#define __COMMON_H__

extern idCVar com_engineHz;
extern float com_engineHz_latched;
extern int64 com_engineHz_numerator;
extern int64 com_engineHz_denominator;

// Returns the msec the frame starts on
ID_INLINE int FRAME_TO_MSEC( int64 frame )
{
	return ( int )( ( frame * com_engineHz_numerator ) / com_engineHz_denominator );
}
// Rounds DOWN to the nearest frame
ID_INLINE int MSEC_TO_FRAME_FLOOR( int msec )
{
	return ( int )( ( ( ( int64 )msec * com_engineHz_denominator ) + ( com_engineHz_denominator - 1 ) ) / com_engineHz_numerator );
}
// Rounds UP to the nearest frame
ID_INLINE int MSEC_TO_FRAME_CEIL( int msec )
{
	return ( int )( ( ( ( int64 )msec * com_engineHz_denominator ) + ( com_engineHz_numerator - 1 ) ) / com_engineHz_numerator );
}
// Aligns msec so it starts on a frame bondary
ID_INLINE int MSEC_ALIGN_TO_FRAME( int msec )
{
	return FRAME_TO_MSEC( MSEC_TO_FRAME_CEIL( msec ) );
}

struct lobbyConnectInfo_t;

ID_INLINE void BeginProfileNamedEventColor( uint32 color, VERIFY_FORMAT_STRING const char* szName )
{
}
ID_INLINE void EndProfileNamedEvent()
{
}

ID_INLINE void BeginProfileNamedEvent( VERIFY_FORMAT_STRING const char* szName )
{
	BeginProfileNamedEventColor( ( uint32 ) 0xFF00FF00, szName );
}

class idScopedProfileEvent
{
public:
	idScopedProfileEvent( const char* name )
	{
		BeginProfileNamedEvent( name );
	}
	~idScopedProfileEvent()
	{
		EndProfileNamedEvent();
	}
};

#define SCOPED_PROFILE_EVENT( x ) idScopedProfileEvent scopedProfileEvent_##__LINE__( x )

ID_INLINE bool BeginTraceRecording( const char* szName )
{
	return false;
}

ID_INLINE bool EndTraceRecording()
{
	return false;
}

typedef enum
{
	EDITOR_NONE					= 0,
	EDITOR_GUI					= BIT( 1 ),
	EDITOR_DEBUGGER				= BIT( 2 ),
	EDITOR_SCRIPT				= BIT( 3 ),
	EDITOR_LIGHT				= BIT( 4 ),
	EDITOR_SOUND				= BIT( 5 ),
	EDITOR_DECL					= BIT( 6 ),
	EDITOR_AF					= BIT( 7 ),
	EDITOR_PARTICLE				= BIT( 8 ),
	EDITOR_PDA					= BIT( 9 ),
	EDITOR_AAS					= BIT( 10 ),
	EDITOR_MATERIAL				= BIT( 11 )
} toolFlag_t;

#define STRTABLE_ID				"#str_"
#define STRTABLE_ID_LENGTH		5

extern idCVar		com_version;
extern idCVar		com_developer;
extern idCVar		com_allowConsole;
extern idCVar		com_speeds;
extern idCVar		com_showFPS;
extern idCVar		com_showMemoryUsage;
extern idCVar		com_updateLoadSize;
extern idCVar		com_productionMode;

extern int			com_editors;			// currently opened editor(s)
struct MemInfo_t
{
	idStr			filebase;
	
	int				total;
	int				assetTotals;
	
	// memory manager totals
	int				memoryManagerTotal;
	
	// subsystem totals
	int				gameSubsystemTotal;
	int				renderSubsystemTotal;
	
	// asset totals
	int				imageAssetsTotal;
	int				modelAssetsTotal;
	int				soundAssetsTotal;
};

static const int	MAX_LOGGED_STATS = 60 * 120;		// log every half second

// RB begin
#if defined(USE_DOOMCLASSIC)
enum currentGame_t
{
	DOOM_CLASSIC,
	DOOM2_CLASSIC,
	DOOM3_BFG
};
#endif
// RB end

#include "framework/ICommon.hpp"

extern idCommon* 		common;

#endif /* !__COMMON_H__ */
