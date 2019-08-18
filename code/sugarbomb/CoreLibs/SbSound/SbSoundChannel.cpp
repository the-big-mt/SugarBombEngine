/*
*******************************************************************************

Copyright (C) 1993-2012 id Software LLC, a ZeniMax Media company.
Copyright (C) 2012-2016 Robert Beckebans
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

#pragma hdrstop

#include "precompiled.h"

#include "framework/CVar.hpp"

#include "SbSoundChannel.hpp"
#include "SbSoundEmitter.hpp"
#include "SbSoundShader.hpp" // TODO: #include "SbSound/ISoundShader.hpp"
#include "SbSoundWorld.hpp"
#include "SbSoundSystem.hpp"
#include "SbSoundDefines.hpp"
#include "SbSound/SoundTypes.hpp"

extern idCVar timescale;
extern idCVar s_playDefaultSound;

idCVar s_centerFractionVO("s_centerFractionVO", "0.75", CVAR_FLOAT, "Portion of VO sounds routed to the center channel");

/*
========================
LinearToDB
========================
*/
ID_INLINE_EXTERN float LinearToDB(float linear)
{
	return (linear > 0.0f) ? (idMath::Log(linear) * (6.0f / 0.693147181f)) : -999.0f;
}

/*
========================
idSoundChannel::idSoundChannel
========================
*/
idSoundChannel::idSoundChannel()
{
	emitter = nullptr;
	hardwareVoice = nullptr;

	startTime = 0;
	endTime = 0;
	leadinSample = nullptr;
	loopingSample = nullptr;
	logicalChannel = SCHANNEL_ANY;
	allowSlow = false;
	soundShader = nullptr;

	volumeFade.Clear();

	volumeDB = DB_SILENCE;
	currentAmplitude = 0.0f;
}

/*
========================
idSoundChannel::~idSoundChannel
========================
*/
idSoundChannel::~idSoundChannel()
{
}

/*
========================
idSoundChannel::CanMute
Never actually mute VO because we can't restart them precisely enough for lip syncing to not fuck up
========================
*/
bool idSoundChannel::CanMute() const
{
	return true;
}

/*
========================
idSoundChannel::Mute

A muted sound is considered still running, and can restart when a listener
gets close enough.
========================
*/
void idSoundChannel::Mute()
{
	if(hardwareVoice != nullptr)
	{
		soundSystemLocal.FreeVoice(hardwareVoice);
		hardwareVoice = nullptr;
	}
}

/*
========================
idSoundChannel::IsLooping
========================
*/
bool idSoundChannel::IsLooping() const
{
	return (parms.soundShaderFlags & SSF_LOOPING) != 0;
}

/*
========================
idSoundChannel::CheckForCompletion
========================
*/
bool idSoundChannel::CheckForCompletion(int currentTime)
{
	if(leadinSample == nullptr)
	{
		return true;
	}
	// endTime of 0 indicates a sound should loop forever
	if(endTime > 0 && endTime < currentTime)
	{
		return true;
	}
	return false;
}

