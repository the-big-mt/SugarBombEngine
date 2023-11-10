/*
*******************************************************************************

Copyright (C) 1993-2012 id Software LLC, a ZeniMax Media company.
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

//*****************************************************************************

#include <AL/al.h>
#include <AL/alc.h>
#include <AL/alext.h>

#include "SbSoundHardwareOpenAL.hpp"
#include "SbSoundVoiceOpenAL.hpp"

#include "CoreLibs/SbSystem/ISystem.hpp"

//*****************************************************************************

namespace sbe::SbSound
{

SbSoundHardwareOpenAL::SbSoundHardwareOpenAL(ISystem &aSystem) : mSystem(aSystem){}

/*
========================
idSoundHardware_OpenAL::Init
========================
*/
void SbSoundHardwareOpenAL::Init()
{
	//mpCmdSystem->AddCommand("listDevices", listDevices_f, 0, "Lists the connected sound devices", nullptr); // TODO
	
	mSystem.Printf("Setup OpenAL device and context...");
	
	openalDevice = alcOpenDevice(nullptr);
	
	if(openalDevice == nullptr)
	{
		mSystem.FatalError("idSoundHardware_OpenAL::Init: alcOpenDevice() failed\n");
		return;
	};
	
	openalContext = alcCreateContext(openalDevice, nullptr);
	
	if(alcMakeContextCurrent(openalContext) == 0)
	{
		mSystem.FatalError("idSoundHardware_OpenAL::Init: alcMakeContextCurrent( %p) failed\n", openalContext);
		return;
	};
	
	mSystem.Printf("Done.\n");
	
	mSystem.Printf( "OpenAL vendor: %s\n", alGetString( AL_VENDOR ) );
	mSystem.Printf( "OpenAL renderer: %s\n", alGetString( AL_RENDERER ) );
	mSystem.Printf( "OpenAL version: %s\n", alGetString( AL_VERSION ) );
	mSystem.Printf( "OpenAL extensions: %s\n", alGetString( AL_EXTENSIONS ) );
	
	//pMasterVoice->SetVolume( DBtoLinear( s_volume_dB.GetFloat() ) );
	
	//outputChannels = deviceDetails.OutputFormat.Format.nChannels;
	//channelMask = deviceDetails.OutputFormat.dwChannelMask;
	
	//idSoundVoice::InitSurround( outputChannels, channelMask );
	
	// ---------------------
	// Create VU Meter Effect
	// ---------------------
	/*
	IUnknown* vuMeter = nullptr;
	XAudio2CreateVolumeMeter( &vuMeter, 0 );
	
	XAUDIO2_EFFECT_DESCRIPTOR descriptor;
	descriptor.InitialState = true;
	descriptor.OutputChannels = outputChannels;
	descriptor.pEffect = vuMeter;
	
	XAUDIO2_EFFECT_CHAIN chain;
	chain.EffectCount = 1;
	chain.pEffectDescriptors = &descriptor;
	
	pMasterVoice->SetEffectChain( &chain );
	
	vuMeter->Release();
	*/
	
	// ---------------------
	// Create VU Meter Graph
	// ---------------------
	
	/*
	vuMeterRMS = console->CreateGraph( outputChannels );
	vuMeterPeak = console->CreateGraph( outputChannels );
	vuMeterRMS->Enable( false );
	vuMeterPeak->Enable( false );
	
	memset( vuMeterPeakTimes, 0, sizeof( vuMeterPeakTimes ) );
	
	vuMeterPeak->SetFillMode( idDebugGraph::GRAPH_LINE );
	vuMeterPeak->SetBackgroundColor( idVec4( 0.0f, 0.0f, 0.0f, 0.0f ) );
	
	vuMeterRMS->AddGridLine( 0.500f, idVec4( 0.5f, 0.5f, 0.5f, 1.0f ) );
	vuMeterRMS->AddGridLine( 0.250f, idVec4( 0.5f, 0.5f, 0.5f, 1.0f ) );
	vuMeterRMS->AddGridLine( 0.125f, idVec4( 0.5f, 0.5f, 0.5f, 1.0f ) );
	
	const char* channelNames[] = { "L", "R", "C", "S", "Lb", "Rb", "Lf", "Rf", "Cb", "Ls", "Rs" };
	for( int i = 0, ci = 0; ci < sizeof( channelNames ) / sizeof( channelNames[0] ); ci++ )
	{
		if( ( channelMask & BIT( ci ) ) == 0 )
			continue;

		vuMeterRMS->SetLabel( i, channelNames[ ci ] );
		i++;
	};
	*/
};

/*
========================
idSoundHardware_OpenAL::Shutdown
========================
*/
void SbSoundHardwareOpenAL::Shutdown()
{
	// TODO
	/*
	for( int i = 0; i < voices.Num(); i++ )
		voices[ i ].DestroyInternal();

	voices.Clear();
	freeVoices.Clear();
	zombieVoices.Clear();
	*/
	
	alcMakeContextCurrent(nullptr);
	
	alcDestroyContext(openalContext);
	openalContext = nullptr;
	
	alcCloseDevice(openalDevice);
	openalDevice = nullptr;
	
	/*
	if( vuMeterRMS != nullptr )
	{
		console->DestroyGraph( vuMeterRMS );
		vuMeterRMS = nullptr;
	};
	if( vuMeterPeak != nullptr )
	{
		console->DestroyGraph( vuMeterPeak );
		vuMeterPeak = nullptr;
	};
	*/
};

/*
========================
idSoundHardware_OpenAL::Update
========================
*/
void SbSoundHardwareOpenAL::Update()
{
};

/*
========================
idSoundHardware_OpenAL::AllocateVoice
========================
*/
SbSoundVoice *SbSoundHardwareOpenAL::AllocateVoice(const SbSoundSample *apLeadinSample, const SbSoundSample *apLoopingSample)
{
	if(apLeadinSample == nullptr)
		return nullptr;
	
	return new SbSoundVoiceOpenAL();
};

/*
========================
idSoundHardware_OpenAL::FreeVoice
========================
*/
void SbSoundHardwareOpenAL::FreeVoice(SbSoundVoice *apVoice)
{
	//voice->Stop(); // TODO
	
	// Stop() is asyncronous, so we won't flush bufferes until the
	// voice on the zombie channel actually returns !IsPlaying()
	//zombieVoices.Append( voice ); // TODO
	
	if(apVoice)
	{
		delete apVoice;
		apVoice = nullptr;
	};
};

}; // namespace sbe::SbSound