/*
*******************************************************************************

Copyright (C) 1993-2012 id Software LLC, a ZeniMax Media company.
Copyright (C) 2013-2016 Robert Beckebans
Copyright (C) 2014-2016 Kot in Action Creative Artel
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

#include "SbSound/ISoundEmitter.hpp"
#include "SbSound/SoundShaderParms.hpp"

#include "idlib/containers/StaticList.h"
#include "idlib/math/Vector.h"

//namespace sbe
//{

// Maximum number of SoundChannels for a single SoundEmitter.
// This is probably excessive...
const int MAX_CHANNELS_PER_EMITTER = 16;

class idSoundWorldLocal;
class idSoundChannel;
class idDecl;

/*
================================================
idSoundEmitterLocal
================================================
*/
class idSoundEmitterLocal : public idSoundEmitter
{
public:
	virtual void Free(bool immediate);

	virtual void Reset();

	virtual void UpdateEmitter(const idVec3 &origin, int listenerId, const soundShaderParms_t *parms);

	virtual int StartSound(const idSoundShader *shader, const s_channelType channel, float diversity = 0, int shaderFlags = 0, bool allowSlow = true);

	virtual void ModifySound(const s_channelType channel, const soundShaderParms_t *parms);
	virtual void StopSound(const s_channelType channel);

	virtual void FadeSound(const s_channelType channel, float to, float over);

	virtual bool CurrentlyPlaying(const s_channelType channel = SCHANNEL_ANY) const;

	virtual float CurrentAmplitude();

	virtual int Index() const;

	//----------------------------------------------

	void Init(int i, idSoundWorldLocal *sw);

	// Returns true if the emitter should be freed.
	bool CheckForCompletion(int currentTime);

	void OverrideParms(const soundShaderParms_t *base, const soundShaderParms_t *over, soundShaderParms_t *out);

	void Update(int currentTime);
	void OnReloadSound(const idDecl *decl);

	//----------------------------------------------

	idSoundWorldLocal *soundWorld; // the world that holds this emitter

	int index;    // in world emitter list
	bool canFree; // if true, this emitter can be canFree (once channels.Num() == 0)

	// a single soundEmitter can have many channels playing from the same point
	idStaticList<idSoundChannel *, MAX_CHANNELS_PER_EMITTER> channels;

	//----- set by UpdateEmitter -----
	idVec3 origin;
	soundShaderParms_t parms;
	int emitterId; // sounds will be full volume when emitterId == listenerId

	//----- set by Update -----
	int lastValidPortalArea;
	float directDistance;
	float spatializedDistance;
	idVec3 spatializedOrigin;

	// sound emitters are only allocated by the soundWorld block allocator
	idSoundEmitterLocal();
	virtual ~idSoundEmitterLocal();
};

//}; // namespace sbe