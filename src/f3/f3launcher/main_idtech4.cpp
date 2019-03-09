/*
===========================================================================

Doom 3 BFG Edition GPL Source Code
Copyright (C) 1993-2012 id Software LLC, a ZeniMax Media company. 
Copyright (C) 2012 Robert Beckebans

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

#ifdef __linux__
/*
===============
main
===============
*/
int main( int argc, const char** argv )
{
#ifdef __linux__
#ifdef USE_BREAKPAD
	google_breakpad::MinidumpDescriptor descriptor( "/tmp" );
	google_breakpad::ExceptionHandler eh( descriptor, nullptr, BFG::breakpad_dumpCallback, nullptr, true, -1 );
#endif
#endif

#if defined(__linux__) || defined(__APPLE__) // posix
	// DG: needed for Sys_ReLaunch()
	cmdargc = argc;
	cmdargv = argv;
	// DG end
#endif

#ifdef __linux__
#ifdef ID_MCHECK
	// must have -lmcheck linkage
	mcheck( abrt_func );
	Sys_Printf( "memory consistency checking enabled\n" );
#endif
#endif

#if defined(__linux__) || defined(__APPLE__) // posix
	Posix_EarlyInit( );

	if( argc > 1 )
		common->Init( argc - 1, &argv[1], nullptr );
	else
		common->Init( 0, nullptr, nullptr );

	Posix_LateInit( );
#elif _WIN32
#ifdef USE_BREAKPAD
    google_breakpad::ExceptionHandler *pHandler =
        new google_breakpad::ExceptionHandler(
                                              L"%TMP%\\", // FIXME: provide base path here, dir must exist
                                              nullptr,
                                              nullptr,
                                              0,
                                              google_breakpad::ExceptionHandler::HANDLER_ALL,
                                              MiniDumpNormal,
                                              L"",
                                              0 );
#endif

	const HCURSOR hcurSave = ::SetCursor( LoadCursor( 0, IDC_WAIT ) );

	Sys_SetPhysicalWorkMemory( 192 << 20, 1024 << 20 );

	Sys_GetCurrentMemoryStatus( exeLaunchMemoryStats );

#if 0
    DWORD handler = (DWORD)_except_handler;
    __asm
    {                           // Build EXCEPTION_REGISTRATION record:
        push    handler         // Address of handler function
        push    FS:[0]          // Address of previous handler
        mov     FS:[0],ESP      // Install new EXECEPTION_REGISTRATION
    }
#endif

	win32.hInstance = hInstance;
	idStr::Copynz( sys_cmdline, lpCmdLine, sizeof( sys_cmdline ) );

	// done before Com/Sys_Init since we need this for error output
	Sys_CreateConsole();

	// no abort/retry/fail errors
	SetErrorMode( SEM_FAILCRITICALERRORS );

	for ( int i = 0; i < MAX_CRITICAL_SECTIONS; i++ )
		InitializeCriticalSection( &win32.criticalSections[i] );

	// make sure the timer is high precision, otherwise
	// NT gets 18ms resolution
	timeBeginPeriod( 1 );

	// get the initial time base
	Sys_Milliseconds();

#ifdef DEBUG
	// disable the painfully slow MS heap check every 1024 allocs
	_CrtSetDbgFlag( 0 );
#endif

//	Sys_FPU_EnableExceptions( TEST_FPU_EXCEPTIONS );
	Sys_FPU_SetPrecision( FPU_PRECISION_DOUBLE_EXTENDED );

	common->Init( 0, nullptr, lpCmdLine );

#if TEST_FPU_EXCEPTIONS != 0
	common->Printf( Sys_FPU_GetState() );
#endif

	if ( win32.win_notaskkeys.GetInteger() )
		DisableTaskKeys( TRUE, FALSE, /*( win32.win_notaskkeys.GetInteger() == 2 )*/ FALSE );

	// hide or show the early console as necessary
	if ( win32.win_viewlog.GetInteger() )
		Sys_ShowConsole( 1, true );
	else
		Sys_ShowConsole( 0, false );

#ifdef SET_THREAD_AFFINITY 
	// give the main thread an affinity for the first cpu
	SetThreadAffinityMask( GetCurrentThread(), 1 );
#endif

	::SetCursor( hcurSave );

	::SetFocus( win32.hWnd );
#endif // defined(__linux__) || defined(__APPLE__)

	// main game loop
	while( 1 )
	{
#ifdef _WIN32
		Win_Frame();

#ifdef DEBUG
		Sys_MemFrame();
#endif

		// set exceptions, even if some crappy syscall changes them!
		Sys_FPU_EnableExceptions( TEST_FPU_EXCEPTIONS );
#endif // _WIN32

		// run the game
		common->Frame();
	};

	// never gets here
	return 0;
};
#endif