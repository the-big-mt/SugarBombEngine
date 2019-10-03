/*
*******************************************************************************

Copyright (C) 1993-2012 id Software LLC, a ZeniMax Media company.
Copyright (C) 2013 Robert Beckebans
Copyright (c) 2010 by Chris Robinson <chris.kcat@gmail.com> (OpenAL Info Utility)
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

/// @file

#pragma hdrstop

#include "precompiled.h"

#include "framework/CVar.hpp" // TODO: #include "framework/ICVarSystem.hpp"?
#include "framework/ICmdSystem.hpp"

#include "idlib/Lib.h"
#include "idlib/containers/StaticList.h"

#include "SbAL_SoundHardware.hpp"
#include "SbAL_SoundSample.hpp"
#include "SbAL_SoundVoice.hpp"
#include "SbWaveFile.hpp"
#include "snd_local.h"
//#include "SbSoundDefines.hpp"

#include "SbSound/SbSound.h"

//#include "CoreLibs/SbSystem/ISystem.hpp"

namespace sbe::SbSound
{

//class idCmdArgs;

idCVar s_showLevelMeter( "s_showLevelMeter", "0", CVAR_BOOL | CVAR_ARCHIVE, "Show VU meter" );
idCVar s_meterTopTime( "s_meterTopTime", "1000", CVAR_INTEGER | CVAR_ARCHIVE, "How long (in milliseconds) peaks are displayed on the VU meter" );
idCVar s_meterPosition( "s_meterPosition", "100 100 20 200", CVAR_ARCHIVE, "VU meter location (x y w h)" );
idCVar s_device( "s_device", "-1", CVAR_INTEGER | CVAR_ARCHIVE, "Which audio device to use (listDevices to list, -1 for default)" );
idCVar s_showPerfData( "s_showPerfData", "0", CVAR_BOOL, "Show XAudio2 Performance data" );

extern idCVar s_volume_dB;

ID_INLINE_EXTERN ALCenum CheckALCErrors_( ALCdevice* device, const char* filename, int linenum )
{
	ALCenum err = alcGetError( device );
	if( err != ALC_NO_ERROR )
	{
		idLib::Printf( "ALC Error: %s (0x%x), @ %s %d\n", alcGetString( device, err ), err, filename, linenum );
	}
	return err;
}
#define CheckALCErrors(x) CheckALCErrors_((x), __FILE__, __LINE__)

/*
========================
idSoundHardware_OpenAL::idSoundHardware_OpenAL
========================
*/
idSoundHardware_OpenAL::idSoundHardware_OpenAL(sbe::ISoundSystem *apSoundSystem, sbe::ISys *apSys, sbe::idCmdSystem *apCmdSystem)
	: mpSoundSystem(apSoundSystem), mpSys(apSys), mpCmdSystem(apCmdSystem)
{
	openalDevice = nullptr;
	openalContext = nullptr;
	
	//vuMeterRMS = nullptr;
	//vuMeterPeak = nullptr;
	
	//outputChannels = 0;
	//channelMask = 0;
	
	voices.SetNum( 0 );
	zombieVoices.SetNum( 0 );
	freeVoices.SetNum( 0 );
	
	lastResetTime = 0;
}

void idSoundHardware_OpenAL::PrintDeviceList( const char* list )
{
	if( !list || *list == '\0' )
	{
		idLib::Printf( "    !!! none !!!\n" );
	}
	else
	{
		do
		{
			idLib::Printf( "    %s\n", list );
			list += strlen( list ) + 1;
		}
		while( *list != '\0' );
	}
}

