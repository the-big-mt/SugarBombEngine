/*
*******************************************************************************

Copyright (C) 1993-2012 id Software LLC, a ZeniMax Media company.
Copyright (C) 2013 Robert Beckebans
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

//#pragma hdrstop
//#include <cstddef>
//#include "precompiled.h"

#include "SbSoundVoiceOpenAL.hpp"

#include "CoreLibs/SbSystem/ISystem.hpp"

//*****************************************************************************

namespace sbe::SbSound
{

// The whole system runs at this sample rate
static int SYSTEM_SAMPLE_RATE{44100};
//static float ONE_OVER_SYSTEM_SAMPLE_RATE = 1.0f / SYSTEM_SAMPLE_RATE; // TODO: unused?

/*
========================
idSoundVoice_OpenAL::idSoundVoice_OpenAL
========================
*/
SbSoundVoiceOpenAL::SbSoundVoiceOpenAL(ISystem &aSystem)
	: mSystem(aSystem),
	  triggered(false),
      openalSource(0),
      formatTag(0),
      numChannels(0),
      sampleRate(0),
      hasVUMeter(false){}

/*
========================
idSoundVoice_OpenAL::~idSoundVoice_OpenAL
========================
*/
SbSoundVoiceOpenAL::~SbSoundVoiceOpenAL()
{
	DestroyInternal();
};

/*
========================
idSoundVoice_OpenAL::Create
========================
*/
void SbSoundVoiceOpenAL::Create(const SbSoundSample *leadinSample_, const SbSoundSample *loopingSample_)
{
	if(IsPlaying())
	{
		// This should never hit
		Stop();
		return;
	};

	triggered = true;

	leadinSample = (SbSoundSample_OpenAL *)leadinSample_;
	loopingSample = (SbSoundSample_OpenAL *)loopingSample_;

	if(alIsSource(openalSource) && CompatibleFormat(leadinSample))
		sampleRate = leadinSample->format.basic.samplesPerSec;
	else
	{
		DestroyInternal();
		formatTag = leadinSample->format.basic.formatTag;
		numChannels = leadinSample->format.basic.numChannels;
		sampleRate = leadinSample->format.basic.samplesPerSec;

		//soundSystemLocal.hardware.pXAudio2->CreateSourceVoice( &pSourceVoice, ( const WAVEFORMATEX* )&leadinSample->format, XAUDIO2_VOICE_USEFILTER, 4.0f, &streamContext );

		CheckALErrors();

		alGenSources(1, &openalSource);
		if(CheckALErrors() != AL_NO_ERROR)
		//if( pSourceVoice == nullptr )
		{
			// If this hits, then we are most likely passing an invalid sample format, which should have been caught by the loader (and the sample defaulted)
			return;
		};

		alSourcef(openalSource, AL_ROLLOFF_FACTOR, 0.0f);

		//if( ( loopingSample == nullptr && leadinSample->openalBuffer != 0 ) || ( loopingSample != nullptr && soundShader->entries[0]->hardwareBuffer ) )
		if(leadinSample->openalBuffer != 0)
		{
			alSourcei(openalSource, AL_BUFFER, 0);

			// handle uncompressed (non streaming) single shot and looping sounds
			/*
			if( triggered )
			{
				alSourcei( openalSource, AL_BUFFER, looping ? chan->soundShader->entries[0]->openalBuffer : leadinSample->openalBuffer );
			}
			*/
		}
		else
		{
			//if( triggered )

			// handle streaming sounds (decode on the fly) both single shot AND looping

			alSourcei(openalSource, AL_BUFFER, 0);
			alDeleteBuffers(3, &lastopenalStreamingBuffer[0]);
			lastopenalStreamingBuffer[0] = openalStreamingBuffer[0];
			lastopenalStreamingBuffer[1] = openalStreamingBuffer[1];
			lastopenalStreamingBuffer[2] = openalStreamingBuffer[2];

			alGenBuffers(3, &openalStreamingBuffer[0]);
			/*
			if( soundSystemLocal.alEAXSetBufferMode )
			{
				soundSystemLocal.alEAXSetBufferMode( 3, &chan->openalStreamingBuffer[0], alGetEnumValue( ID_ALCHAR "AL_STORAGE_ACCESSIBLE" ) );
			}
			*/
			openalStreamingBuffer[0];
			openalStreamingBuffer[1];
			openalStreamingBuffer[2];
		};

		if(s_debugHardware.GetBool())
		{
			if(loopingSample == nullptr || loopingSample == leadinSample)
				idLib::Printf("%dms: %i created for %s\n", Sys_Milliseconds(), openalSource, leadinSample ? leadinSample->GetName() : "<null>"); // TODO
			else
				idLib::Printf("%dms: %i created for %s and %s\n", Sys_Milliseconds(), openalSource, leadinSample ? leadinSample->GetName() : "<null>", loopingSample ? loopingSample->GetName() : "<null>"); // TODO
		};
	};

	sourceVoiceRate = sampleRate;
	//pSourceVoice->SetSourceSampleRate( sampleRate );
	//pSourceVoice->SetVolume( 0.0f );

	alSourcei(openalSource, AL_SOURCE_RELATIVE, AL_TRUE);
	alSource3f(openalSource, AL_POSITION, 0.0f, 0.0f, 0.0f);

	// RB: FIXME 0.0f ?
	alSourcef(openalSource, AL_GAIN, 1.0f);

	//OnBufferStart( leadinSample, 0 );
};

