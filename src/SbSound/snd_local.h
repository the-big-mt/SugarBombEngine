/*
===========================================================================

Doom 3 BFG Edition GPL Source Code
Copyright (C) 1993-2012 id Software LLC, a ZeniMax Media company.
Copyright (C) 2013-2016 Robert Beckebans
Copyright (C) 2014-2016 Kot in Action Creative Artel

This file is part of the Doom 3 BFG Edition GPL Source Code ("Doom 3 BFG Edition Source Code").

Doom 3 BFG Edition Source Code is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

Doom 3 BFG Edition Source Code is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with Doom 3 BFG Edition Source Code.  If not, see <http://www.gnu.org/licenses/>.

In addition, the Doom 3 BFG Edition Source Code is also subject to certain additional terms. You should have received a copy of these additional terms immediately following the terms and conditions of the GNU General Public License which accompanied the Doom 3 BFG Edition Source Code.  If not, please request a copy in writing from id Software at the address below.

If you have questions concerning this license or the applicable additional terms, you may contact in writing id Software LLC, c/o ZeniMax Media Inc., Suite 120, Rockville, Maryland 20850 USA.

===========================================================================
*/

#ifndef __SND_LOCAL_H__
#define __SND_LOCAL_H__

//#include "idlib/Thread.h"

//} // namespace BFG


#if defined(USE_OPENAL)

//#define AL_ALEXT_PROTOTYPES

#ifdef __APPLE__
#include <OpenAL/al.h>
#include <OpenAL/alc.h>
#else
#include <AL/al.h>
#include <AL/alc.h>
#include <AL/alext.h>
#endif

#include "OpenAL/AL_SoundSample.h"
#include "OpenAL/AL_SoundVoice.h"
#include "OpenAL/AL_SoundHardware.h"

//namespace BFG
//{

ID_INLINE_EXTERN ALenum CheckALErrors_( const char* filename, int line )
{
	ALenum err = alGetError();
	if( err != AL_NO_ERROR )
	{
		idLib::Printf( "OpenAL Error: %s (0x%x), @ %s %d\n", alGetString( err ), err, filename, line );
	}
	return err;
}
#define CheckALErrors() CheckALErrors_(__FILE__, __LINE__)

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

//} // namespace BFG

#elif defined(_MSC_VER) // DG: stub out xaudio for MinGW etc

#define OPERATION_SET 1

// RB: not available on Windows 8 SDK
#if defined(USE_WINRT) // (_WIN32_WINNT < 0x0602 /*_WIN32_WINNT_WIN8*/)
#include <mmdeviceapi.h>
#include <initguid.h> // For the pkey defines to be properly instantiated.
#include <propkeydef.h>
#include "functiondiscoverykeys_devpkey.h"
#include <string>
#include <vector>

//namespace BFG
//{

DEFINE_PROPERTYKEY( PKEY_AudioEndpoint_Path, 0x9c119480, 0xddc2, 0x4954, 0xa1, 0x50, 0x5b, 0xd2, 0x40, 0xd4, 0x54, 0xad, 1 );

#pragma comment(lib,"xaudio2.lib")

struct AudioDevice
{
	std::wstring name;
	std::wstring id;
};

//} // namespace BFG

#else
#include <dxsdkver.h>
#endif
// RB end

#include <xaudio2.h>
#include <xaudio2fx.h>
#include <X3DAudio.h>

// RB: not available on Windows 8 SDK
#if !defined(USE_WINRT) // (_WIN32_WINNT < 0x0602 /*_WIN32_WINNT_WIN8*/)
#include <xma2defs.h>
#endif
// RB end

#include "XAudio2/XA2_SoundSample.h"
#include "XAudio2/XA2_SoundVoice.h"
#include "XAudio2/XA2_SoundHardware.h"

#else // not _MSC_VER => MinGW, GCC, ...
// just a stub for now
#include "stub/SoundStub.h"
#endif // _MSC_VER ; DG end

//namespace BFG
//{

#	include "stub/SoundStub.h" // just a stub for now
#endif // _MSC_VER ; DG end

//} // namespace BFG

#endif /* !__SND_LOCAL_H__ */#include "SbSoundFade.hpp"
#include "SbSoundChannel.hpp"
#include "SbSoundWorld.hpp"
#include "SbSoundEmitter.hpp"
#include "SbSoundSystem.hpp"