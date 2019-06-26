/*
===========================================================================

Doom 3 BFG Edition GPL Source Code
Copyright (C) 1993-2012 id Software LLC, a ZeniMax Media company.
Copyright (C) 2012 Robert Beckebans
Copyright (C) 2018-2019 BlackPhrase

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

/// @file
/// @brief Non-portable system services

#pragma once


#include "SbInput/SbInputTypes.hpp"

namespace sbe
{

enum cpuid_t
{
	CPUID_NONE							= 0x00000,
	CPUID_UNSUPPORTED					= 0x00001,	// unsupported (386/486)
	CPUID_GENERIC						= 0x00002,	// unrecognized processor
	CPUID_INTEL							= 0x00004,	// Intel
	CPUID_AMD							= 0x00008,	// AMD
	CPUID_MMX							= 0x00010,	// Multi Media Extensions
	CPUID_3DNOW							= 0x00020,	// 3DNow!
	CPUID_SSE							= 0x00040,	// Streaming SIMD Extensions
	CPUID_SSE2							= 0x00080,	// Streaming SIMD Extensions 2
	CPUID_SSE3							= 0x00100,	// Streaming SIMD Extentions 3 aka Prescott's New Instructions
	CPUID_ALTIVEC						= 0x00200,	// AltiVec
	CPUID_HTT							= 0x01000,	// Hyper-Threading Technology
	CPUID_CMOV							= 0x02000,	// Conditional Move (CMOV) and fast floating point comparison (FCOMI) instructions
	CPUID_FTZ							= 0x04000,	// Flush-To-Zero mode (denormal results are flushed to zero)
	CPUID_DAZ							= 0x08000,	// Denormals-Are-Zero mode (denormal source operands are set to zero)
	CPUID_XENON							= 0x10000,	// Xbox 360
	CPUID_CELL							= 0x20000	// PS3
};

enum sysEventType_t
{
	SE_NONE,				// evTime is still valid
	SE_KEY,					// evValue is a key code, evValue2 is the down flag
	SE_CHAR,				// evValue is an Unicode UTF-32 char (or non-surrogate UTF-16)
	SE_MOUSE,				// evValue and evValue2 are relative signed x / y moves
	SE_MOUSE_ABSOLUTE,		// evValue and evValue2 are absolute coordinates in the window's client area.
	SE_MOUSE_LEAVE,			// evValue and evValue2 are meaninless, this indicates the mouse has left the client area.
	SE_JOYSTICK,			// evValue is an axis number and evValue2 is the current state (-127 to 127)
	SE_CONSOLE				// evPtr is a char*, from typing something at a non-game console
};

struct sysEvent_t
{
	sysEventType_t	evType;
	int				evValue;
	int				evValue2;
	int				evPtrLength;		// bytes of data pointed to by evPtr, for journaling
	void* 			evPtr;				// this must be manually freed if not nullptr
	
	int				inputDevice;
	bool			IsKeyEvent() const
	{
		return evType == SE_KEY;
	}
	bool			IsMouseEvent() const
	{
		return evType == SE_MOUSE;
	}
	bool			IsCharEvent() const
	{
		return evType == SE_CHAR;
	}
	bool			IsJoystickEvent() const
	{
		return evType == SE_JOYSTICK;
	}
	bool			IsKeyDown() const
	{
		return evValue2 != 0;
	}
	keyNum_t		GetKey() const
	{
		return static_cast< keyNum_t >( evValue );
	}
	int				GetXCoord() const
	{
		return evValue;
	}
	int				GetYCoord() const
	{
		return evValue2;
	}
};

/*
==============================================================

	idSys

==============================================================
*/

struct idSys
{
	virtual void Init() = 0;
	virtual void Shutdown() = 0;
	
	virtual void Quit() = 0;
	
	virtual void			DebugPrintf( VERIFY_FORMAT_STRING const char* fmt, ... ) = 0;
	virtual void			DebugVPrintf( const char* fmt, va_list arg ) = 0;
	
	virtual double			GetClockTicks() = 0;
	virtual double			ClockTicksPerSecond() = 0;
	virtual cpuid_t			GetProcessorId() = 0;
	virtual const char* 	GetProcessorString() = 0;
	virtual const char* 	FPU_GetState() = 0;
	virtual bool			FPU_StackIsEmpty() = 0;
	virtual void			FPU_SetFTZ( bool enable ) = 0;
	virtual void			FPU_SetDAZ( bool enable ) = 0;
	
	virtual void			FPU_EnableExceptions( int exceptions ) = 0;
	
	virtual bool			LockMemory( void* ptr, int bytes ) = 0;
	virtual bool			UnlockMemory( void* ptr, int bytes ) = 0;
	
	virtual int				DLL_Load( const char* dllName ) = 0;
	virtual void* 			DLL_GetProcAddress( int dllHandle, const char* procName ) = 0;
	virtual void			DLL_Unload( int dllHandle ) = 0;
	virtual void			DLL_GetFileName( const char* baseName, char* dllName, int maxLength ) = 0;
	
	virtual sysEvent_t		GenerateMouseButtonEvent( int button, bool down ) = 0;
	virtual sysEvent_t		GenerateMouseMoveEvent( int deltax, int deltay ) = 0;
	
	virtual void			OpenURL( const char* url, bool quit ) = 0;
	virtual void			StartProcess( const char* exePath, bool quit ) = 0;
	// BP: Moved here from idCommon
	
	/// Prints message to the console, which may cause a screen update if com_refreshOnPrint is set
	virtual void				Printf( VERIFY_FORMAT_STRING const char* fmt, ... ) = 0;
	
	/// Prints message that only shows up if the "developer" cvar is set,
	/// and NEVER forces a screen update, which could cause reentrancy problems
	virtual void				DPrintf( VERIFY_FORMAT_STRING const char* fmt, ... ) = 0;
	
	/// Prints WARNING %s message and adds the warning message to a queue for printing later on
	virtual void				Warning( VERIFY_FORMAT_STRING const char* fmt, ... ) = 0;
	
	/// Prints WARNING %s message in yellow that only shows up if the "developer" cvar is set
	virtual void				DWarning( VERIFY_FORMAT_STRING const char* fmt, ... ) = 0;
	
	/// Issues a C++ throw. Normal errors just abort to the game loop,
	/// which is appropriate for media or dynamic logic errors
	virtual void				Error( VERIFY_FORMAT_STRING const char* fmt, ... ) ID_INSTANCE_ATTRIBUTE_PRINTF( 1, 2 ) = 0;
	
	/// Fatal errors quit all the way to a system dialog box, which is appropriate for
	/// static internal errors or cases where the system may be corrupted
	virtual void                FatalError( VERIFY_FORMAT_STRING const char* fmt, ... ) ID_INSTANCE_ATTRIBUTE_PRINTF( 1, 2 ) = 0;
};

/*
===============================================================================

	System API.

===============================================================================
*/

const int SYS_API_VERSION		= 1;

struct idCommon;
struct idCmdSystem;
struct idCVarSystem;
struct IFileSystem;

struct sysImport_t
{
	int							version;				// API version
	idCommon* 					common;					// common
	idCmdSystem* 				cmdSystem;				// console command system
	idCVarSystem* 				cvarSystem;				// console variable system
};

struct sysExport_t
{
	int							version;				// API version
	idSys* 						sys;					// non-portable system services
	IFileSystem* 				fileSystem;				// file system
};

extern "C" using GetSysAPI_t = sysExport_t *(*)( sysImport_t* import );

}; // namespace sbe