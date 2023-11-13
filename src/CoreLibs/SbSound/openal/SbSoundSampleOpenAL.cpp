/*
*******************************************************************************

Copyright (C) 2019-2020 SugarBombEngine Developers

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

*******************************************************************************
*/

/// @file

//*****************************************************************************

#include "SbSoundSampleOpenAL.hpp"

//*****************************************************************************

namespace sbe::SbSound
{

SbSoundSampleOpenAL::SbSoundSampleOpenAL()
{
};

/*
========================
idSoundSample_OpenAL::~idSoundSample_OpenAL
========================
*/
SbSoundSampleOpenAL::~SbSoundSampleOpenAL()
{
	FreeData();
};

void SbSoundSampleOpenAL::CreateOpenALBuffer()
{
	// build OpenAL buffer
	CheckALErrors();
	alGenBuffers(1, &mnOpenALBuffer);

	if(CheckALErrors() != AL_NO_ERROR)
		mSystem.Error("idSoundSample_OpenAL::CreateOpenALBuffer: error generating OpenAL hardware buffer");

	if(alIsBuffer(mnOpenALBuffer))
	{
		CheckALErrors();

		void *buffer = nullptr;
		uint32 bufferSize = 0;

		if(format.basic.formatTag == SbWaveFile::FORMAT_ADPCM)
		{
			// RB: decode SbWaveFile::FORMAT_ADPCM to SbWaveFile::FORMAT_PCM

			buffer = buffers[0].buffer;
			bufferSize = buffers[0].bufferSize;

			if(MS_ADPCM_decode((uint8 **)&buffer, &bufferSize) < 0)
				mSystem.Error("idSoundSample_OpenAL::CreateOpenALBuffer: could not decode ADPCM '%s' to 16 bit format", GetName());

			buffers[0].buffer = buffer;
			buffers[0].bufferSize = bufferSize;

			totalBufferSize = bufferSize;
		}
		else if(format.basic.formatTag == SbWaveFile::FORMAT_XMA2)
		{
			// RB: not used in the PC version of the BFG edition
			mSystem.Error("idSoundSample_OpenAL::CreateOpenALBuffer: could not decode XMA2 '%s' to 16 bit format", GetName());
		}
		else if(format.basic.formatTag == SbWaveFile::FORMAT_EXTENSIBLE)
		{
			// RB: not used in the PC version of the BFG edition
			mSystem.Error("idSoundSample_OpenAL::CreateOpenALBuffer: could not decode extensible WAV format '%s' to 16 bit format", GetName());
		}
		else
		{
			// TODO concatenate buffers

			assert(buffers.Num() == 1);

			buffer = buffers[0].buffer;
			bufferSize = buffers[0].bufferSize;
		};

#if 0 //#if defined(AL_SOFT_buffer_samples)
		if( alIsExtensionPresent( "AL_SOFT_buffer_samples" ) )
		{
			ALenum type = AL_SHORT_SOFT;
			
			if( format.basic.bitsPerSample != 16 )
			{
				//mSystem.Error( "idSoundSample_OpenAL::LoadResource: '%s' not a 16 bit format", GetName() );
			};
			
			ALenum channels = NumChannels() == 1 ? AL_MONO_SOFT : AL_STEREO_SOFT;
			ALenum alFormat = GetOpenALSoftFormat( channels, type );
			
			alBufferSamplesSOFT( mnOpenALBuffer, format.basic.samplesPerSec, alFormat, BytesToFrames( bufferSize, channels, type ), channels, type, buffer );
		}
		else
#endif
		{
			alBufferData(mnOpenALBuffer, GetOpenALBufferFormat(), buffer, bufferSize, format.basic.samplesPerSec);
		};

		if(CheckALErrors() != AL_NO_ERROR)
			mSystem.Error("idSoundSample_OpenAL::CreateOpenALBuffer: error loading data into OpenAL hardware buffer");
	};
};

/*
========================
idSoundSample_OpenAL::FreeData

Called before deleting the object and at the start of LoadResource()
========================
*/
void SbSoundSampleOpenAL::FreeData()
{
	if(buffers.Num() > 0)
	{
		soundSystemLocal.StopVoicesWithSample((SbSoundSample *)this);
		for(int i = 0; i < buffers.Num(); i++)
			FreeBuffer(buffers[i].buffer);

		buffers.Clear();
	};
	amplitude.Clear();

	timestamp = FILE_NOT_FOUND_TIMESTAMP;
	memset(&format, 0, sizeof(format));
	loaded = false;
	totalBufferSize = 0;
	playBegin = 0;
	playLength = 0;

	if(alIsBuffer(mnOpenALBuffer))
	{
		CheckALErrors();

		alDeleteBuffers(1, &mnOpenALBuffer);
		if(CheckALErrors() != AL_NO_ERROR)
			mSystem.Error("idSoundSample_OpenAL::FreeData: error unloading data from OpenAL hardware buffer");
		else
			mnOpenALBuffer = 0;
	};
};

}; // namespace sbe::SbSound