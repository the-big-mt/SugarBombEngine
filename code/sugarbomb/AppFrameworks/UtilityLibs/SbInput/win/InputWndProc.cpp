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

/*
====================
InputWndProc

input window procedure
====================
*/
LONG WINAPI InputWndProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	switch(uMsg)
	{
	case WM_SYSKEYDOWN:
		if( wParam == 13 )  	// alt-enter toggles full-screen
		{
			cvarSystem->SetCVarBool( "r_fullscreen", !renderSystem->IsFullScreen() );
			cmdSystem->BufferCommandText( CMD_EXEC_APPEND, "vid_restart\n" );
			return 0; // TODO: break?
		}
	// fall through for other keys
	case WM_KEYDOWN:
		key = ( ( lParam >> 16 ) & 0xFF ) | ( ( ( lParam >> 24 ) & 1 ) << 7 );
		if( key == K_LCTRL || key == K_LALT || key == K_RCTRL || key == K_RALT )
		{
			// let direct-input handle this because windows sends Alt-Gr
			// as two events (ctrl then alt)
			break;
		}
		// D
		if( key == K_NUMLOCK )
		{
			key = K_PAUSE;
		}
		else if( key == K_PAUSE )
		{
			key = K_NUMLOCK;
		}
		Sys_QueEvent( SE_KEY, key, true, 0, nullptr, 0 );
		
		break;
		
	case WM_SYSKEYUP:
	case WM_KEYUP:
		key = ( ( lParam >> 16 ) & 0xFF ) | ( ( ( lParam >> 24 ) & 1 ) << 7 );
		if( key == K_PRINTSCREEN )
		{
			// don't queue printscreen keys.  Since windows doesn't send us key
			// down events for this, we handle queueing them with DirectInput
			break;
		}
		else if( key == K_LCTRL || key == K_LALT || key == K_RCTRL || key == K_RALT )
		{
			// let direct-input handle this because windows sends Alt-Gr
			// as two events (ctrl then alt)
			break;
		}
		Sys_QueEvent( SE_KEY, key, false, 0, nullptr, 0 );
		break;
		
	case WM_CHAR:
		// DG: make sure it's an utf-16 non-surrogate character (and thus a valid utf-32 character as well)
		// TODO: will there ever be two messages with surrogate characters that should be combined?
		//       (probably not, some people claim it's actually UCS-2, not UTF-16)
		if( wParam < 0xD800 || wParam > 0xDFFF )
		{
			Sys_QueEvent( SE_CHAR, wParam, 0, 0, nullptr, 0 );
		}
		break;
		
	// DG: support utf-32 input via WM_UNICHAR
	case WM_UNICHAR:
		Sys_QueEvent( SE_CHAR, wParam, 0, 0, nullptr, 0 );
		break;
	// DG end

	case WM_LBUTTONDOWN:
	{
		Sys_QueEvent( SE_KEY, K_MOUSE1, 1, 0, nullptr, 0 );
		return 0; // TODO: break?
	}
	case WM_LBUTTONUP:
	{
		Sys_QueEvent( SE_KEY, K_MOUSE1, 0, 0, nullptr, 0 );
		return 0; // TODO: break?
	}
	case WM_RBUTTONDOWN:
	{
		Sys_QueEvent( SE_KEY, K_MOUSE2, 1, 0, nullptr, 0 );
		return 0; // TODO: break?
	}
	case WM_RBUTTONUP:
	{
		Sys_QueEvent( SE_KEY, K_MOUSE2, 0, 0, nullptr, 0 );
		return 0; // TODO: break?
	}
	case WM_MBUTTONDOWN:
	{
		Sys_QueEvent( SE_KEY, K_MOUSE3, 1, 0, nullptr, 0 );
		return 0; // TODO: break?
	}
	case WM_MBUTTONUP:
	{
		Sys_QueEvent( SE_KEY, K_MOUSE3, 0, 0, nullptr, 0 );
		return 0; // TODO: break?
	}
	case WM_XBUTTONDOWN:
	{
		// RB begin
#if defined(__MINGW32__)
		Sys_QueEvent( SE_KEY, K_MOUSE4, 1, 0, nullptr, 0 );
#else
		int button = GET_XBUTTON_WPARAM( wParam );
		if( button == 1 )
		{
			Sys_QueEvent( SE_KEY, K_MOUSE4, 1, 0, nullptr, 0 );
		}
		else if( button == 2 )
		{
			Sys_QueEvent( SE_KEY, K_MOUSE5, 1, 0, nullptr, 0 );
		}
#endif
		// RB end
		return 0; // TODO: break?
	}
	case WM_XBUTTONUP:
	{
		// RB begin
#if defined(__MINGW32__)
		Sys_QueEvent( SE_KEY, K_MOUSE4, 0, 0, nullptr, 0 );
#else
		int button = GET_XBUTTON_WPARAM( wParam );
		if( button == 1 )
		{
			Sys_QueEvent( SE_KEY, K_MOUSE4, 0, 0, nullptr, 0 );
		}
		else if( button == 2 )
		{
			Sys_QueEvent( SE_KEY, K_MOUSE5, 0, 0, nullptr, 0 );
		}
#endif
		// RB end
		return 0; // TODO: break?
	}
	case WM_MOUSEWHEEL:
	{
		int delta = GET_WHEEL_DELTA_WPARAM( wParam ) / WHEEL_DELTA;
		int key = delta < 0 ? K_MWHEELDOWN : K_MWHEELUP;
		delta = abs( delta );
		while( delta-- > 0 )
		{
			Sys_QueEvent( SE_KEY, key, true, 0, nullptr, 0 );
			Sys_QueEvent( SE_KEY, key, false, 0, nullptr, 0 );
		}
		break;
	}
	case WM_MOUSEMOVE:
	{
		if( !common->IsInitialized() )
		{
			break;
		}
		
		const bool isShellActive = ( game && ( game->Shell_IsActive() || game->IsPDAOpen() ) );
		const bool isConsoleActive = console->Active();
		
		if( win32.activeApp )
		{
			if( isShellActive )
			{
				// If the shell is active, it will display its own cursor.
				SetCursor( nullptr );
			}
			else if( isConsoleActive )
			{
				// The console is active but the shell is not.
				// Show the Windows cursor.
				SetCursor( LoadCursor( 0, IDC_ARROW ) );
			}
			else
			{
				// The shell not active and neither is the console.
				// This is normal gameplay, hide the cursor.
				SetCursor( nullptr );
			}
		}
		else
		{
			if( !isShellActive )
			{
				// Always show the cursor when the window is in the background
				SetCursor( LoadCursor( 0, IDC_ARROW ) );
			}
			else
			{
				SetCursor( nullptr );
			}
		}
		
		const int x = GET_X_LPARAM( lParam );
		const int y = GET_Y_LPARAM( lParam );
		
		// Generate an event
		Sys_QueEvent( SE_MOUSE_ABSOLUTE, x, y, 0, nullptr, 0 );
		
		// Get a mouse leave message
		TRACKMOUSEEVENT tme =
		{
			sizeof( TRACKMOUSEEVENT ),
			TME_LEAVE,
			hWnd,
			0
		};
		
		TrackMouseEvent( &tme );
		
		return 0; // TODO: break?
	}
	case WM_MOUSELEAVE:
	{
		Sys_QueEvent( SE_MOUSE_LEAVE, 0, 0, 0, nullptr, 0 );
		return 0; // TODO: break?
	}
	};
	
	return DefSubclassProc(hWnd, uMsg, wParam, lParam);
};