/*
===========================================================================

Doom 3 BFG Edition GPL Source Code
Copyright (C) 1993-2012 id Software LLC, a ZeniMax Media company.
Copyright (C) 2012-2016 Robert Beckebans
Copyright (C) 2014-2016 Kot in Action Creative Artel
Copyright (C) 2018-2019 BlackPhrase

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

#include "precompiled.h"

#include "SbSoundFade.hpp"

/*
================================================================================================

	idSoundFade

================================================================================================
*/

/*
========================
idSoundFade::Clear
========================
*/
void idSoundFade::Clear()
{
	fadeStartTime = 0;
	fadeEndTime = 0;
	fadeStartVolume = 0.0f;
	fadeEndVolume = 0.0f;
}

/*
========================
idSoundFade::SetVolume
========================
*/
void idSoundFade::SetVolume( float to )
{
	fadeStartVolume = to;
	fadeEndVolume = to;
	fadeStartTime = 0;
	fadeEndTime = 0;
}

/*
========================
idSoundFade::Fade
========================
*/
void idSoundFade::Fade( float to, int length, int soundTime )
{
	int startTime = soundTime;
	// if it is already fading to this volume at this rate, don't change it
	if( fadeEndTime == startTime + length && fadeEndVolume == to )
	{
		return;
	}
	fadeStartVolume = GetVolume( soundTime );
	fadeEndVolume = to;
	fadeStartTime = startTime;
	fadeEndTime = startTime + length;
}

/*
========================
idSoundFade::GetVolume
========================
*/
float idSoundFade::GetVolume( const int soundTime ) const
{
	const float fadeDuration = ( fadeEndTime - fadeStartTime );
	const int currentTime = soundTime;
	const float playTime = ( currentTime - fadeStartTime );
	if( fadeDuration <= 0.0f )
	{
		return fadeEndVolume;
	}
	else if( currentTime >= fadeEndTime )
	{
		return fadeEndVolume;
	}
	else if( currentTime > fadeStartTime )
	{
		return fadeStartVolume + ( fadeEndVolume - fadeStartVolume ) * playTime / fadeDuration;
	}
	else
	{
		return fadeStartVolume;
	}
}