/*
===========================================================================

Doom 3 BFG Edition GPL Source Code
Copyright (C) 1993-2012 id Software LLC, a ZeniMax Media company.
Copyright (C) 2013-2016 Robert Beckebans
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

#pragma once

#include "SbSound/ISoundWorld.hpp"
#include "SbSoundFade.hpp"
#include "SbSoundDefines.hpp"

#include "idlib/Heap.h"
#include "idlib/math/Matrix.h"
#include "idlib/math/Vector.h"
#include "idlib/containers/List.h"

//namespace sbe
//{

struct idRenderWorld;
struct idCommon;
struct idConsole;
class idDecl;
class idSoundChannel;
class idSoundEmitterLocal;
class idSoundShader;

//------------------------
// Listener data
//------------------------
struct listener_t
{
	idMat3	axis;		// orientation of the listener
	idVec3	pos;		// position in meters
	int		id;			// the entity number, used to detect when a sound is local
	int		area;		// area number the listener is in
};

class idSoundWorldLocal : public idSoundWorld
{
public:
	idSoundWorldLocal(idCommon *apCommon, idConsole *apConsole);
	virtual					~idSoundWorldLocal();
	
	//------------------------
	// Functions from idSoundWorld, implemented in SoundWorld.cpp
	//------------------------
	
	// Called at map start
	virtual void			ClearAllSoundEmitters();
	
	// stop all playing sounds
	virtual void			StopAllSounds();
	
	// get a new emitter that can play sounds in this world
	virtual idSoundEmitter* AllocSoundEmitter();
	
	// for load games
	virtual idSoundEmitter* EmitterForIndex( int index );
	
	// query data from all emitters in the world
	virtual float			CurrentShakeAmplitude();
	
	// where is the camera
	virtual void			PlaceListener( const idVec3& origin, const idMat3& axis, const int listenerId );
	
	virtual void			WriteSoundShaderLoad( const idSoundShader* snd );
	
	// fade all sounds in the world with a given shader soundClass
	// to is in Db, over is in seconds
	virtual void			FadeSoundClasses( const int soundClass, const float to, const float over );
	
	// dumps the current state and begins archiving commands
	virtual void			StartWritingDemo( idDemoFile* demo );
	virtual void			StopWritingDemo();
	
	// read a sound command from a demo file
	virtual void			ProcessDemoCommand( idDemoFile* readDemo );
	
	// menu sounds
	virtual int				PlayShaderDirectly( const char* name, int channel = -1 );
	
	virtual void			Skip( int time );
	
	virtual void			Pause();
	virtual void			UnPause();
	virtual bool			IsPaused()
	{
		return isPaused;
	}
	
	virtual int				GetSoundTime();
	
	// avidump
	virtual void			AVIOpen( const char* path, const char* name );
	virtual void			AVIClose();
	
	// SaveGame Support
	virtual void			WriteToSaveGame( idFile* savefile );
	virtual void			ReadFromSaveGame( idFile* savefile );
	
	virtual void			SetSlowmoSpeed( float speed );
	virtual void			SetEnviroSuit( bool active );
	
	//=======================================
	
	//------------------------
	// Random stuff that's not exposed outside the sound system
	//------------------------
	void			Update();
	void			OnReloadSound( const idDecl* decl );
	
	idSoundChannel* 	AllocSoundChannel();
	void				FreeSoundChannel( idSoundChannel* );
	
public:
	// even though all these variables are public, nobody outside the sound system includes SoundWorld_local.h
	// so this is equivalent to making it private and friending all the other classes in the sound system
	
	idSoundFade			volumeFade;						// master volume knob for the entire world
	idSoundFade			soundClassFade[SOUND_MAX_CLASSES];
	
	idRenderWorld* 		renderWorld;	// for debug visualization and light amplitude sampling
	idDemoFile* 		writeDemo;		// if not NULL, archive commands here
	
	float				currentCushionDB;	// channels at or below this level will be faded to 0
	float				shakeAmp;			// last calculated shake amplitude
	
	listener_t			listener;
	idList<idSoundEmitterLocal*, TAG_AUDIO>	emitters;
	
	idSoundEmitter* 	localSound;			// for PlayShaderDirectly()
	
	idBlockAlloc<idSoundEmitterLocal, 16>	emitterAllocator;
	idBlockAlloc<idSoundChannel, 16>		channelAllocator;
	
	idSoundFade				pauseFade;
	int						pausedTime;
	int						accumulatedPauseTime;
	bool					isPaused;
	
	float					slowmoSpeed;
	bool					enviroSuitActive;
	
public:
	struct soundPortalTrace_t
	{
		int		portalArea;
		const soundPortalTrace_t* prevStack;
	};
	
	void			ResolveOrigin( const int stackDepth, const soundPortalTrace_t* prevStack, const int soundArea, const float dist, const idVec3& soundOrigin, idSoundEmitterLocal* def );
private:
	idCommon *common{nullptr};
	idConsole *console{nullptr};
};

//}; // namespace sbe