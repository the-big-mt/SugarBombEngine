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

#pragma hdrstop
#include "precompiled.h"

#include "win_local.hpp"

#include "renderer/RenderCommon.h"

#include <windowsx.h>

//namespace BFG
//{

LONG WINAPI MainWndProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

static bool s_alttab_disabled;

extern idCVar r_windowX;
extern idCVar r_windowY;
extern idCVar r_windowWidth;
extern idCVar r_windowHeight;

static void WIN_DisableAltTab()
{
	if( s_alttab_disabled || win32.win_allowAltTab.GetBool() )
	{
		return;
	}
	if( !idStr::Icmp( cvarSystem->GetCVarString( "sys_arch" ), "winnt" ) )
	{
		RegisterHotKey( 0, 0, MOD_ALT, VK_TAB );
	}
	else
	{
		BOOL old;
		
		// RB begin
#if defined(__MINGW32__)
		SystemParametersInfo( SPI_GETSCREENSAVEACTIVE, 1, &old, 0 );
#else
		SystemParametersInfo( SPI_SCREENSAVERRUNNING, 1, &old, 0 );
#endif
		// RB end
	}
	s_alttab_disabled = true;
}

static void WIN_EnableAltTab()
{
	if( !s_alttab_disabled || win32.win_allowAltTab.GetBool() )
	{
		return;
	}
	if( !idStr::Icmp( cvarSystem->GetCVarString( "sys_arch" ), "winnt" ) )
	{
		UnregisterHotKey( 0, 0 );
	}
	else
	{
		BOOL old;
		
		// RB begin
#if defined(__MINGW32__)
		SystemParametersInfo( SPI_GETSCREENSAVEACTIVE, 1, &old, 0 );
#else
		SystemParametersInfo( SPI_SCREENSAVERRUNNING, 1, &old, 0 );
#endif
		// RB end
	}
	
	s_alttab_disabled = false;
}

void WIN_Sizing( WORD side, RECT* rect )
{
	if( !R_IsInitialized() || renderSystem->GetWidth() <= 0 || renderSystem->GetHeight() <= 0 )
	{
		return;
	}
	
	// restrict to a standard aspect ratio
	int width = rect->right - rect->left;
	int height = rect->bottom - rect->top;
	
	// Adjust width/height for window decoration
	RECT decoRect = { 0, 0, 0, 0 };
	AdjustWindowRect( &decoRect, WINDOW_STYLE | WS_SYSMENU, FALSE );
	int decoWidth = decoRect.right - decoRect.left;
	int decoHeight = decoRect.bottom - decoRect.top;
	
	width -= decoWidth;
	height -= decoHeight;
	
	// Clamp to a minimum size
	if( width < SCREEN_WIDTH / 4 )
	{
		width = SCREEN_WIDTH / 4;
	}
	if( height < SCREEN_HEIGHT / 4 )
	{
		height = SCREEN_HEIGHT / 4;
	}
	
	const int minWidth = height * 4 / 3;
	const int maxHeight = width * 3 / 4;
	
	const int maxWidth = height * 16 / 9;
	const int minHeight = width * 9 / 16;
	
	// Set the new size
	switch( side )
	{
		case WMSZ_LEFT:
			rect->left = rect->right - width - decoWidth;
			rect->bottom = rect->top + idMath::ClampInt( minHeight, maxHeight, height ) + decoHeight;
			break;
		case WMSZ_RIGHT:
			rect->right = rect->left + width + decoWidth;
			rect->bottom = rect->top + idMath::ClampInt( minHeight, maxHeight, height ) + decoHeight;
			break;
		case WMSZ_BOTTOM:
		case WMSZ_BOTTOMRIGHT:
			rect->bottom = rect->top + height + decoHeight;
			rect->right = rect->left + idMath::ClampInt( minWidth, maxWidth, width ) + decoWidth;
			break;
		case WMSZ_TOP:
		case WMSZ_TOPRIGHT:
			rect->top = rect->bottom - height - decoHeight;
			rect->right = rect->left + idMath::ClampInt( minWidth, maxWidth, width ) + decoWidth;
			break;
		case WMSZ_BOTTOMLEFT:
			rect->bottom = rect->top + height + decoHeight;
			rect->left = rect->right - idMath::ClampInt( minWidth, maxWidth, width ) - decoWidth;
			break;
		case WMSZ_TOPLEFT:
			rect->top = rect->bottom - height - decoHeight;
			rect->left = rect->right - idMath::ClampInt( minWidth, maxWidth, width ) - decoWidth;
			break;
	}
}