void idSoundHardware_OpenAL::PrintALCInfo( ALCdevice* device )
{
	ALCint major, minor;
	
	if( device )
	{
		const ALCchar* devname = nullptr;
		idLib::Printf( "\n" );
		if( alcIsExtensionPresent( device, "ALC_ENUMERATE_ALL_EXT" ) != AL_FALSE )
		{
			devname = alcGetString( device, ALC_ALL_DEVICES_SPECIFIER );
		}
		
		if( CheckALCErrors( device ) != ALC_NO_ERROR || !devname )
		{
			devname = alcGetString( device, ALC_DEVICE_SPECIFIER );
		}
		
		idLib::Printf( "** Info for device \"%s\" **\n", devname );
	}
	alcGetIntegerv( device, ALC_MAJOR_VERSION, 1, &major );
	alcGetIntegerv( device, ALC_MINOR_VERSION, 1, &minor );
	
	if( CheckALCErrors( device ) == ALC_NO_ERROR )
		idLib::Printf( "ALC version: %d.%d\n", major, minor );
		
	if( device )
	{
		idLib::Printf( "OpenAL extensions: %s", alGetString( AL_EXTENSIONS ) );
		
		//idLib::Printf("ALC extensions:");
		//printList(alcGetString(device, ALC_EXTENSIONS), ' ');
		CheckALCErrors( device );
	}
}

void idSoundHardware_OpenAL::PrintALInfo()
{
	idLib::Printf( "OpenAL vendor string: %s\n", alGetString( AL_VENDOR ) );
	idLib::Printf( "OpenAL renderer string: %s\n", alGetString( AL_RENDERER ) );
	idLib::Printf( "OpenAL version string: %s\n", alGetString( AL_VERSION ) );
	idLib::Printf( "OpenAL extensions: %s", alGetString( AL_EXTENSIONS ) );
	//PrintList(alGetString(AL_EXTENSIONS), ' ');
	CheckALErrors();
}

//void idSoundHardware_OpenAL::listDevices_f( const idCmdArgs& args )
void listDevices_f( const idCmdArgs& args )
{
	idLib::Printf( "Available playback devices:\n" );
	if( alcIsExtensionPresent( nullptr, "ALC_ENUMERATE_ALL_EXT" ) != AL_FALSE )
	{
		idSoundHardware_OpenAL::PrintDeviceList( alcGetString( nullptr, ALC_ALL_DEVICES_SPECIFIER ) );
	}
	else
	{
		idSoundHardware_OpenAL::PrintDeviceList( alcGetString( nullptr, ALC_DEVICE_SPECIFIER ) );
	}
	
	//idLib::Printf("Available capture devices:\n");
	//printDeviceList(alcGetString(nullptr, ALC_CAPTURE_DEVICE_SPECIFIER));
	
	if( alcIsExtensionPresent( nullptr, "ALC_ENUMERATE_ALL_EXT" ) != AL_FALSE )
	{
		idLib::Printf( "Default playback device: %s\n", alcGetString( nullptr, ALC_DEFAULT_ALL_DEVICES_SPECIFIER ) );
	}
	else
	{
		idLib::Printf( "Default playback device: %s\n",  alcGetString( nullptr, ALC_DEFAULT_DEVICE_SPECIFIER ) );
	}
	
	//idLib::Printf("Default capture device: %s\n", alcGetString(nullptr, ALC_CAPTURE_DEFAULT_DEVICE_SPECIFIER));
	
	idSoundHardware_OpenAL::PrintALCInfo( nullptr );
	
	idSoundHardware_OpenAL::PrintALCInfo( ( ALCdevice* )mpSoundSystem->GetOpenALDevice() );
}