/*
========================
idSoundVoice_OpenAL::Start
========================
*/
void SbSoundVoiceOpenAL::Start(int anOffsetMS, int anFlags)
{
	if(s_debugHardware.GetBool())
		mSystem.Printf("%dms: %i starting %s @ %dms\n", Sys_Milliseconds(), openalSource, leadinSample ? leadinSample->GetName() : "<null>", anOffsetMS);

	if(!leadinSample)
		return;

	if(!alIsSource(openalSource))
		return;

	if(leadinSample->IsDefault())
		mSystem.Warning("Starting defaulted sound sample %s", leadinSample->GetName());

	bool flicker = (anFlags & SSF_NO_FLICKER) == 0;

	if(flicker != hasVUMeter)
	{
		hasVUMeter = flicker;

		/*
		if( flicker )
		{
			IUnknown* vuMeter = nullptr;
		
			if( XAudio2CreateVolumeMeter( &vuMeter, 0 ) == S_OK )
			{
		
				XAUDIO2_EFFECT_DESCRIPTOR descriptor;
				descriptor.InitialState = true;
				descriptor.OutputChannels = leadinSample->NumChannels();
				descriptor.pEffect = vuMeter;
		
				XAUDIO2_EFFECT_CHAIN chain;
				chain.EffectCount = 1;
				chain.pEffectDescriptors = &descriptor;
		
				pSourceVoice->SetEffectChain( &chain );
		
				vuMeter->Release();
			}
		}
		else
		{
			pSourceVoice->SetEffectChain( nullptr );
		}
		*/
	};

	assert(anOffsetMS >= 0);
	int offsetSamples = MsecToSamples(anOffsetMS, leadinSample->SampleRate());
	if(loopingSample == nullptr && offsetSamples >= leadinSample->playLength)
		return;

	RestartAt(offsetSamples);
	Update();
	UnPause();
};

/*
========================
idSoundVoice_OpenAL::Stop
========================
*/
void SbSoundVoiceOpenAL::Stop()
{
	if(!alIsSource(openalSource))
		return;

	if(!paused)
	{
		if(s_debugHardware.GetBool())
			mSystem.Printf("%dms: %i stopping %s\n", Sys_Milliseconds(), openalSource, leadinSample ? leadinSample->GetName() : "<null>");

		alSourceStop(openalSource);
		alSourcei(openalSource, AL_BUFFER, 0);

		//pSourceVoice->Stop( 0, OPERATION_SET );
		paused = true;
	};
};

/*
========================
idSoundVoice_OpenAL::Pause
========================
*/
void SbSoundVoiceOpenAL::Pause()
{
	if(!alIsSource(openalSource) || paused)
		return;

	if(s_debugHardware.GetBool())
		mSystem.Printf("%dms: %i pausing %s\n", Sys_Milliseconds(), openalSource, leadinSample ? leadinSample->GetName() : "<null>");

	alSourcePause(openalSource);
	//pSourceVoice->Stop( 0, OPERATION_SET );
	paused = true;
};

