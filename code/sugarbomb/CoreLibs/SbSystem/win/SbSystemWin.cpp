/*
===========================================================================

Doom 3 BFG Edition GPL Source Code
Copyright (C) 1993-2012 id Software LLC, a ZeniMax Media company. 
Copyright (C) 2012 Robert Beckebans
Copyright (C) 2019 BlackPhrase

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

#include "precompiled.h"

#include "SbSystem.hpp"

/*
================
Sys_Init

The cvar system must already be setup
================
*/
#define OSR2_BUILD_NUMBER 1111
#define WIN98_BUILD_NUMBER 1998

void SbSystem::Init()
{
	CoInitialize( nullptr );

	// get WM_TIMER messages pumped every millisecond
//	SetTimer( nullptr, 0, 100, nullptr );

	//
	// Windows user name
	//
	win32.win_username.SetString( Sys_GetCurrentUser() );

	//
	// Windows version
	//
	win32.osversion.dwOSVersionInfoSize = sizeof( win32.osversion );

	if ( !GetVersionEx( (LPOSVERSIONINFO)&win32.osversion ) )
		Sys_Error( "Couldn't get OS info" );

	if ( win32.osversion.dwMajorVersion < 4 )
		Sys_Error( GAME_NAME " requires Windows version 4 (NT) or greater" );

	if ( win32.osversion.dwPlatformId == VER_PLATFORM_WIN32s )
		Sys_Error( GAME_NAME " doesn't run on Win32s" );

	if( win32.osversion.dwPlatformId == VER_PLATFORM_WIN32_NT )
	{
		if( win32.osversion.dwMajorVersion <= 4 )
			win32.sys_arch.SetString( "WinNT (NT)" );
		else if( win32.osversion.dwMajorVersion == 5 && win32.osversion.dwMinorVersion == 0 )
			win32.sys_arch.SetString( "Win2K (NT)" );
		else if( win32.osversion.dwMajorVersion == 5 && win32.osversion.dwMinorVersion == 1 )
			win32.sys_arch.SetString( "WinXP (NT)" );
		else if( win32.osversion.dwMajorVersion == 6 )
			win32.sys_arch.SetString( "Vista" );
		else if( win32.osversion.dwMajorVersion == 6 && win32.osversion.dwMinorVersion == 1 )
			win32.sys_arch.SetString( "Win7" );
		else if( win32.osversion.dwMajorVersion == 6 && win32.osversion.dwMinorVersion == 2 )
			win32.sys_arch.SetString( "Win8" );
		else if( win32.osversion.dwMajorVersion == 6 && win32.osversion.dwMinorVersion == 3 )
			win32.sys_arch.SetString( "Win8.1" );
		else
			win32.sys_arch.SetString( "Unknown NT variant" );
	}
	else if( win32.osversion.dwPlatformId == VER_PLATFORM_WIN32_WINDOWS )
	{
		if( win32.osversion.dwMajorVersion == 4 && win32.osversion.dwMinorVersion == 0 )
		{
			// Win95
			if( win32.osversion.szCSDVersion[1] == 'C' )
				win32.sys_arch.SetString( "Win95 OSR2 (95)" );
			else
				win32.sys_arch.SetString( "Win95 (95)" );
		}
		else if( win32.osversion.dwMajorVersion == 4 && win32.osversion.dwMinorVersion == 10 )
		{
			// Win98
			if( win32.osversion.szCSDVersion[1] == 'A' )
				win32.sys_arch.SetString( "Win98SE (95)" );
			else
				win32.sys_arch.SetString( "Win98 (95)" );
		}
		else if( win32.osversion.dwMajorVersion == 4 && win32.osversion.dwMinorVersion == 90 )
		{
			// WinMe
		  	win32.sys_arch.SetString( "WinMe (95)" );
		else
		  	win32.sys_arch.SetString( "Unknown 95 variant" );
	else
		win32.sys_arch.SetString( "unknown Windows variant" );

	//
	// CPU type
	//
	if ( !idStr::Icmp( win32.sys_cpustring.GetString(), "detect" ) )
	{
		idStr string;

		Printf( "%1.0f MHz ", Sys_ClockTicksPerSecond() / 1000000.0f );

		win32.cpuid = Sys_GetCPUId();

		string.Clear();

		if ( win32.cpuid & CPUID_AMD )
			string += "AMD CPU";
		else if ( win32.cpuid & CPUID_INTEL )
			string += "Intel CPU";
		else if ( win32.cpuid & CPUID_UNSUPPORTED )
			string += "unsupported CPU";
		else
			string += "generic CPU";

		string += " with ";
		if ( win32.cpuid & CPUID_MMX )
			string += "MMX & ";

		if ( win32.cpuid & CPUID_3DNOW )
			string += "3DNow! & ";

		if ( win32.cpuid & CPUID_SSE )
			string += "SSE & ";

		if ( win32.cpuid & CPUID_SSE2 )
            string += "SSE2 & ";

		if ( win32.cpuid & CPUID_SSE3 )
			string += "SSE3 & ";

		if ( win32.cpuid & CPUID_HTT )
			string += "HTT & ";

		string.StripTrailing( " & " );
		string.StripTrailing( " with " );
		win32.sys_cpustring.SetString( string );
	}
	else
	{
		Printf( "forcing CPU type to " );
		idLexer src( win32.sys_cpustring.GetString(), idStr::Length( win32.sys_cpustring.GetString() ), "sys_cpustring" );
		idToken token;

		int id = CPUID_NONE;
		while( src.ReadToken( &token ) )
		{
			if ( token.Icmp( "generic" ) == 0 )
				id |= CPUID_GENERIC;
			else if ( token.Icmp( "intel" ) == 0 )
				id |= CPUID_INTEL;
			else if ( token.Icmp( "amd" ) == 0 )
				id |= CPUID_AMD;
			else if ( token.Icmp( "mmx" ) == 0 )
				id |= CPUID_MMX;
			else if ( token.Icmp( "3dnow" ) == 0 )
				id |= CPUID_3DNOW;
			else if ( token.Icmp( "sse" ) == 0 )
				id |= CPUID_SSE;
			else if ( token.Icmp( "sse2" ) == 0 )
				id |= CPUID_SSE2;
			else if ( token.Icmp( "sse3" ) == 0 )
				id |= CPUID_SSE3;
			else if ( token.Icmp( "htt" ) == 0 )
				id |= CPUID_HTT;
		};

		if ( id == CPUID_NONE )
		{
			Printf( "WARNING: unknown sys_cpustring '%s'\n", win32.sys_cpustring.GetString() );
			id = CPUID_GENERIC;
		};

		win32.cpuid = (cpuid_t) id;
	};

	Printf( "%s\n", win32.sys_cpustring.GetString() );

	if ( ( win32.cpuid & CPUID_SSE2 ) == 0 )
		Error( "SSE2 not supported!" );
};

/*
================
Sys_Shutdown
================
*/
void SbSystem::Shutdown()
{
	CoUninitialize();
};

/*
==============
Sys_Quit
==============
*/
void SbSystem::Quit()
{
	timeEndPeriod( 1 );
	Sys_DestroyConsole();
	ExitProcess( 0 );
};

/*
================
Sys_GetDriveFreeSpace
returns in megabytes
================
*/
int SbSystem::GetDriveFreeSpace(const char *path) const
{
	DWORDLONG lpFreeBytesAvailable;
	DWORDLONG lpTotalNumberOfBytes;
	DWORDLONG lpTotalNumberOfFreeBytes;

	int ret = 26;

	//FIXME: see why this is failing on some machines
	if( ::GetDiskFreeSpaceEx( path, ( PULARGE_INTEGER )&lpFreeBytesAvailable, ( PULARGE_INTEGER )&lpTotalNumberOfBytes, ( PULARGE_INTEGER )&lpTotalNumberOfFreeBytes ) )
		ret = ( double )( lpFreeBytesAvailable ) / ( 1024.0 * 1024.0 );

	return ret;
};

/*
================
Sys_LockMemory
================
*/
bool SbSystem::LockMemory(void *ptr, int bytes)
{
	return ( VirtualLock( ptr, ( SIZE_T )bytes ) != FALSE );
};

/*
================
Sys_UnlockMemory
================
*/
bool SbSystem::UnlockMemory(void *ptr, int bytes)
{
	return ( VirtualUnlock( ptr, ( SIZE_T )bytes ) != FALSE );
};

/*
================
Sys_GetCurrentMemoryStatus

	returns OS mem info
	all values are in kB except the memoryload
================
*/
void SbSystem::GetCurrentMemoryStatus(sysMemoryStats_t &stats)
{
	MEMORYSTATUSEX statex = {};
	unsigned __int64 work;
	
	statex.dwLength = sizeof( statex );
	GlobalMemoryStatusEx( &statex );
	
	memset( &stats, 0, sizeof( stats ) );
	
	stats.memoryLoad = statex.dwMemoryLoad;
	
	work = statex.ullTotalPhys >> 20;
	stats.totalPhysical = *( int* )&work;
	
	work = statex.ullAvailPhys >> 20;
	stats.availPhysical = *( int* )&work;
	
	work = statex.ullAvailPageFile >> 20;
	stats.availPageFile = *( int* )&work;
	
	work = statex.ullTotalPageFile >> 20;
	stats.totalPageFile = *( int* )&work;
	
	work = statex.ullTotalVirtual >> 20;
	stats.totalVirtual = *( int* )&work;
	
	work = statex.ullAvailVirtual >> 20;
	stats.availVirtual = *( int* )&work;
	
	work = statex.ullAvailExtendedVirtual >> 20;
	stats.availExtendedVirtual = *( int* )&work;
};

/*
========================
Sys_GetDriveFreeSpaceInBytes
========================
*/
int64 SbSystem::GetDriveFreeSpaceInBytes(const char *path) const
{
	DWORDLONG lpFreeBytesAvailable;
	DWORDLONG lpTotalNumberOfBytes;
	DWORDLONG lpTotalNumberOfFreeBytes;

	int64 ret = 1;

	//FIXME: see why this is failing on some machines
	if( ::GetDiskFreeSpaceEx( path, ( PULARGE_INTEGER )&lpFreeBytesAvailable, ( PULARGE_INTEGER )&lpTotalNumberOfBytes, ( PULARGE_INTEGER )&lpTotalNumberOfFreeBytes ) )
		ret = lpFreeBytesAvailable;

	return ret;
};

/*
================
Sys_GetExeLaunchMemoryStatus
================
*/
void SbSystem::GetExeLaunchMemoryStatus(sysMemoryStats_t &aStats)
{
	aStats = exeLaunchMemoryStats;
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

/*
================
Sys_SetClipboardData
================
*/
void SbSystem::SetClipboardData( const char *string )
{
	HGLOBAL HMem;
	char *PMem;

	// allocate memory block
	HMem = (char *)::GlobalAlloc( GMEM_MOVEABLE | GMEM_DDESHARE, strlen( string ) + 1 );
	if ( HMem == nullptr )
		return;

	// lock allocated memory and obtain a pointer
	PMem = (char *)::GlobalLock( HMem );
	if ( PMem == nullptr )
		return;

	// copy text into allocated memory block
	lstrcpy( PMem, string );
	// unlock allocated memory
	::GlobalUnlock( HMem );
	// open Clipboard
	if ( !OpenClipboard( 0 ) )
	{
		::GlobalFree( HMem );
		return;
	};
	// remove current Clipboard contents
	EmptyClipboard();
	// supply the memory handle to the Clipboard
	SetClipboardData( CF_TEXT, HMem );
	HMem = 0;
	// close Clipboard
	CloseClipboard();
};

/*
================
Sys_GetClipboardData
================
*/
char *SbSystem::GetClipboardData() const
{
	char *data = nullptr;
	char *cliptext;

	if ( OpenClipboard( nullptr ) != 0 )
	{
		HANDLE hClipboardData;

		if ( ( hClipboardData = GetClipboardData( CF_TEXT ) ) != 0 )
		{
			if ( ( cliptext = (char *)GlobalLock( hClipboardData ) ) != 0 )
			{
				data = (char *)Mem_Alloc( GlobalSize( hClipboardData ) + 1, TAG_CRAP );
				strcpy( data, cliptext );
				GlobalUnlock( hClipboardData );
				
				strtok( data, "\n\r\b" );
			};
		};
		CloseClipboard();
	};

	return data;
};

/*
==============
Sys_Sleep
==============
*/
void SbSystem::Sleep(int msec)
{
	Sleep( msec );
};

/*
==================
idSysLocal::OpenURL
==================
*/
void SbSystem::OpenURL( const char *url, bool doexit )
{
	static bool doexit_spamguard = false;
	HWND wnd;

	if (doexit_spamguard)
	{
		DPrintf( "OpenURL: already in an exit sequence, ignoring %s\n", url );
		return;
	};

	Printf("Open URL: %s\n", url);

	if ( !ShellExecute( nullptr, "open", url, nullptr, nullptr, SW_RESTORE ) )
	{
		Error( "Could not open url: '%s' ", url );
		return;
	};

	wnd = GetForegroundWindow();
	if ( wnd )
		ShowWindow( wnd, SW_MAXIMIZE );

	if ( doexit )
	{
		doexit_spamguard = true;
		cmdSystem->BufferCommandText( CMD_EXEC_APPEND, "quit\n" );
	};
};

/*
==================
idSysLocal::StartProcess
==================
*/
void SbSystem::StartProcess( const char *exePath, bool doexit )
{
	TCHAR				szPathOrig[_MAX_PATH];
	STARTUPINFO			si;
	PROCESS_INFORMATION	pi;

	ZeroMemory( &si, sizeof(si) );
	si.cb = sizeof(si);

	strncpy( szPathOrig, exePath, _MAX_PATH );

	if( !CreateProcess( nullptr, szPathOrig, nullptr, nullptr, FALSE, 0, nullptr, nullptr, &si, &pi ) )
	{
        Error( "Could not start process: '%s' ", szPathOrig );
	    return;
	};

	if ( doexit )
		cmdSystem->BufferCommandText( CMD_EXEC_APPEND, "quit\n" );
};

/*
================
Sys_AlreadyRunning

returns true if there is a copy of D3 running already
================
*/
bool SbSystem::AlreadyRunning() const
{
#ifndef DEBUG
	if ( !win32.win_allowMultipleInstances.GetBool() )
	{
		hProcessMutex = ::CreateMutex( nullptr, FALSE, "DOOM3" );
		if ( ::GetLastError() == ERROR_ALREADY_EXISTS || ::GetLastError() == ERROR_ACCESS_DENIED )
			return true;
	};
#endif
	return false;
};