/*
========================
idSoundHardware_OpenAL::Init
========================
*/
void idSoundHardware_OpenAL::Init()
{
	mpCmdSystem->AddCommand( "listDevices", listDevices_f, 0, "Lists the connected sound devices", nullptr );
	
	mpSys->Printf( "Setup OpenAL device and context... " );
	
	openalDevice = alcOpenDevice( nullptr );
	if( openalDevice == nullptr )
	{
		mpSys->FatalError( "idSoundHardware_OpenAL::Init: alcOpenDevice() failed\n" );
		return;
	}
	
	openalContext = alcCreateContext( openalDevice, nullptr );
	if( alcMakeContextCurrent( openalContext ) == 0 )
	{
		mpSys->FatalError( "idSoundHardware_OpenAL::Init: alcMakeContextCurrent( %p) failed\n", openalContext );
		return;
	}
	
	mpSys->Printf( "Done.\n" );
	
	mpSys->Printf( "OpenAL vendor: %s\n", alGetString( AL_VENDOR ) );
	mpSys->Printf( "OpenAL renderer: %s\n", alGetString( AL_RENDERER ) );
	mpSys->Printf( "OpenAL version: %s\n", alGetString( AL_VERSION ) );
	mpSys->Printf( "OpenAL extensions: %s\n", alGetString( AL_EXTENSIONS ) );
	
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
		{
			continue;
		}
		vuMeterRMS->SetLabel( i, channelNames[ ci ] );
		i++;
	}
	*/
	
	// OpenAL doesn't really impose a maximum number of sources
	voices.SetNum( voices.Max() );
	freeVoices.SetNum( voices.Max() );
	zombieVoices.SetNum( 0 );
	for( int i = 0; i < voices.Num(); i++ )
	{
		freeVoices[i] = &voices[i];
	}
}

/*
========================
idSoundHardware_OpenAL::Shutdown
========================
*/
void idSoundHardware_OpenAL::Shutdown()
{
	for( int i = 0; i < voices.Num(); i++ )
	{
		voices[ i ].DestroyInternal();
	}
	voices.Clear();
	freeVoices.Clear();
	zombieVoices.Clear();
	
	alcMakeContextCurrent( nullptr );
	
	alcDestroyContext( openalContext );
	openalContext = nullptr;
	
	alcCloseDevice( openalDevice );
	openalDevice = nullptr;
	
	/*
	if( vuMeterRMS != nullptr )
	{
		console->DestroyGraph( vuMeterRMS );
		vuMeterRMS = nullptr;
	}
	if( vuMeterPeak != nullptr )
	{
		console->DestroyGraph( vuMeterPeak );
		vuMeterPeak = nullptr;
	}
	*/
}

/*
========================
idSoundHardware_OpenAL::AllocateVoice
========================
*/
idSoundVoice* idSoundHardware_OpenAL::AllocateVoice( const idSoundSample* leadinSample, const idSoundSample* loopingSample )
{
	if( leadinSample == nullptr )
	{
		return nullptr;
	}
	if( loopingSample != nullptr )
	{
		if( ( leadinSample->format.basic.formatTag != loopingSample->format.basic.formatTag ) || ( leadinSample->format.basic.numChannels != loopingSample->format.basic.numChannels ) )
		{
			mpSys->Warning( "Leadin/looping format mismatch: %s & %s", leadinSample->GetName(), loopingSample->GetName() );
			loopingSample = nullptr;
		}
	}
	
	// Try to find a free voice that matches the format
	// But fallback to the last free voice if none match the format
	idSoundVoice* voice = nullptr;
	for( int i = 0; i < freeVoices.Num(); i++ )
	{
		if( freeVoices[i]->IsPlaying() )
		{
			continue;
		}
		voice = ( idSoundVoice* )freeVoices[i];
		if( voice->CompatibleFormat( ( idSoundSample_OpenAL* )leadinSample ) )
		{
			break;
		}
	}
	if( voice != nullptr )
	{
		voice->Create( leadinSample, loopingSample );
		freeVoices.Remove( voice );
		return voice;
	}
	
	return nullptr;
}

/*
========================
idSoundHardware_OpenAL::FreeVoice
========================
*/
void idSoundHardware_OpenAL::FreeVoice( idSoundVoice* voice )
{
	voice->Stop();
	
	// Stop() is asyncronous, so we won't flush bufferes until the
	// voice on the zombie channel actually returns !IsPlaying()
	zombieVoices.Append( voice );
}