/*
========================
idSoundVoice_OpenAL::UnPause
========================
*/
void SbSoundVoiceOpenAL::UnPause()
{
	if(!alIsSource(openalSource) || !paused)
		return;

	if(s_debugHardware.GetBool())
		mSystem.Printf("%dms: %i unpausing %s\n", Sys_Milliseconds(), openalSource, leadinSample ? leadinSample->GetName() : "<null>");

	alSourcePlay(openalSource);
	//pSourceVoice->Start( 0, OPERATION_SET );
	paused = false;
};

/*
========================
idSoundVoice_OpenAL::Update
========================
*/
bool SbSoundVoiceOpenAL::Update()
{
	/*
	if( pSourceVoice == nullptr || leadinSample == nullptr )
	{
		return false;
	}
	
	XAUDIO2_VOICE_STATE state;
	pSourceVoice->GetState( &state );
	
	const int srcChannels = leadinSample->NumChannels();
	
	float pLevelMatrix[ MAX_CHANNELS_PER_VOICE * MAX_CHANNELS_PER_VOICE ] = { 0 };
	CalculateSurround( srcChannels, pLevelMatrix, 1.0f );
	
	if( s_skipHardwareSets.GetBool() )
	{
		return true;
	}
	
	pSourceVoice->SetOutputMatrix( soundSystemLocal.hardware.pMasterVoice, srcChannels, dstChannels, pLevelMatrix, OPERATION_SET );
	
	assert( idMath::Fabs( gain ) <= XAUDIO2_MAX_VOLUME_LEVEL );
	pSourceVoice->SetVolume( gain, OPERATION_SET );
	
	SetSampleRate( sampleRate, OPERATION_SET );
	
	// we don't do this any longer because we pause and unpause explicitly when the soundworld is paused or unpaused
	// UnPause();
	*/
	return true;
};

/*
========================
idSoundVoice_OpenAL::GetAmplitude
========================
*/
float SbSoundVoiceOpenAL::GetAmplitude() const
{
	// TODO
	return 1.0f;

	/*
	if( !hasVUMeter )
	{
		return 1.0f;
	}
	
	float peakLevels[ MAX_CHANNELS_PER_VOICE ];
	float rmsLevels[ MAX_CHANNELS_PER_VOICE ];
	
	XAUDIO2FX_VOLUMEMETER_LEVELS levels;
	levels.ChannelCount = leadinSample->NumChannels();
	levels.pPeakLevels = peakLevels;
	levels.pRMSLevels = rmsLevels;
	
	if( levels.ChannelCount > MAX_CHANNELS_PER_VOICE )
	{
		levels.ChannelCount = MAX_CHANNELS_PER_VOICE;
	}
	
	if( pSourceVoice->GetEffectParameters( 0, &levels, sizeof( levels ) ) != S_OK )
	{
		return 0.0f;
	}
	
	if( levels.ChannelCount == 1 )
	{
		return rmsLevels[0];
	}
	
	float rms = 0.0f;
	for( uint32 i = 0; i < levels.ChannelCount; i++ )
	{
		rms += rmsLevels[i];
	}
	
	return rms / ( float )levels.ChannelCount;
	*/
};

/*
========================
idSoundVoice_OpenAL::CompatibleFormat
========================
*/
bool SbSoundVoiceOpenAL::CompatibleFormat(SbSoundSample *apSample) const
{
	if(alIsSource(openalSource))
	{
		// If this voice has never been allocated, then it's compatible with everything
		return true;
	};

	return false;
};

uint32_t SbSoundVoiceOpenAL::GetSampleRate() const
{
	return 0;
};

/*
========================
idSoundVoice_OpenAL::OnBufferStart
========================
*/
//void SbSoundVoice_OpenAL::OnBufferStart(SbSoundSample_OpenAL *sample, int bufferNumber)
void SbSoundVoiceOpenAL::OnBufferStart(SbSoundSample *apSample, int anBuffer)
{
	//SetSampleRate( sample->SampleRate(), XAUDIO2_COMMIT_NOW );

	SbSoundSample_OpenAL *nextSample = sample;
	int nextBuffer = bufferNumber + 1;
	if(nextBuffer == sample->buffers.Num())
	{
		if(sample == leadinSample)
		{
			if(loopingSample == nullptr)
				return;

			nextSample = loopingSample;
		};
		nextBuffer = 0;
	};

	SubmitBuffer(nextSample, nextBuffer, 0);
};

