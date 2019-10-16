/*
*******************************************************************************

Copyright (C) 1993-2012 id Software LLC, a ZeniMax Media company.
Copyright (C) 2018-2019 SugarBombEngine Developers

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

#include "SbRenderer/Cinematic.h" // for cinData_t, idCinematic

namespace sbe
{

struct MemInfo_t;
struct idRenderWorld;
struct idSoundWorld;
class idDecl;
class idPreloadManifest;

struct ISoundSystem
{
	// All non-hardware initialization.
	virtual void			Init() = 0;
	
	// Shutdown routine.
	virtual	void			Shutdown() = 0;
	
	// The renderWorld is used for visualization and light amplitude sampling.
	virtual idSoundWorld* 	AllocSoundWorld( idRenderWorld* rw ) = 0;
	virtual void			FreeSoundWorld( idSoundWorld* sw ) = 0;
	
	// Specifying nullptr will cause silence to be played.
	virtual void			SetPlayingSoundWorld( idSoundWorld* soundWorld ) = 0;
	
	// Some tools, like the sound dialog, may be used in both the game and the editor
	// This can return nullptr, so check!
	virtual idSoundWorld* 	GetPlayingSoundWorld() = 0;
	
	// Sends the current playing sound world information to the sound hardware.
	virtual void			Render(float afTimeStep) = 0;
	
	virtual void			MuteBackgroundMusic( bool mute ) = 0;
	
	// Sets the final output volume to 0.
	virtual void			SetMute( bool mute ) = 0;
	virtual bool			IsMuted() = 0;
	
	// Called by the decl system when a sound decl is reloaded
	virtual void			OnReloadSound( const idDecl* sound ) = 0;
	
	// Called before freeing any sound sample resources
	virtual void			StopAllSounds() = 0;
	
	// May be called to free memory for level loads
	virtual void			InitStreamBuffers() = 0;
	virtual void			FreeStreamBuffers() = 0;
	
	// video playback needs to get this
	virtual void* 			GetIXAudio2() const = 0; // FIXME: stupid name if we have other backends
	
#if defined(USE_OPENAL)
	virtual void*			GetOpenALDevice() const = 0;
#endif
	
	// for the sound level meter window
	virtual cinData_t		ImageForTime( const int milliseconds, const bool waveform ) = 0;
	
	// Free all sounds loaded during the last map load
	virtual	void			BeginLevelLoad() = 0;
	
	// Load all sounds marked as used this level
	virtual	void			EndLevelLoad() = 0;
	
	virtual void			Preload( idPreloadManifest& preload ) = 0;
	
	// prints memory info
	virtual void			PrintMemInfo( MemInfo_t* mi ) = 0;
};

struct idSoundSystem : public ISoundSystem
{
	virtual					~idSoundSystem() = default;
};


{

}; // namespace sbe