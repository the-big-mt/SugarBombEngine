/*
*******************************************************************************

Copyright (C) 1993-2012 id Software LLC, a ZeniMax Media company.
Copyright (C) 2012 Daniel Gibson
Copyright (C) 2012 Robert Beckebans
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

#pragma once

//*****************************************************************************

// Include the various platform specific header files (windows.h, etc)

/*
================================================================================================

	Common Include Files

================================================================================================
*/

#if !defined( _DEBUG ) && !defined( NDEBUG )
// don't generate asserts
#define NDEBUG
#endif

#include <cstdio>
#include <cstdlib>
#include <cstdarg>
#include <cstring>
#include <cassert>
#include <ctime>
#include <cctype>
#include <typeinfo>
#include <cerrno>
#include <cmath>
#include <climits>
#include <memory>
// RB: added <cstdint> for missing uintptr_t with MinGW
#include <cstdint>
// RB end
// Yamagi: <cstddef> for ptrdiff_t on FreeBSD
#include <cstddef>
// Yamagi end

/*
================================================================================================

	Windows

================================================================================================
*/

// RB: windows specific stuff should only be set on Windows
#if defined(_WIN32)

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS	// prevent auto literal to string conversion

#ifndef _D3SDK
#ifndef GAME_DLL

#define WINVER				0x501

#include <winsock2.h>
#include <mmsystem.h>
#include <mmreg.h>

#define DIRECTINPUT_VERSION  0x0800			// was 0x0700 with the old mssdk
#define DIRECTSOUND_VERSION  0x0800

#ifdef _MSC_VER
#include <dsound.h>
#else
// DG: MinGW is incompatible with the original dsound.h because it contains MSVC specific annotations
#include <wine-dsound.h>

// RB: was missing in MinGW/include/winuser.h
#ifndef MAPVK_VSC_TO_VK_EX
#define MAPVK_VSC_TO_VK_EX 3
#endif

// RB begin
#if defined(__MINGW32__)
//#include <sal.h> 	// RB: missing __analysis_assume
// including <sal.h> breaks some STL crap ...

#ifndef __analysis_assume
#define __analysis_assume( x )
#endif

#endif
// RB end

#endif



#include <dinput.h>

#endif /* !GAME_DLL */
#endif /* !_D3SDK */

// DG: intrinsics for GCC
#if defined(__GNUC__) && defined(__SSE2__)
#include <emmintrin.h>

// TODO: else: alternative implementations?
#endif
// DG end

#ifdef _MSC_VER
#include <intrin.h>			// needed for intrinsics like _mm_setzero_si28

#pragma warning(disable : 4100)				// unreferenced formal parameter
#pragma warning(disable : 4127)				// conditional expression is constant
#pragma warning(disable : 4244)				// conversion to smaller type, possible loss of data
#pragma warning(disable : 4714)				// function marked as __forceinline not inlined
#pragma warning(disable : 4996)				// unsafe string operations
#endif // _MSC_VER

#include <windows.h>						// for gl.h

#elif defined(__linux__) || defined(__FreeBSD__)

#include <csignal>
#include <pthread.h>

#endif // #if defined(_WIN32)
// RB end

#include <cstdlib>							// no malloc.h on mac or unix
#undef FindText								// fix namespace pollution

//-----------------------------------------------------

//namespace BFG
//{

// Hacked stuff we may want to consider implementing later
class idScopedGlobalHeap
{
};

//} // namespace BFG