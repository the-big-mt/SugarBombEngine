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

/// @file

#pragma once

/*
===============================================================================

	SOUND WORLD

There can be multiple independent sound worlds, just as there can be multiple
independent render worlds.  The prime example is the editor sound preview
option existing simultaniously with a live game.
===============================================================================
*/

struct idSoundWorld
{
	virtual					~idSoundWorld() {}
	
	// call at each map start
	virtual void			ClearAllSoundEmitters() = 0;
	virtual void			StopAllSounds() = 0;
	
	// get a new emitter that can play sounds in this world
	virtual idSoundEmitter* AllocSoundEmitter() = 0;
	
	// for load games, index 0 will return NULL
	virtual idSoundEmitter* EmitterForIndex( int index ) = 0;
	
	// query sound samples from all emitters reaching a given listener
	virtual float			CurrentShakeAmplitude() = 0;
	
	// where is the camera/microphone
	// listenerId allows listener-private and antiPrivate sounds to be filtered
	virtual void			PlaceListener( const idVec3& origin, const idMat3& axis, const int listenerId ) = 0;
	
	// fade all sounds in the world with a given shader soundClass
	// to is in Db, over is in seconds
	virtual void			FadeSoundClasses( const int soundClass, const float to, const float over ) = 0;
	
	// menu sounds
	virtual	int				PlayShaderDirectly( const char* name, int channel = -1 ) = 0;
	
	// dumps the current state and begins archiving commands
	virtual void			StartWritingDemo( idDemoFile* demo ) = 0;
	virtual void			StopWritingDemo() = 0;
	
	// read a sound command from a demo file
	virtual void			ProcessDemoCommand( idDemoFile* demo ) = 0;
	
	// when cinematics are skipped, we need to advance sound time this much
	virtual void			Skip( int time ) = 0;
	
	// pause and unpause the sound world
	virtual void			Pause() = 0;
	virtual void			UnPause() = 0;
	virtual bool			IsPaused() = 0;
	
	// Write the sound output to multiple wav files.  Note that this does not use the
	// work done by AsyncUpdate, it mixes explicitly in the foreground every PlaceOrigin(),
	// under the assumption that we are rendering out screenshots and the gameTime is going
	// much slower than real time.
	// path should not include an extension, and the generated filenames will be:
	// <path>_left.raw, <path>_right.raw, or <path>_51left.raw, <path>_51right.raw,
	// <path>_51center.raw, <path>_51lfe.raw, <path>_51backleft.raw, <path>_51backright.raw,
	// If only two channel mixing is enabled, the left and right .raw files will also be
	// combined into a stereo .wav file.
	virtual void			AVIOpen( const char* path, const char* name ) = 0;
	virtual void			AVIClose() = 0;
	
	// SaveGame / demo Support
	virtual void			WriteToSaveGame( idFile* savefile ) = 0;
	virtual void			ReadFromSaveGame( idFile* savefile ) = 0;
	
	virtual void			SetSlowmoSpeed( float speed ) = 0;
	virtual void			SetEnviroSuit( bool active ) = 0;
};