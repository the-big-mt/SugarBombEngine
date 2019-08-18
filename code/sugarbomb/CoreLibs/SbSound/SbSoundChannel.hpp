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

#include "SbSound/SoundShaderParms.hpp"
#include "SbSoundFade.hpp"

//namespace sbe
//{

struct idSoundShader;
struct idSoundSample;
class idSoundEmitterLocal;
class idSoundVoice;

/*
================================================
idSoundChannel
================================================
*/
class idSoundChannel
{
public:
	bool CanMute() const;

	void Mute();
	bool CheckForCompletion(int currentTime);

	void UpdateVolume(int currentTime);
	void UpdateHardware(float volumeAdd, int currentTime);

	// returns true if this channel is marked as looping
	bool IsLooping() const;

	class idSoundEmitterLocal *emitter;

	int startTime;
	int endTime;
	int logicalChannel;
	bool allowSlow;

	soundShaderParms_t parms; // combines shader parms and per-channel overrides
	const idSoundShader *soundShader;
	idSoundSample *leadinSample;
	idSoundSample *loopingSample;
	idSoundFade volumeFade;

	float volumeDB;         // last volume at which this channel will play (calculated in UpdateVolume)
	float currentAmplitude; // current amplitude on the hardware voice

	// hardwareVoice will be freed and NULL'd when a sound is out of range,
	// and reallocated when it comes back in range
	idSoundVoice *hardwareVoice;

	// only allocated by the soundWorld block allocator
	idSoundChannel();
	~idSoundChannel();
};

//}; // namespace sbe