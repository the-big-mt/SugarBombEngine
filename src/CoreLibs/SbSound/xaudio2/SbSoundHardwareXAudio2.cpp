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

#include "SbSoundHardwareXAudio2.hpp"
#include "SbSoundVoiceXAudio2.hpp"

//*****************************************************************************

namespace sbe::SbSound
{

/*
========================
idSoundHardware_XAudio2::Init
========================
*/
void SbSoundHardwareXAudio2::Init()
{
	DWORD xAudioCreateFlags = 0;
	
// RB: not available on Windows 8 SDK
#if !defined(SBE_USE_WINRT) && defined(_DEBUG) // (_WIN32_WINNT < 0x0602 /*_WIN32_WINNT_WIN8*/) && defined(_DEBUG)
	xAudioCreateFlags |= XAUDIO2_DEBUG_ENGINE;
#endif
// RB end

	XAUDIO2_PROCESSOR xAudioProcessor = XAUDIO2_DEFAULT_PROCESSOR;
	
// RB: not available on Windows 8 SDK
	if( FAILED( XAudio2Create( &pXAudio2, xAudioCreateFlags, xAudioProcessor ) ) )
	{
#if !defined(SBE_USE_WINRT) && defined(_DEBUG) // (_WIN32_WINNT < 0x0602 /*_WIN32_WINNT_WIN8*/) && defined(_DEBUG)
		if( xAudioCreateFlags & XAUDIO2_DEBUG_ENGINE )
		{
			// in case the debug engine isn't installed
			xAudioCreateFlags &= ~XAUDIO2_DEBUG_ENGINE;
			if( FAILED( XAudio2Create( &pXAudio2, xAudioCreateFlags, xAudioProcessor ) ) )
			{
				idLib::FatalError( "Failed to create XAudio2 engine.  Try installing the latest DirectX." ); // TODO
				return;
			};
		}
		else
#endif
// RB end
		{
			idLib::FatalError( "Failed to create XAudio2 engine.  Try installing the latest DirectX." ); // TODO
			return;
		};
	};
	
#ifdef _DEBUG
	XAUDIO2_DEBUG_CONFIGURATION debugConfiguration = { 0 };
	debugConfiguration.TraceMask = XAUDIO2_LOG_WARNINGS;
	debugConfiguration.BreakMask = XAUDIO2_LOG_ERRORS;
	pXAudio2->SetDebugConfiguration( &debugConfiguration );
#endif

	// Register the sound engine callback
	pXAudio2->RegisterForCallbacks(&soundEngineCallback);
	soundEngineCallback.hardware = this;
};

/*
========================
idSoundHardware_XAudio2::Shutdown
========================
*/
void SbSoundHardwareXAudio2::Shutdown()
{
	for( int i = 0; i < voices.Num(); i++ )
		voices[ i ].DestroyInternal();

	voices.Clear();
	freeVoices.Clear();
	zombieVoices.Clear();
	
	if( pXAudio2 != nullptr )
	{
		// Unregister the sound engine callback
		pXAudio2->UnregisterForCallbacks( &soundEngineCallback );
	};
};

void SbSoundHardwareXAudio2::Update()
{
};

/*
========================
idSoundHardware_XAudio2::AllocateVoice
========================
*/
SbSoundVoice *SbSoundHardwareXAudio2::AllocVoice(const SbSoundSample *apLeadinSample, const SbSoundSample *apLoopingSample)
{
	if(apLeadinSample == nullptr)
		return nullptr;
	
	if(apLoopingSample != nullptr)
	{
		if( ( leadinSample->format.basic.formatTag != apLoopingSample->format.basic.formatTag ) || ( leadinSample->format.basic.numChannels != apLoopingSample->format.basic.numChannels ) )
		{
			idLib::Warning( "Leadin/looping format mismatch: %s & %s", leadinSample->GetName(), apLoopingSample->GetName() ); // TODO
			apLoopingSample = nullptr;
		};
	};
	
	SbSoundVoice *pVoice{nullptr};
	
	pVoice = new SbSoundVoiceXAudio2();
	
	if(pVoice != nullptr)
	{
		pVoice->Create(apLeadinSample, apLoopingSample);
		freeVoices.Remove(pVoice);
	};
	
	return pVoice;
};

void SbSoundHardwareXAudio2::FreeVoice(SbSoundVoice *apVoice)
{
	if(apVoice)
	{
		delete apVoice;
		apVoice = nullptr;
	};
};

}; // namespace sbe::SbSound