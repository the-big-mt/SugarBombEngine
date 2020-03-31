/*
*******************************************************************************

Copyright (C) 1993-2012 id Software LLC, a ZeniMax Media company.
Copyright (C) 2013 Robert Beckebans
Copyright (C) 2019 SugarBombEngine Developers

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

//#ifdef __APPLE__
//#	include <OpenAL/al.h>
//#else
//#	include <AL/al.h>
//#endif

#include "framework/ICVarSystem.hpp"

#include "idlib/Lib.h"
#include "idlib/containers/List.h"
#include "idlib/sys/sys_assert.h"
#include "idlib/sys/sys_types.h"

#include "SbAL_SoundVoice.hpp"
#include "SbAL_SoundSample.hpp"
#include "snd_local.h"
#include "SbWaveFile.hpp"
//#include "SbSoundDefines.hpp"

#include "SbSound/SbSound.hpp"

idCVar s_skipHardwareSets("s_skipHardwareSets", "0", CVAR_BOOL, "Do all calculation, but skip XA2 calls");
idCVar s_debugHardware("s_debugHardware", "0", CVAR_BOOL, "Print a message any time a hardware voice changes");

/*
========================
idSoundVoice_OpenAL::ResetSampleRate
========================
*/
// TODO: unused atm
void SbSoundVoice_OpenAL::SetSampleRate(uint32 newSampleRate, uint32 operationSet)
{
	/*
	if( pSourceVoice == nullptr || leadinSample == nullptr )
	{
		return;
	}
	
	sampleRate = newSampleRate;
	
	XAUDIO2_FILTER_PARAMETERS filter;
	filter.Type = LowPassFilter;
	filter.OneOverQ = 1.0f;			// [0.0f, XAUDIO2_MAX_FILTER_ONEOVERQ]
	float cutoffFrequency = 1000.0f / Max( 0.01f, occlusion );
	if( cutoffFrequency * 6.0f >= ( float )sampleRate )
	{
		filter.Frequency = XAUDIO2_MAX_FILTER_FREQUENCY;
	}
	else
	{
		filter.Frequency = 2.0f * idMath::Sin( idMath::PI * cutoffFrequency / ( float )sampleRate );
	}
	assert( filter.Frequency >= 0.0f && filter.Frequency <= XAUDIO2_MAX_FILTER_FREQUENCY );
	filter.Frequency = idMath::ClampFloat( 0.0f, XAUDIO2_MAX_FILTER_FREQUENCY, filter.Frequency );
	
	pSourceVoice->SetFilterParameters( &filter, operationSet );
	
	float freqRatio = pitch * ( float )sampleRate / ( float )sourceVoiceRate;
	assert( freqRatio >= XAUDIO2_MIN_FREQ_RATIO && freqRatio <= XAUDIO2_MAX_FREQ_RATIO );
	freqRatio = idMath::ClampFloat( XAUDIO2_MIN_FREQ_RATIO, XAUDIO2_MAX_FREQ_RATIO, freqRatio );
	
	// if the value specified for maxFreqRatio is too high for the specified format, the call to CreateSourceVoice will fail
	if( numChannels == 1 )
	{
		assert( freqRatio * ( float )SYSTEM_SAMPLE_RATE <= XAUDIO2_MAX_RATIO_TIMES_RATE_XMA_MONO );
	}
	else
	{
		assert( freqRatio * ( float )SYSTEM_SAMPLE_RATE <= XAUDIO2_MAX_RATIO_TIMES_RATE_XMA_MULTICHANNEL );
	}
	
	pSourceVoice->SetFrequencyRatio( freqRatio, operationSet );
	*/
};