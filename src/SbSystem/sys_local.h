/*
*******************************************************************************

Copyright (C) 1993-2012 id Software LLC, a ZeniMax Media company.
Copyright (C) 2012 Robert Beckebans
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

#ifndef __SYS_LOCAL__
#define __SYS_LOCAL__

//#include <cstdarg>
//#include <cstdint>
//#include <cstdio>

//#include "idlib/sys/sys_defines.h"
//#include "idlib/CmdArgs.h"


//namespace sbe
//{

void			Sys_Quit();

// guaranteed to be thread-safe
void			Sys_DebugPrintf( VERIFY_FORMAT_STRING const char* fmt, ... );
void			Sys_DebugVPrintf( const char* fmt, va_list arg );

// for accurate performance testing
double			Sys_GetClockTicks();
double			Sys_ClockTicksPerSecond();

// returns a selection of the CPUID_* flags
cpuid_t			Sys_GetProcessorId();
const char* 	Sys_GetProcessorString();

// returns the FPU state as a string
const char* 	Sys_FPU_GetState();

// returns true if the FPU stack is empty
bool			Sys_FPU_StackIsEmpty();

// sets Flush-To-Zero mode (only available when CPUID_FTZ is set)
void			Sys_FPU_SetFTZ( bool enable );

// sets Denormals-Are-Zero mode (only available when CPUID_DAZ is set)
void			Sys_FPU_SetDAZ( bool enable );

// enables the given FPU exceptions
void			Sys_FPU_EnableExceptions( int exceptions );

// DLL loading, the path should be a fully qualified OS path to the DLL file to be loaded

// RB: 64 bit fixes, changed int to intptr_t
intptr_t		Sys_DLL_Load( const char* dllName );
void* 			Sys_DLL_GetProcAddress( intptr_t dllHandle, const char* procName );
void			Sys_DLL_Unload( intptr_t dllHandle );
// RB end

bool			Sys_AlreadyRunning();

void			Sys_Launch( const char* path, idCmdArgs& args,  void* launchData, unsigned int launchDataSize );

// Execute the specified process and wait until it's done, calling workFn every waitMS milliseconds.
// If showOutput == true, std IO from the executed process will be output to the console.
// Note that the return value is not an indication of the exit code of the process, but is false
// only if the process could not be created at all. If you wish to check the exit code of the
// spawned process, check the value returned in exitCode.
typedef bool ( *execProcessWorkFunction_t )();
typedef void ( *execOutputFunction_t )( const char* text );
bool Sys_Exec(	const char* appPath, const char* workingPath, const char* args,
				execProcessWorkFunction_t workFn, execOutputFunction_t outputFn, const int waitMS,
				unsigned int& exitCode );

enum fpuRounding_t
{
	FPU_ROUNDING_TO_NEAREST				= 0,
	FPU_ROUNDING_DOWN					= 1,
	FPU_ROUNDING_UP						= 2,
	FPU_ROUNDING_TO_ZERO				= 3
};

// sets the FPU rounding mode
void			Sys_FPU_SetRounding( int rounding );

// will go to the various text consoles
// NOT thread safe - never use in the async paths
void			Sys_Printf( VERIFY_FORMAT_STRING const char* msg, ... );

// DG: Sys_ReLaunch() doesn't need any options (and the old way is painful for POSIX systems)
void			Sys_ReLaunch();
// DG end

const char* 	Sys_GetCmdLine();

// know early if we are performing a fatal error shutdown so the error message doesn't get lost
void			Sys_SetFatalError( const char* error );

/*
==============================================================

	idSysLocal

==============================================================
*/

class idSysLocal : public idSys
{
public:
	void Init() override;
	void Shutdown() override;
	
	void Quit() override;
	
	virtual void			DebugPrintf( VERIFY_FORMAT_STRING const char* fmt, ... );
	virtual void			DebugVPrintf( const char* fmt, va_list arg ) override;
	
	virtual double			GetClockTicks();
	virtual double			ClockTicksPerSecond();
	virtual cpuid_t			GetProcessorId();
	virtual const char* 	GetProcessorString();
	virtual const char* 	FPU_GetState();
	virtual bool			FPU_StackIsEmpty();
	virtual void			FPU_SetFTZ( bool enable );
	virtual void			FPU_SetDAZ( bool enable );
	
	virtual void			FPU_EnableExceptions( int exceptions );
	
	/// lock and unlock memory
	virtual bool			LockMemory( void* ptr, int bytes );
	virtual bool			UnlockMemory( void* ptr, int bytes );
	
	virtual int				DLL_Load( const char* dllName ) override;
	virtual void* 			DLL_GetProcAddress( int dllHandle, const char* procName ) override;
	virtual void			DLL_Unload( int dllHandle ) override;
	virtual void			DLL_GetFileName( const char* baseName, char* dllName, int maxLength ) override;
	
	virtual sysEvent_t		GenerateMouseButtonEvent( int button, bool down );
	virtual sysEvent_t		GenerateMouseMoveEvent( int deltax, int deltay );
	
	virtual void			OpenURL( const char* url, bool quit );
	virtual void			StartProcess( const char* exeName, bool quit );
	
	virtual int GetLangsNum() const override;
	virtual const char *GetLangName(int anIndex) const override;
	
	bool AlreadyRunning() const override;
	
	/// returns memory stats
	void GetCurrentMemoryStatus(sysMemoryStats_t &aStats) override; // TODO: should this be public?
	
	/// allow game to yield CPU time
	// NOTE: due to SYS_MINSLEEP this is very bad portability karma, and should be completely removed
	void Sleep(int msec) override;
private:
	/// returns amount of drive space in path
	int GetDriveFreeSpace(const char *asPath) const;

	/// returns amount of drive space in path in bytes
	int64 GetDriveFreeSpaceInBytes(const char *asPath) const;

	void GetExeLaunchMemoryStatus(sysMemoryStats_t &aStats);
};


#endif /* !__SYS_LOCAL__ *///} // namespace sbe