/*
========================
idSoundChannel::UpdateVolume
========================
*/
void idSoundChannel::UpdateVolume(int currentTime)
{
	idSoundWorldLocal *soundWorld = emitter->soundWorld;

	volumeDB = DB_SILENCE;
	currentAmplitude = 0.0f;

	if(leadinSample == nullptr)
	{
		return;
	}
	if(startTime > currentTime)
	{
		return;
	}
	if(endTime > 0 && endTime < currentTime)
	{
		return;
	}

	// if you don't want to hear all the beeps from missing sounds
	if(leadinSample->IsDefault() && !s_playDefaultSound.GetBool())
	{
		return;
	}

	bool emitterIsListener = (emitter->emitterId == soundWorld->listener.id);

	// if it is a private sound, set the volume to zero unless we match the listener.id
	if(parms.soundShaderFlags & SSF_PRIVATE_SOUND)
	{
		if(!emitterIsListener)
		{
			return;
		}
	}
	if(parms.soundShaderFlags & SSF_ANTI_PRIVATE_SOUND)
	{
		if(emitterIsListener)
		{
			return;
		}
	}

	// volume fading
	float newVolumeDB = parms.volume;
	newVolumeDB += volumeFade.GetVolume(currentTime);
	newVolumeDB += soundWorld->volumeFade.GetVolume(currentTime);
	newVolumeDB += soundWorld->pauseFade.GetVolume(currentTime);
	if(parms.soundClass >= 0 && parms.soundClass < SOUND_MAX_CLASSES)
	{
		newVolumeDB += soundWorld->soundClassFade[parms.soundClass].GetVolume(currentTime);
	}

	bool global = (parms.soundShaderFlags & SSF_GLOBAL) != 0;

	// attenuation
	if(!global && !emitterIsListener)
	{
		float distance = (parms.soundShaderFlags & SSF_NO_OCCLUSION) == 0 ? emitter->spatializedDistance : emitter->directDistance;
		float mindist = parms.minDistance;
		float maxdist = parms.maxDistance;
		if(distance >= maxdist)
		{
			newVolumeDB = DB_SILENCE;
		}
		else if((distance > mindist) && (maxdist > mindist))
		{
			float f = (distance - mindist) / (maxdist - mindist);
			newVolumeDB += LinearToDB(Square(1.0f - f));
		}
	}

	if(soundSystemLocal.musicMuted && (parms.soundShaderFlags & SSF_MUSIC) != 0)
	{
		newVolumeDB = DB_SILENCE;
	}

	// store the new volume on the channel
	volumeDB = newVolumeDB;

	// keep track of the maximum volume
	float currentVolumeDB = newVolumeDB;
	if(hardwareVoice != nullptr)
	{
		float amplitude = hardwareVoice->GetAmplitude();
		if(amplitude <= 0.0f)
		{
			currentVolumeDB = DB_SILENCE;
		}
		else
		{
			currentVolumeDB += LinearToDB(amplitude);
		}
		currentAmplitude = amplitude;
	}
}

/*
========================
idSoundChannel::UpdateHardware
========================
*/
void idSoundChannel::UpdateHardware(float volumeAdd, int currentTime)
{
	idSoundWorldLocal *soundWorld = emitter->soundWorld;

	if(soundWorld == nullptr)
	{
		return;
	}
	if(leadinSample == nullptr)
	{
		return;
	}
	if(startTime > currentTime)
	{
		return;
	}
	if(endTime > 0 && endTime < currentTime)
	{
		return;
	}

	// convert volumes from decibels to linear
	float volume = Max(0.0f, DBtoLinear(volumeDB + volumeAdd));

	if((parms.soundShaderFlags & SSF_UNCLAMPED) == 0)
	{
		volume = Min(1.0f, volume);
	}

	bool global = (parms.soundShaderFlags & SSF_GLOBAL) != 0;
	bool omni = (parms.soundShaderFlags & SSF_OMNIDIRECTIONAL) != 0;
	bool emitterIsListener = (emitter->emitterId == soundWorld->listener.id);

	int startOffset = 0;
	bool issueStart = false;

	if(hardwareVoice == nullptr)
	{
		if(volume <= 0.00001f)
		{
			return;
		}

		hardwareVoice = soundSystemLocal.AllocateVoice(leadinSample, loopingSample);

		if(hardwareVoice == nullptr)
		{
			return;
		}

		issueStart = true;
		startOffset = currentTime - startTime;
	}

	if(omni || global || emitterIsListener)
	{
		hardwareVoice->SetPosition(vec3_zero);
	}
	else
	{
		hardwareVoice->SetPosition((emitter->spatializedOrigin - soundWorld->listener.pos) * soundWorld->listener.axis.Transpose());
	}
	if(parms.soundShaderFlags & SSF_VO)
	{
		hardwareVoice->SetCenterChannel(s_centerFractionVO.GetFloat());
	}
	else
	{
		hardwareVoice->SetCenterChannel(0.0f);
	}

	hardwareVoice->SetGain(volume);
	hardwareVoice->SetInnerRadius(parms.minDistance * METERS_TO_DOOM);
	hardwareVoice->SetPitch(soundWorld->slowmoSpeed * idMath::ClampFloat(0.2f, 5.0f, timescale.GetFloat()));

	if(soundWorld->enviroSuitActive)
	{
		hardwareVoice->SetOcclusion(0.5f);
	}
	else
	{
		hardwareVoice->SetOcclusion(0.0f);
	}

	if(issueStart)
	{
		hardwareVoice->Start(startOffset, parms.soundShaderFlags | (parms.shakes == 0.0f ? SSF_NO_FLICKER : 0));
	}
	else
	{
		hardwareVoice->Update();
	}
}