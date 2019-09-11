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


// DLL loading, the path should be a fully qualified OS path to the DLL file to be loaded

// RB: 64 bit fixes, changed int to intptr_t
intptr_t		Sys_DLL_Load( const char* dllName );
void* 			Sys_DLL_GetProcAddress( intptr_t dllHandle, const char* procName );
void			Sys_DLL_Unload( intptr_t dllHandle );
// RB end

bool			Sys_AlreadyRunning();


// empties the FPU stack
void			Sys_FPU_ClearStack();

// sets the FPU rounding mode
void			Sys_FPU_SetRounding( int rounding );

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