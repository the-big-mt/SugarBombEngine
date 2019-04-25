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

#include "SbApplication.hpp"
#include "win_local.hpp"

#ifdef USE_BREAKPAD
#include "client/windows/handler/exception_handler.h"
#endif

static char sys_cmdline[MAX_STRING_CHARS];

/*
=================
Sys_In_Restart_f

Restart the input subsystem
=================
*/
void Sys_In_Restart_f( const idCmdArgs &args )
{
	Sys_ShutdownInput();
	Sys_InitInput();
};

/*
====================
Win_Frame
====================
*/
void Win_Frame()
{
	// if "viewlog" has been modified, show or hide the log console
	if ( win32.win_viewlog.IsModified() )
		win32.win_viewlog.ClearModified();
};

#pragma optimize( "", on )

#ifdef DEBUG

static unsigned int debug_total_alloc = 0;
static unsigned int debug_total_alloc_count = 0;
static unsigned int debug_current_alloc = 0;
static unsigned int debug_current_alloc_count = 0;
static unsigned int debug_frame_alloc = 0;
static unsigned int debug_frame_alloc_count = 0;

idCVar sys_showMallocs( "sys_showMallocs", "0", CVAR_SYSTEM, "" );

// _HOOK_ALLOC, _HOOK_REALLOC, _HOOK_FREE

typedef struct CrtMemBlockHeader
{
	struct _CrtMemBlockHeader *pBlockHeaderNext;	// Pointer to the block allocated just before this one:
	struct _CrtMemBlockHeader *pBlockHeaderPrev;	// Pointer to the block allocated just after this one
   char *szFileName;    // File name
   int nLine;           // Line number
   size_t nDataSize;    // Size of user block
   int nBlockUse;       // Type of block
   long lRequest;       // Allocation number
	byte		gap[4];								// Buffer just before (lower than) the user's memory:
} CrtMemBlockHeader;

#include <crtdbg.h>

/*
==================
Sys_AllocHook

	called for every malloc/new/free/delete
==================
*/
int Sys_AllocHook( int nAllocType, void *pvData, size_t nSize, int nBlockUse, long lRequest, const unsigned char * szFileName, int nLine ) 
{
	CrtMemBlockHeader	*pHead;
	byte				*temp;

	if ( nBlockUse == _CRT_BLOCK )
	{
      return( TRUE );
	}

	// get a pointer to memory block header
	temp = ( byte * )pvData;
	temp -= 32;
	pHead = ( CrtMemBlockHeader * )temp;

	switch( nAllocType ) {
		case	_HOOK_ALLOC:
			debug_total_alloc += nSize;
			debug_current_alloc += nSize;
			debug_frame_alloc += nSize;
			debug_total_alloc_count++;
			debug_current_alloc_count++;
			debug_frame_alloc_count++;
			break;

		case	_HOOK_FREE:
			assert( pHead->gap[0] == 0xfd && pHead->gap[1] == 0xfd && pHead->gap[2] == 0xfd && pHead->gap[3] == 0xfd );

			debug_current_alloc -= pHead->nDataSize;
			debug_current_alloc_count--;
			debug_total_alloc_count++;
			debug_frame_alloc_count++;
			break;

		case	_HOOK_REALLOC:
			assert( pHead->gap[0] == 0xfd && pHead->gap[1] == 0xfd && pHead->gap[2] == 0xfd && pHead->gap[3] == 0xfd );

			debug_current_alloc -= pHead->nDataSize;
			debug_total_alloc += nSize;
			debug_current_alloc += nSize;
			debug_frame_alloc += nSize;
			debug_total_alloc_count++;
			debug_current_alloc_count--;
			debug_frame_alloc_count++;
			break;
	}
	return( TRUE );
}

/*
==================
Sys_DebugMemory_f
==================
*/
void Sys_DebugMemory_f()
{
  	common->Printf( "Total allocation %8dk in %d blocks\n", debug_total_alloc / 1024, debug_total_alloc_count );
  	common->Printf( "Current allocation %8dk in %d blocks\n", debug_current_alloc / 1024, debug_current_alloc_count );
}

