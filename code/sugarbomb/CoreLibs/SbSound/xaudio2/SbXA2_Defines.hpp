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

#define OPERATION_SET 1

// RB: not available on Windows 8 SDK
#if defined(USE_WINRT) // (_WIN32_WINNT < 0x0602 /*_WIN32_WINNT_WIN8*/)
#	include <mmdeviceapi.h>
#	include <initguid.h> // For the pkey defines to be properly instantiated.
#	include <propkeydef.h>
#	include "functiondiscoverykeys_devpkey.h"
#	include <string>
#	include <vector>

//namespace sbe
//{

DEFINE_PROPERTYKEY(PKEY_AudioEndpoint_Path, 0x9c119480, 0xddc2, 0x4954, 0xa1, 0x50, 0x5b, 0xd2, 0x40, 0xd4, 0x54, 0xad, 1);

#pragma comment(lib, "xaudio2.lib")

struct AudioDevice
{
	std::wstring name;
	std::wstring id;
};

//} // namespace sbe

#else
#	include <dxsdkver.h>
#endif
// RB end

#include <xaudio2.h>
#include <xaudio2fx.h>
#include <X3DAudio.h>

// RB: not available on Windows 8 SDK
#if !defined(USE_WINRT) // (_WIN32_WINNT < 0x0602 /*_WIN32_WINNT_WIN8*/)
#	include <xma2defs.h>
#endif
// RB end

#include "XA2_SoundSample.h"
#include "XA2_SoundVoice.h"
#include "XA2_SoundHardware.h"