/*
====================
MainWndProc

main window procedure
====================
*/
LONG WINAPI MainWndProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	int key;
	switch( uMsg )
	{
		case WM_WINDOWPOSCHANGED:
			if( R_IsInitialized() )
			{
				RECT rect;
				if( ::GetClientRect( win32.hWnd, &rect ) )
				{
				
					if( rect.right > rect.left && rect.bottom > rect.top )
					{
						glConfig.nativeScreenWidth = rect.right - rect.left;
						glConfig.nativeScreenHeight = rect.bottom - rect.top;
						
						// save the window size in cvars if we aren't fullscreen
						int style = GetWindowLong( hWnd, GWL_STYLE );
						if( ( style & WS_POPUP ) == 0 )
						{
							r_windowWidth.SetInteger( glConfig.nativeScreenWidth );
							r_windowHeight.SetInteger( glConfig.nativeScreenHeight );
						}
					}
				}
			}
			break;
		case WM_MOVE:
		{
			int		xPos, yPos;
			RECT r;
			
			// save the window origin in cvars if we aren't fullscreen
			int style = GetWindowLong( hWnd, GWL_STYLE );
			if( ( style & WS_POPUP ) == 0 )
			{
				xPos = ( short ) LOWORD( lParam ); // horizontal position
				yPos = ( short ) HIWORD( lParam ); // vertical position
				
				r.left   = 0;
				r.top    = 0;
				r.right  = 1;
				r.bottom = 1;
				
				AdjustWindowRect( &r, style, FALSE );
				
				r_windowX.SetInteger( xPos + r.left );
				r_windowY.SetInteger( yPos + r.top );
			}
			break;
		}
		case WM_CREATE:
		
			win32.hWnd = hWnd;
			
			if( win32.cdsFullscreen )
			{
				WIN_DisableAltTab();
			}
			else
			{
				WIN_EnableAltTab();
			}
			
			// do the OpenGL setup
			void GLW_WM_CREATE( HWND hWnd );
			GLW_WM_CREATE( hWnd );
			
			break;
			
		case WM_DESTROY:
			// let sound and input know about this?
			win32.hWnd = nullptr;
			if( win32.cdsFullscreen )
			{
				WIN_EnableAltTab();
			}
			break;
			
		case WM_CLOSE:
			soundSystem->SetMute( true );
			cmdSystem->BufferCommandText( CMD_EXEC_APPEND, "quit\n" );
			break;
			
		case WM_ACTIVATE:
			// if we got here because of an alt-tab or maximize,
			// we should activate immediately.  If we are here because
			// the mouse was clicked on a title bar or drag control,
			// don't activate until the mouse button is released
		{
			int	fActive, fMinimized;
			
			fActive = LOWORD( wParam );
			fMinimized = ( BOOL ) HIWORD( wParam );
			
			win32.activeApp = ( fActive != WA_INACTIVE );
			if( win32.activeApp )
			{
				idKeyInput::ClearStates();
				Sys_GrabMouseCursor( true );
				if( common->IsInitialized() )
				{
					SetCursor( nullptr );
				}
			}
			
			if( fActive == WA_INACTIVE )
			{
				win32.movingWindow = false;
				if( common->IsInitialized() )
				{
					SetCursor( LoadCursor( 0, IDC_ARROW ) );
				}
			}
			
			// start playing the game sound world
			soundSystem->SetMute( !win32.activeApp );
			// DG: set com_pause so game pauses when focus is lost
			// and continues when focus is regained
			cvarSystem->SetCVarBool( "com_pause", !win32.activeApp );
			// DG end
			
			// we do not actually grab or release the mouse here,
			// that will be done next time through the main loop
		}
		break;
		case WM_TIMER:
		{
			if( win32.win_timerUpdate.GetBool() )
			{
				common->Frame();
			}
			break;
		}
		case WM_SYSCOMMAND:
			if( wParam == SC_SCREENSAVE || wParam == SC_KEYMENU )
			{
				return 0;
			}
			break;
		
		case WM_NCLBUTTONDOWN:
//			win32.movingWindow = true;
			break;
			
		case WM_ENTERSIZEMOVE:
			win32.movingWindow = true;
			break;
			
		case WM_EXITSIZEMOVE:
			win32.movingWindow = false;
			break;
			
		case WM_SIZING:
			WIN_Sizing( wParam, ( RECT* )lParam );
			break;
		
	}
	
	return DefWindowProc( hWnd, uMsg, wParam, lParam );
}

//} // namespace BFG