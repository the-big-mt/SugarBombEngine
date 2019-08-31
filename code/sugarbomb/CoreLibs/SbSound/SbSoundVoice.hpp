/*
*******************************************************************************

Copyright (C) 1993-2012 id Software LLC, a ZeniMax Media company.
Copyright (C) 2013 Robert Beckebans
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

#include "SbWaveFile.hpp"
#include "SbSoundDefines.hpp"

#include "idlib/Heap.h"
#include "idlib/containers/List.h"
#include "idlib/math/Vector.h"
#include "idlib/sys/sys_types.h"

//namespace sbe
//{

class idSoundSample;

/*
========================
MsecToSamples
========================
*/
ID_INLINE_EXTERN uint32 MsecToSamples(uint32 msec, uint32 sampleRate)
{
	return (msec * (sampleRate / 100)) / 10;
};

/*
================================================
idSoundVoice_Base
================================================
*/
class SbSoundVoice_Base
{
public:
	SbSoundVoice_Base();

	static void InitSurround(int outputChannels, int channelMask);

	void CalculateSurround(int srcChannels, float pLevelMatrix[MAX_CHANNELS_PER_VOICE * MAX_CHANNELS_PER_VOICE], float scale);

	// RB begin
	virtual void SetPosition(const idVec3 &p)
	{
		position = p;
	};

	virtual void SetGain(float g)
	{
		gain = g;
	};

	virtual void SetPitch(float p)
	{
		pitch = p;
	};
	// RB end

	void SetCenterChannel(float c)
	{
		centerChannel = c;
	};

	void SetInnerRadius(float r)
	{
		innerRadius = r;
	};
	void SetChannelMask(uint32 mask)
	{
		channelMask = mask;
	};

	const idSoundSample *GetCurrentSample() const;

	// Controls the low pass filter, where 0.0f = no filtering, 1.0f = full filter
	void SetOcclusion(float f)
	{
		occlusion = f;
	};

	float GetGain() const
	{
		return gain;
	};
	float GetPitch() const
	{
		return pitch;
	};

	virtual ~SbSoundVoice_Base() = default;
protected:
	idVec3 position{};     // Position of the sound relative to listener
	float gain{0.0f};          // Volume (0-1)
	float centerChannel{0.0f}; // Value (0-1) which indicates how much of this voice goes to the center channel
	float pitch{0.0f};         // Pitch multiplier
	float innerRadius{0.0f};   // Anything closer than this is omni
	float occlusion{0.0f};     // How much of this sound is occluded (0-1)
	uint32 channelMask{0};  // Set to override the default channel mask

	// These are some setting used to do SSF_DISTANCE_BASED_STERO blending
	float innerSampleRangeSqr{0.0f};
	float outerSampleRangeSqr{0.0f};

	idList<idSoundSample *, TAG_AUDIO> samples;

	// These are constants which are initialized with InitSurround
	//-------------------------------------------------------------

	static idVec2 speakerPositions[idWaveFile::CHANNEL_INDEX_MAX];

	// This is to figure out which speakers are "next to" this one
	static int speakerLeft[idWaveFile::CHANNEL_INDEX_MAX];
	static int speakerRight[idWaveFile::CHANNEL_INDEX_MAX];

	// Number of channels in the output hardware
	static int dstChannels{0};

	// Mask indicating which speakers exist in the hardware configuration
	static int dstMask{0};

	// dstMap maps a destination channel to a speaker
	// invMap maps a speaker to a destination channel
	static int dstCenter{0};
	static int dstLFE{0};
	static int dstMap[MAX_CHANNELS_PER_VOICE];
	static int invMap[idWaveFile::CHANNEL_INDEX_MAX];

	// specifies what volume to specify for each channel when a speaker is omni
	static float omniLevel{0.0f};
};

//} // namespace sbe