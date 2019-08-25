/*
*******************************************************************************

Copyright (C) 1993-2012 id Software LLC, a ZeniMax Media company.
Copyright (C) 2019 BlackPhrase

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

#pragma once

/*
===============================================================================

	SOUND EMITTER

===============================================================================
*/

// sound channels
static const int SCHANNEL_ANY = 0;	// used in queries and commands to effect every channel at once, in
// startSound to have it not override any other channel
static const int SCHANNEL_ONE = 1;	// any following integer can be used as a channel number

typedef int s_channelType;	// the game uses its own series of enums, and we don't want to require casts

struct soundShaderParms_t;
struct idSoundShader;
class idVec3;

struct idSoundEmitter
{
public:
	virtual					~idSoundEmitter() {}
	
	// a non-immediate free will let all currently playing sounds complete
	// soundEmitters are not actually deleted, they are just marked as
	// reusable by the soundWorld
	virtual void			Free( bool immediate ) = 0;
	
	// the parms specified will be the default overrides for all sounds started on this emitter.
	// nullptr is acceptable for parms
	virtual void			UpdateEmitter( const idVec3& origin, int listenerId, const soundShaderParms_t* parms ) = 0;
	
	// returns the length of the started sound in msec
	virtual int				StartSound( const idSoundShader* shader, const s_channelType channel, float diversity = 0, int shaderFlags = 0, bool allowSlow = true ) = 0;
	
	// pass SCHANNEL_ANY to effect all channels
	virtual void			ModifySound( const s_channelType channel, const soundShaderParms_t* parms ) = 0;
	virtual void			StopSound( const s_channelType channel ) = 0;
	// to is in Db, over is in seconds
	virtual void			FadeSound( const s_channelType channel, float to, float over ) = 0;
	
	// returns true if there are any sounds playing from this emitter.  There is some conservative
	// slop at the end to remove inconsistent race conditions with the sound thread updates.
	// FIXME: network game: on a dedicated server, this will always be false
	virtual bool			CurrentlyPlaying( const s_channelType channel = SCHANNEL_ANY ) const = 0;
	
	// returns a 0.0 to 1.0 value based on the current sound amplitude, allowing
	// graphic effects to be modified in time with the audio.
	// just samples the raw wav file, it doesn't account for volume overrides in the
	virtual	float			CurrentAmplitude() = 0;
	
	// for save games.  Index will always be > 0
	virtual	int				Index() const = 0;
};