/*
========================
idSoundVoice_OpenAL::FlushSourceBuffers
========================
*/
void SbSoundVoiceOpenAL::FlushSourceBuffers()
{
	if(alIsSource(openalSource))
	{
		//pSourceVoice->FlushSourceBuffers();
	};
};

/*
========================
idSoundVoice_OpenAL::DestroyInternal
========================
*/
void SbSoundVoiceOpenAL::DestroyInternal()
{
	if(alIsSource(openalSource))
	{
		if(s_debugHardware.GetBool())
			idLib::Printf("%dms: %i destroyed\n", Sys_Milliseconds(), openalSource); // TODO

		alDeleteSources(1, &openalSource);
		openalSource = 0;

		alSourcei(openalSource, AL_BUFFER, 0);

		if(openalStreamingBuffer[0] && openalStreamingBuffer[1] && openalStreamingBuffer[2])
		{
			CheckALErrors();

			alDeleteBuffers(3, &openalStreamingBuffer[0]);
			if(CheckALErrors() == AL_NO_ERROR)
				openalStreamingBuffer[0] = openalStreamingBuffer[1] = openalStreamingBuffer[2] = 0;
		};

		if(lastopenalStreamingBuffer[0] && lastopenalStreamingBuffer[1] && lastopenalStreamingBuffer[2])
		{
			CheckALErrors();

			alDeleteBuffers(3, &lastopenalStreamingBuffer[0]);
			if(CheckALErrors() == AL_NO_ERROR)
				lastopenalStreamingBuffer[0] = lastopenalStreamingBuffer[1] = lastopenalStreamingBuffer[2] = 0;
		};

		openalStreamingOffset = 0;

		hasVUMeter = false;
	};
};

/*
========================
idSoundVoice_OpenAL::RestartAt
========================
*/
int SbSoundVoiceOpenAL::RestartAt(int offsetSamples)
{
	offsetSamples &= ~127;

	SbSoundSample_OpenAL *sample = leadinSample;
	if(offsetSamples >= leadinSample->playLength)
	{
		if(loopingSample != nullptr)
		{
			offsetSamples %= loopingSample->playLength;
			sample = loopingSample;
		}
		else
			return 0;
	};

	int previousNumSamples = 0;
	for(int i = 0; i < sample->buffers.Num(); i++)
	{
		if(sample->buffers[i].numSamples > sample->playBegin + offsetSamples)
			return SubmitBuffer(sample, i, sample->playBegin + offsetSamples - previousNumSamples);

		previousNumSamples = sample->buffers[i].numSamples;
	};

	return 0;
};

