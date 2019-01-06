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

// unfortunately, our minDistance / maxDistance is specified in meters, and
// we have far too many of them to change at this time.
const float DOOM_TO_METERS = 0.0254f;					// doom to meters
const float METERS_TO_DOOM = ( 1.0f / DOOM_TO_METERS );	// meters to doom

const float DB_SILENCE = -60.0f;

class idSoundSample;

// sound shader flags
static const int	SSF_PRIVATE_SOUND =		BIT( 0 );	// only plays for the current listenerId
static const int	SSF_ANTI_PRIVATE_SOUND = BIT( 1 );	// plays for everyone but the current listenerId
static const int	SSF_NO_OCCLUSION =		BIT( 2 );	// don't flow through portals, only use straight line
static const int	SSF_GLOBAL =			BIT( 3 );	// play full volume to all speakers and all listeners
static const int	SSF_OMNIDIRECTIONAL =	BIT( 4 );	// fall off with distance, but play same volume in all speakers
static const int	SSF_LOOPING =			BIT( 5 );	// repeat the sound continuously
static const int	SSF_PLAY_ONCE =			BIT( 6 );	// never restart if already playing on any channel of a given emitter
static const int	SSF_UNCLAMPED =			BIT( 7 );	// don't clamp calculated volumes at 1.0
static const int	SSF_NO_FLICKER =		BIT( 8 );	// always return 1.0 for volume queries
static const int	SSF_NO_DUPS =			BIT( 9 );	// try not to play the same sound twice in a row
static const int	SSF_VO =				BIT( 10 ); // VO - direct a portion of the sound through the center channel (set automatically on shaders that contain files that start with "sound/vo/")
static const int	SSF_MUSIC =				BIT( 11 ); // Music - Muted when the player is playing his own music

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

// sound classes are used to fade most sounds down inside cinematics, leaving dialog
// flagged with a non-zero class full volume
const int		SOUND_MAX_CLASSES		= 4;

// it is somewhat tempting to make this a virtual class to hide the private
// details here, but that doesn't fit easily with the decl manager at the moment.
class idSoundShader : public idDecl
{
public:
	idSoundShader();
	virtual					~idSoundShader();
	
	virtual size_t			Size() const;
	virtual bool			SetDefaultText();
	virtual const char* 	DefaultDefinition() const;
	virtual bool			Parse( const char* text, const int textLength, bool allowBinaryVersion );
	virtual void			FreeData();
	virtual void			List() const;
	
	// so the editor can draw correct default sound spheres
	// this is currently defined as meters, which sucks, IMHO.
	virtual float			GetMinDistance() const;		// FIXME: replace this with a GetSoundShaderParms()
	virtual float			GetMaxDistance() const;
	
	// returns NULL if an AltSound isn't defined in the shader.
	// we use this for pairing a specific broken light sound with a normal light sound
	virtual const idSoundShader* GetAltSound() const;
	
	virtual bool			HasDefaultSound() const;
	
	virtual const soundShaderParms_t* GetParms() const;
	virtual int				GetNumSounds() const;
	virtual const char* 	GetSound( int index ) const;
	
private:
	friend class idSoundWorldLocal;
	friend class idSoundEmitterLocal;
	friend class idSoundChannel;
	
	// options from sound shader text
	soundShaderParms_t		parms;			// can be overriden on a per-channel basis
	
	int						speakerMask;
	const idSoundShader* 	altSound;
	
	bool					leadin;			// true if this sound has a leadin
	float					leadinVolume;	// allows light breaking leadin sounds to be much louder than the broken loop
	
	idList<idSoundSample*, TAG_AUDIO>	entries;
	
private:
	void					Init();
	bool					ParseShader( idLexer& src );
};

/*
===============================================================================

	SOUND EMITTER

===============================================================================
*/

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