/*
==================
Sys_MemFrame
==================
*/
void Sys_MemFrame()
{
	if( sys_showMallocs.GetInteger() ) {
		common->Printf("Frame: %8dk in %5d blocks\n", debug_frame_alloc / 1024, debug_frame_alloc_count );
	}

	debug_frame_alloc = 0;
	debug_frame_alloc_count = 0;
}

#endif

/*
====================
GLW_GetWindowDimensions
====================
*/
static bool GLW_GetWindowDimensions( const glimpParms_t parms, int& x, int& y, int& w, int& h )
{
	//
	// compute width and height
	//
	if( parms.fullScreen != 0 )
	{
		if( parms.fullScreen == -1 )
		{
			// borderless window at specific location, as for spanning
			// multiple monitor outputs
			x = parms.x;
			y = parms.y;
			w = parms.width;
			h = parms.height;
		}
		else
		{
			// get the current monitor position and size on the desktop, assuming
			// any required ChangeDisplaySettings has already been done
			int displayHz = 0;
			if( !GetDisplayCoordinates( parms.fullScreen - 1, x, y, w, h, displayHz ) )
			{
				return false;
			}
		}
	}
	else
	{
		RECT	r;
		
		// adjust width and height for window border
		r.bottom = parms.height;
		r.left = 0;
		r.top = 0;
		r.right = parms.width;
		
		AdjustWindowRect( &r, WINDOW_STYLE | WS_SYSMENU, FALSE );
		
		w = r.right - r.left;
		h = r.bottom - r.top;
		
		x = parms.x;
		y = parms.y;
	}
	
	return true;
}

SbApplicationWin::~SbApplicationWin()
{
	// destroy window
	if( win32.hWnd )
	{
		common->Printf( "...destroying window\n" );
		ShowWindow( win32.hWnd, SW_HIDE );
		DestroyWindow( win32.hWnd );
		win32.hWnd = nullptr;
	};
};

void SbApplicationWin::Init()
{
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
	
	cmdSystem->AddCommand( "in_restart", Sys_In_Restart_f, CMD_FL_SYSTEM, "restarts the input system" );
};

void SbApplicationWin::Frame()
{
	Win_Frame();

#ifdef DEBUG
	Sys_MemFrame();
#endif

	// set exceptions, even if some crappy syscall changes them!
	Sys_FPU_EnableExceptions( TEST_FPU_EXCEPTIONS );
};

/*
=======================
GLW_CreateWindow

Responsible for creating the Win32 window.
If fullscreen, it won't have a border
=======================
*/
void SbApplicationWin::CreateMainWindow(int anWidth, int anHeight, const char *asTitle, bool abFullScreen)
{
	int				x, y, w, h;
	if( !GLW_GetWindowDimensions( parms, x, y, w, h ) )
	{
		return false;
	}
	
	int				stylebits;
	int				exstyle;
	if( parms.fullScreen != 0 )
	{
		exstyle = WS_EX_TOPMOST;
		stylebits = WS_POPUP | WS_VISIBLE | WS_SYSMENU;
	}
	else
	{
		exstyle = 0;
		stylebits = WINDOW_STYLE | WS_SYSMENU;
	}
	
	win32.hWnd = CreateWindowEx(
					 exstyle,
					 WIN32_WINDOW_CLASS_NAME,
					 GAME_NAME,
					 stylebits,
					 x, y, w, h,
					 nullptr,
					 nullptr,
					 win32.hInstance,
					 nullptr );
					 
	if( !win32.hWnd )
	{
		common->Printf( "^3GLW_CreateWindow() - Couldn't create window^0\n" );
		return false;
	};
	
	::SetTimer( win32.hWnd, 0, 100, nullptr );
	
	ShowWindow( win32.hWnd, SW_SHOW );
	UpdateWindow( win32.hWnd );
	common->Printf( "...created window @ %d,%d (%dx%d)\n", x, y, w, h );
	
	SetForegroundWindow( win32.hWnd );
	SetFocus( win32.hWnd );
};