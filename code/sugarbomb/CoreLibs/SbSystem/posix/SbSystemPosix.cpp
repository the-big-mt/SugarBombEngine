/*
===========================================================================

Doom 3 GPL Source Code
Copyright (C) 1999-2011 id Software LLC, a ZeniMax Media company.
Copyright (C) 2012 Robert Beckebans
Copyright (C) 2019 BlackPhrase

This file is part of the Doom 3 GPL Source Code (?Doom 3 Source Code?).

Doom 3 Source Code is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

Doom 3 Source Code is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with Doom 3 Source Code.  If not, see <http://www.gnu.org/licenses/>.

In addition, the Doom 3 Source Code is also subject to certain additional terms. You should have received a copy of these additional terms immediately following the terms and conditions of the GNU General Public License which accompanied the Doom 3 Source Code.  If not, please request a copy in writing from id Software at the address below.

If you have questions concerning this license or the applicable additional terms, you may contact in writing id Software LLC, c/o ZeniMax Media Inc., Suite 120, Rockville, Maryland 20850 USA.

===========================================================================
*/

/// @file

#include "precompiled.h"

#include "SbSystem.hpp"

constexpr auto COMMAND_HISTORY{64};

static idStr basepath{""};
static idStr savepath{""};

static idStr history[COMMAND_HISTORY]; // cycle buffer

/*
=================
Sys_Init
Posix_EarlyInit/Posix_LateInit is better
=================
*/
void SbSystem::Init()
{
};

/*
=================
Posix_Shutdown
=================
*/
void Posix_Shutdown()
{
	for(int i = 0; i < COMMAND_HISTORY; ++i)
		history[i].Clear();
};

/*
================
Sys_Quit
================
*/
void SbSystem::Quit()
{
	Posix_Exit(EXIT_SUCCESS);
};

/*
===============
Sys_Shutdown
===============
*/
void SbSystem::Shutdown()
{
	basepath.Clear();
	savepath.Clear();
	Posix_Shutdown();
};

// RB begin

/*
================
Sys_GetDriveFreeSpace
returns in megabytes
================
*/
int SbSystem::GetDriveFreeSpace(const char *path)
{
	int ret = 26;
	
	struct statvfs st;
	
	if( statvfs( path, &st ) == 0 )
	{
		unsigned long blocksize = st.f_bsize;
		unsigned long freeblocks = st.f_bfree;
		
		unsigned long free = blocksize * freeblocks;
		
		ret = ( double )( free ) / ( 1024.0 * 1024.0 );
	};
	
	return ret;
};

/*
========================
Sys_GetDriveFreeSpaceInBytes
========================
*/
int64 SbSystem::GetDriveFreeSpaceInBytes(const char *path)
{
	int64 ret = 1;
	
	struct statvfs st;
	
	if( statvfs( path, &st ) == 0 )
	{
		unsigned long blocksize = st.f_bsize;
		unsigned long freeblocks = st.f_bfree;
		
		unsigned long free = blocksize * freeblocks;
		
		ret = free;
	};
	
	return ret;
};

// RB end

/*
================
Sys_LockMemory
================
*/
bool SbSystem::LockMemory(void *ptr, int bytes)
{
	return true;
};

/*
================
Sys_UnlockMemory
================
*/
bool SbSystem::UnlockMemory(void *ptr, int bytes)
{
	return true;
};

void SbSystem::GetCurrentMemoryStatus(sysMemoryStats_t &aStats)
{
	Printf( "FIXME: Sys_GetCurrentMemoryStatus\n" );
};

void SbSystem::GetExeLaunchMemoryStatus(sysMemoryStats_t &aStats)
{
	Printf( "FIXME: Sys_GetExeLaunchMemoryStatus\n" );
};

/*
================
Sys_SetLanguageFromSystem
================
*/
extern sbe::idCVar sys_lang;
void SbSystem::SetLanguageFromSystem()
{
	sys_lang.SetString(GetDefaultLanguage());
};