/*
========================
idSoundHardware_OpenAL::Update
========================
*/
void idSoundHardware_OpenAL::Update()
{
	if( openalDevice == nullptr )
	{
		int nowTime = Sys_Milliseconds();
		if( lastResetTime + 1000 < nowTime )
		{
			lastResetTime = nowTime;
			Init();
		}
		return;
	}
	
	if( mpSoundSystem->IsMuted() )
	{
		alListenerf( AL_GAIN, 0.0f );
	}
	else
	{
		alListenerf( AL_GAIN, DBtoLinear( s_volume_dB.GetFloat() ) );
	}
	
	// IXAudio2SourceVoice::Stop() has been called for every sound on the
	// zombie list, but it is documented as asyncronous, so we have to wait
	// until it actually reports that it is no longer playing.
	for( int i = 0; i < zombieVoices.Num(); i++ )
	{
		zombieVoices[i]->FlushSourceBuffers();
		if( !zombieVoices[i]->IsPlaying() )
		{
			freeVoices.Append( zombieVoices[i] );
			zombieVoices.RemoveIndexFast( i );
			i--;
		}
		else
		{
			static int playingZombies;
			playingZombies++;
		}
	}
	
	/*
	if( s_showPerfData.GetBool() )
	{
		XAUDIO2_PERFORMANCE_DATA perfData;
		pXAudio2->GetPerformanceData( &perfData );
		idLib::Printf( "Voices: %d/%d CPU: %.2f%% Mem: %dkb\n", perfData.ActiveSourceVoiceCount, perfData.TotalSourceVoiceCount, perfData.AudioCyclesSinceLastQuery / ( float )perfData.TotalCyclesSinceLastQuery, perfData.MemoryUsageInBytes / 1024 );
	}
	*/
	
	/*
	if( vuMeterRMS == nullptr )
	{
		// Init probably hasn't been called yet
		return;
	}
	
	vuMeterRMS->Enable( s_showLevelMeter.GetBool() );
	vuMeterPeak->Enable( s_showLevelMeter.GetBool() );
	
	if( !s_showLevelMeter.GetBool() )
	{
		pMasterVoice->DisableEffect( 0 );
		return;
	}
	else
	{
		pMasterVoice->EnableEffect( 0 );
	}
	
	float peakLevels[ 8 ];
	float rmsLevels[ 8 ];
	
	XAUDIO2FX_VOLUMEMETER_LEVELS levels;
	levels.ChannelCount = outputChannels;
	levels.pPeakLevels = peakLevels;
	levels.pRMSLevels = rmsLevels;
	
	if( levels.ChannelCount > 8 )
	{
		levels.ChannelCount = 8;
	}
	
	pMasterVoice->GetEffectParameters( 0, &levels, sizeof( levels ) );
	
	int currentTime = Sys_Milliseconds();
	for( int i = 0; i < outputChannels; i++ )
	{
		if( vuMeterPeakTimes[i] < currentTime )
		{
			vuMeterPeak->SetValue( i, vuMeterPeak->GetValue( i ) * 0.9f, colorRed );
		}
	}
	
	float width = 20.0f;
	float height = 200.0f;
	float left = 100.0f;
	float top = 100.0f;
	
	sscanf( s_meterPosition.GetString(), "%f %f %f %f", &left, &top, &width, &height );
	
	vuMeterRMS->SetPosition( left, top, width * levels.ChannelCount, height );
	vuMeterPeak->SetPosition( left, top, width * levels.ChannelCount, height );
	
	for( uint32 i = 0; i < levels.ChannelCount; i++ )
	{
		vuMeterRMS->SetValue( i, rmsLevels[ i ], idVec4( 0.5f, 1.0f, 0.0f, 1.00f ) );
		if( peakLevels[ i ] >= vuMeterPeak->GetValue( i ) )
		{
			vuMeterPeak->SetValue( i, peakLevels[ i ], colorRed );
			vuMeterPeakTimes[i] = currentTime + s_meterTopTime.GetInteger();
		}
	}
	*/
}

}; // namespace sbe::SbSound