/*
========================
idSoundVoice_OpenAL::SubmitBuffer
========================
*/
int SbSoundVoiceOpenAL::SubmitBuffer(SbSoundSample_OpenAL *sample, int bufferNumber, int offset)
{
	if(sample == nullptr || (bufferNumber < 0) || (bufferNumber >= sample->buffers.Num()))
		return 0;

#if 0
	SbSoundSystemLocal::bufferContext_t* bufferContext = soundSystemLocal.ObtainStreamBufferContext();
	if( bufferContext == nullptr )
	{
		idLib::Warning( "No free buffer contexts!" ); // TODO
		return 0;
	};
	
	bufferContext->voice = this;
	bufferContext->sample = sample;
	bufferContext->bufferNumber = bufferNumber;
#endif

	if(sample->openalBuffer != 0)
	{
		alSourcei(openalSource, AL_BUFFER, sample->openalBuffer);
		alSourcei(openalSource, AL_LOOPING, (sample == loopingSample && loopingSample != nullptr ? AL_TRUE : AL_FALSE));

		return sample->totalBufferSize;
	}
	else
	{
		ALint finishedbuffers;

		if(!triggered)
		{
			alGetSourcei(openalSource, AL_BUFFERS_PROCESSED, &finishedbuffers);
			alSourceUnqueueBuffers(openalSource, finishedbuffers, &openalStreamingBuffer[0]);
			if(finishedbuffers == 3)
				triggered = true;
		}
		else
			finishedbuffers = 3;

		ALenum format;

		if(sample->format.basic.formatTag == SbWaveFile::FORMAT_PCM)
			format = sample->NumChannels() == 1 ? AL_FORMAT_MONO16 : AL_FORMAT_STEREO16;
		else if(sample->format.basic.formatTag == SbWaveFile::FORMAT_ADPCM)
			format = sample->NumChannels() == 1 ? AL_FORMAT_MONO16 : AL_FORMAT_STEREO16;
		else if(sample->format.basic.formatTag == SbWaveFile::FORMAT_XMA2)
			format = sample->NumChannels() == 1 ? AL_FORMAT_MONO16 : AL_FORMAT_STEREO16;
		else
			format = sample->NumChannels() == 1 ? AL_FORMAT_MONO16 : AL_FORMAT_STEREO16;

		int rate = sample->SampleRate(); /*44100*/

		for(int j = 0; j < finishedbuffers && j < 1; j++)
		{
			/*
			chan->GatherChannelSamples( chan->openalStreamingOffset * sample->objectInfo.nChannels, MIXBUFFER_SAMPLES * sample->objectInfo.nChannels, alignedInputSamples );
			for( int i = 0; i < ( MIXBUFFER_SAMPLES * sample->objectInfo.nChannels ); i++ )
			{
				if( alignedInputSamples[i] < -32768.0f )
					( ( short* )alignedInputSamples )[i] = -32768;
				else if( alignedInputSamples[i] > 32767.0f )
					( ( short* )alignedInputSamples )[i] = 32767;
				else
					( ( short* )alignedInputSamples )[i] = idMath::FtoiFast( alignedInputSamples[i] );
			}
			*/

			//alBufferData( buffers[0], sample->NumChannels() == 1 ? AL_FORMAT_MONO16 : AL_FORMAT_STEREO16, sample->buffers[bufferNumber].buffer, sample->buffers[bufferNumber].bufferSize, sample->SampleRate() /*44100*/ );

			alBufferData(openalStreamingBuffer[j], format, sample->buffers[bufferNumber].buffer, sample->buffers[bufferNumber].bufferSize, rate);
			//openalStreamingOffset += MIXBUFFER_SAMPLES;
		};

		if(finishedbuffers > 0)
		{
			//alSourceQueueBuffers( openalSource, finishedbuffers, &buffers[0] );
			alSourceQueueBuffers(openalSource, 1, &openalStreamingBuffer[0]);

			if(bufferNumber == 0)
			{
				//alSourcePlay( openalSource );
				triggered = false;
			};

			return sample->buffers[bufferNumber].bufferSize;
		};
	};

	// should never happen
	return 0;

	/*
	
	XAUDIO2_BUFFER buffer = { 0 };
	if( offset > 0 )
	{
		int previousNumSamples = 0;
		if( bufferNumber > 0 )
		{
			previousNumSamples = sample->buffers[bufferNumber - 1].numSamples;
		}
		buffer.PlayBegin = offset;
		buffer.PlayLength = sample->buffers[bufferNumber].numSamples - previousNumSamples - offset;
	}
	buffer.AudioBytes = sample->buffers[bufferNumber].bufferSize;
	buffer.pAudioData = ( BYTE* )sample->buffers[bufferNumber].buffer;
	buffer.pContext = bufferContext;
	if( ( loopingSample == nullptr ) && ( bufferNumber == sample->buffers.Num() - 1 ) )
	{
		buffer.Flags = XAUDIO2_END_OF_STREAM;
	}
	pSourceVoice->SubmitSourceBuffer( &buffer );
	
	return buffer.AudioBytes;
	
	*/
};

/*
========================
idSoundVoice_OpenAL::IsPlaying
========================
*/
bool SbSoundVoice_OpenAL::IsPlaying() const
{
	if(!alIsSource(openalSource))
		return false;

	ALint state = AL_INITIAL;

	alGetSourcei(openalSource, AL_SOURCE_STATE, &state);

	return state == AL_PLAYING;

	//XAUDIO2_VOICE_STATE state;
	//pSourceVoice->GetState( &state );

	//return ( state.BuffersQueued != 0 );
};

}; // namespace sbe::SbSound