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

#ifndef __SOUND__
#define __SOUND__

//#include "../renderer/Cinematic.h"      // for cinData_t, idCinematic
//#include "../framework/File_Manifest.h"  // for idPreloadManifest, etc
//#include "../framework/Common.h"        // for MemInfo_t
//#include "../framework/DeclManager.h"   // for declManager, idDeclManager, etc

//namespace BFG
//{

//extern idCVar timescale;

/*
===============================================================================

	SOUND SHADER DECL

===============================================================================
*/
const float DB_SILENCE = -60.0f;

class idSoundSample;
// these options can be overriden from sound shader defaults on a per-emitter and per-channel basis
typedef struct
{
	float					minDistance;
	float					maxDistance;
	float					volume;					// in dB.  Negative values get quieter
	float					shakes;
	int						soundShaderFlags;		// SSF_* bit flags
	int						soundClass;				// for global fading of sounds
} soundShaderParms_t;


#include "SbSoundShader.hpp"

#include "sound/ISoundEmitter.hpp"

/*
===============================================================================

	SOUND WORLD

There can be multiple independent sound worlds, just as there can be multiple
independent render worlds.  The prime example is the editor sound preview
option existing simultaniously with a live game.
===============================================================================
*/

#include "sound/ISoundWorld.hpp"

/*
===============================================================================

	SOUND SYSTEM

===============================================================================
*/

typedef struct
{
	idStr					name;
	idStr					format;
	int						numChannels;
	int						numSamplesPerSecond;
	int						num44kHzSamples;
	int						numBytes;
	bool					looping;
	float					lastVolume;
	int						start44kHzTime;
	int						current44kHzTime;
} soundDecoderInfo_t;

#include "sound/ISoundSystem.hpp"

extern idSoundSystem*	soundSystem;

//} // namespace BFG

#endif /* !__SOUND__ */