// TODO: SbSystem::SetClipboardData

// TODO: SbSystem::GetClipboardData

void SbSystem::Sleep(int msec)
{
#if 0 // DG: I don't really care, this spams the console (and on windows this case isn't handled either)
	// Furthermore, there are several Sys_Sleep( 10 ) calls throughout the code
	if( msec < 20 )
	{
		static int last = 0;
		int now = Sys_Milliseconds();
		if( now - last > 1000 )
		{
			Sys_Printf( "WARNING: Sys_Sleep - %d < 20 msec is not portable\n", msec );
			last = now;
		};
		// ignore that sleep call, keep going
		return;
	};
#endif // DG end
	// use nanosleep? keep sleeping if signal interrupt?
	
	// RB begin
#if defined(__ANDROID__)
	usleep( msec * 1000 );
#else
	if( usleep( msec * 1000 ) == -1 )
		Sys_Printf( "usleep: %s\n", strerror( errno ) );
#endif
};

/*
=================
Sys_OpenURL
=================
*/
void SbSystem::OpenURL( const char* url, bool quit )
{
	const char*	script_path;
	idFile*		script_file;
	char		cmdline[ 1024 ];
	
	static bool	quit_spamguard = false;
	
	if( quit_spamguard )
	{
		DPrintf( "Sys_OpenURL: already in a doexit sequence, ignoring %s\n", url );
		return;
	};
	
	// FIXME: this could use xdg-open
	
	Printf( "Open URL: %s\n", url );
	// opening an URL on *nix can mean a lot of things ..
	// just spawn a script instead of deciding for the user :-)
	
	// look in the savepath first, then in the basepath
	script_path = mpFileSystem->BuildOSPath( cvarSystem->GetCVarString( "fs_savepath" ), "", "openurl.sh" );
	script_file = mpFileSystem->OpenExplicitFileRead( script_path );
	
	if( !script_file )
	{
		script_path = mpFileSystem->BuildOSPath( cvarSystem->GetCVarString( "fs_basepath" ), "", "openurl.sh" );
		script_file = mpFileSystem->OpenExplicitFileRead( script_path );
	};
	
	if( !script_file )
	{
		Printf( "Can't find URL script 'openurl.sh' in either savepath or basepath\n" );
		Printf( "OpenURL '%s' failed\n", url );
		return;
	};
	
	mpFileSystem->CloseFile( script_file );
	
	// if we are going to quit, only accept a single URL before quitting and spawning the script
	if( quit )
		quit_spamguard = true;
	
	Printf( "URL script: %s\n", script_path );
	
	// StartProcess is going to execute a system() call with that - hence the &
	idStr::snPrintf( cmdline, 1024, "%s '%s' &",  script_path, url );
	StartProcess( cmdline, quit );
};

/*
===============
Posix_SetExitSpawn
set the process to be spawned when we quit
===============
*/
void Posix_SetExitSpawn(const char* exeName)
{
	idStr::Copynz( exit_spawn, exeName, 1024 );
};

/*
==================
idSysLocal::StartProcess
if !quit, start the process asap
otherwise, push it for execution at exit
(i.e. let complete shutdown of the game and freeing of resources happen)
NOTE: might even want to add a small delay?
==================
*/
void SbSystem::StartProcess( const char* exeName, bool quit )
{
	if( quit )
	{
		DPrintf( "Sys_StartProcess %s (delaying until final exit)\n", exeName );
		Posix_SetExitSpawn( exeName );
		cmdSystem->BufferCommandText( CMD_EXEC_APPEND, "quit\n" );
		return;
	};
	
	DPrintf( "Sys_StartProcess %s\n", exeName );
	Sys_DoStartProcess( exeName );
};

/*
================
Sys_AlreadyRunning
return true if there is a copy of D3 running already
================
*/
bool SbSystem::AlreadyRunning() const
{
	return false;
};