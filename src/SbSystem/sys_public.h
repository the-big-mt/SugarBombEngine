/*
*******************************************************************************

Copyright (C) 1993-2012 id Software LLC, a ZeniMax Media company.
Copyright (C) 2012 Robert Beckebans
Copyright (C) 2019 BlackPhrase

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

#ifndef __SYS_PUBLIC__
#define __SYS_PUBLIC__

//#include <cstdarg>
//#include <stdint.h>
//#include <cstdio>

//#include "../idlib/sys/sys_defines.h"
#include "../idlib/CmdArgs.h"

//namespace BFG
//{
enum fpuExceptions_t
{
	FPU_EXCEPTION_INVALID_OPERATION		= 1,
	FPU_EXCEPTION_DENORMALIZED_OPERAND	= 2,
	FPU_EXCEPTION_DIVIDE_BY_ZERO		= 4,
	FPU_EXCEPTION_NUMERIC_OVERFLOW		= 8,
	FPU_EXCEPTION_NUMERIC_UNDERFLOW		= 16,
	FPU_EXCEPTION_INEXACT_RESULT		= 32
};

enum fpuPrecision_t
{
	FPU_PRECISION_SINGLE				= 0,
	FPU_PRECISION_DOUBLE				= 1,
	FPU_PRECISION_DOUBLE_EXTENDED		= 2
};


enum joystickAxis_t
{
	AXIS_LEFT_X,
	AXIS_LEFT_Y,
	AXIS_RIGHT_X,
	AXIS_RIGHT_Y,
	AXIS_LEFT_TRIG,
	AXIS_RIGHT_TRIG,
	MAX_JOYSTICK_AXIS
};

enum sys_mEvents
{
	M_ACTION1,
	M_ACTION2,
	M_ACTION3,
	M_ACTION4,
	M_ACTION5,
	M_ACTION6,
	M_ACTION7,
	M_ACTION8,
	// DG: support some more mouse buttons
	M_ACTION9,
	M_ACTION10,
	M_ACTION11,
	M_ACTION12,
	M_ACTION13,
	M_ACTION14,
	M_ACTION15,
	M_ACTION16,
	// DG end
	M_DELTAX,
	M_DELTAY,
	M_DELTAZ,
	M_INVALID
};

enum sys_jEvents
{
	J_ACTION1,
	J_ACTION2,
	J_ACTION3,
	J_ACTION4,
	J_ACTION5,
	J_ACTION6,
	J_ACTION7,
	J_ACTION8,
	J_ACTION9,
	J_ACTION10,
	J_ACTION11,
	J_ACTION12,
	J_ACTION13,
	J_ACTION14,
	J_ACTION15,
	J_ACTION16,
	J_ACTION17,
	J_ACTION18,
	J_ACTION19,
	J_ACTION20,
	J_ACTION21,
	J_ACTION22,
	J_ACTION23,
	J_ACTION24,
	J_ACTION25,
	J_ACTION26,
	J_ACTION27,
	J_ACTION28,
	J_ACTION29,
	J_ACTION30,
	J_ACTION31,
	J_ACTION32,
	J_ACTION_MAX = J_ACTION32,
	
	J_AXIS_MIN,
	J_AXIS_LEFT_X = J_AXIS_MIN + AXIS_LEFT_X,
	J_AXIS_LEFT_Y = J_AXIS_MIN + AXIS_LEFT_Y,
	J_AXIS_RIGHT_X = J_AXIS_MIN + AXIS_RIGHT_X,
	J_AXIS_RIGHT_Y = J_AXIS_MIN + AXIS_RIGHT_Y,
	J_AXIS_LEFT_TRIG = J_AXIS_MIN + AXIS_LEFT_TRIG,
	J_AXIS_RIGHT_TRIG = J_AXIS_MIN + AXIS_RIGHT_TRIG,
	
	J_AXIS_MAX = J_AXIS_MIN + MAX_JOYSTICK_AXIS - 1,
	
	J_DPAD_UP,
	J_DPAD_DOWN,
	J_DPAD_LEFT,
	J_DPAD_RIGHT,
	
	MAX_JOY_EVENT
};

struct sysMemoryStats_t
{
	int memoryLoad;
	int totalPhysical;
	int availPhysical;
	int totalPageFile;
	int availPageFile;
	int totalVirtual;
	int availVirtual;
	int availExtendedVirtual;
};

void			Sys_Error( const char* error, ... );
// a decent minimum sleep time to avoid going below the process scheduler speeds
#define			SYS_MINSLEEP	20

// Sys_Milliseconds should only be used for profiling purposes,
// any game related timing information should come from event timestamps
int				Sys_Milliseconds();
uint64			Sys_Microseconds();

// empties the FPU stack
void			Sys_FPU_ClearStack();

// sets the FPU precision
void			Sys_FPU_SetPrecision( int precision );

// sets the FPU rounding mode
void			Sys_FPU_SetRounding( int rounding );

// returns amount of drive space in path
int				Sys_GetDriveFreeSpace( const char* path );

// returns amount of drive space in path in bytes
int64			Sys_GetDriveFreeSpaceInBytes( const char* path );

// returns memory stats
void			Sys_GetCurrentMemoryStatus( sysMemoryStats_t& stats );
void			Sys_GetExeLaunchMemoryStatus( sysMemoryStats_t& stats );

// set amount of physical work memory
void			Sys_SetPhysicalWorkMemory( int minBytes, int maxBytes );

// event generation
void			Sys_GenerateEvents();
sysEvent_t		Sys_GetEvent();
void			Sys_ClearEvents();

void			Sys_ShowWindow( bool show );
bool			Sys_IsWindowVisible();
void			Sys_ShowConsole( int visLevel, bool quitOnClose );

// This really isn't the right place to have this, but since this is the 'top level' include
// and has a function signature with 'FILE' in it, it kinda needs to be here =/

// RB begin
#if defined(_WIN32)
typedef HANDLE idFileHandle;
#else
typedef FILE* idFileHandle;
#endif
// RB end


ID_TIME_T		Sys_FileTimeStamp( idFileHandle fp );
// NOTE: do we need to guarantee the same output on all platforms?
const char* 	Sys_TimeStampToStr( ID_TIME_T timeStamp );
const char* 	Sys_SecToStr( int sec );

const char* 	Sys_DefaultBasePath();
const char* 	Sys_DefaultSavePath();


// localization

#define ID_LANG_ENGLISH		"english"
#define ID_LANG_FRENCH		"french"
#define ID_LANG_ITALIAN		"italian"
#define ID_LANG_GERMAN		"german"
#define ID_LANG_SPANISH		"spanish"
#define ID_LANG_JAPANESE	"japanese"
int Sys_NumLangs();
const char* Sys_Lang( int idx );


struct idSys : public ISys
{
	virtual void			DebugVPrintf( const char* fmt, va_list arg ) = 0;
	
	// BP: Moved here from idCommon
	
	/// Same as Printf, with a more usable API - Printf pipes to this
	virtual void				VPrintf( const char* fmt, va_list arg ) = 0;
};

/*
================================================
idJoystick is managed by each platform's local Sys implementation, and
provides full *Joy Pad* support (the most common device, these days).
================================================
*/
class idJoystick
{
public:
	virtual			~idJoystick() { }
	
	virtual bool	Init()
	{
		return false;
	}
	virtual void	Shutdown() { }
	virtual void	Deactivate() { }
	virtual void	SetRumble( int deviceNum, int rumbleLow, int rumbleHigh ) { }
	virtual int		PollInputEvents( int inputDeviceNum )
	{
		return 0;
	}
	virtual int		ReturnInputEvent( const int n, int& action, int& value )
	{
		return 0;
	}
	virtual void	EndInputEvents() { }
};

#include "sys/ISys.hpp"

extern idSys* 				sys;

bool Sys_LoadOpenAL();
void Sys_FreeOpenAL();

//} // namespace BFG

#endif /* !__SYS_PUBLIC__ */