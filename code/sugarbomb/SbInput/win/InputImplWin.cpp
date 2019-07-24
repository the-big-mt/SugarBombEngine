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

//#pragma hdrstop
//#include "precompiled.h"

#include "InputImplWin.hpp"

//#include "framework/sys_session_local.h"

//#include "win_local.h"

#include "idlib/sys/sys_assert.h"
#include "idlib/sys/sys_defines.h"

#include "sys/ISys.hpp"

#include "framework/IUsercmdGen.hpp"

//namespace sbe
//{

idCVar SbInputImplWin::in_mouse( "in_mouse", "1", CVAR_SYSTEM | CVAR_BOOL, "enable mouse input" );

constexpr auto DINPUT_BUFFERSIZE{256};

static DIDEVICEOBJECTDATA polled_didod[DINPUT_BUFFERSIZE]; // Receives buffered data

sbe::IInputSystem *CreateInputSystem(sbe::ISys *apSys, void *apWnd)
{
	static SbInputImplWin InputSystem(apSys, nullptr, reinterpret_cast<HWND>(apWnd)); // TODO: usercmdGen
	return &InputSystem;
};

SbInputImplWin::SbInputImplWin(sbe::ISys *apSys, idUsercmdGen *apUserCmdGen, HWND ahWnd)
	: mpSys(apSys), usercmdGen(apUserCmdGen), mhWnd(ahWnd){}

/*
===========
Sys_InitInput
===========
*/
void SbInputImplWin::Init()
{
	mpSys->Printf( "\n------- Input Initialization -------\n" );
	InitDirectInput();

	if( SbInputImplWin::in_mouse.GetBool() )
	{
		InitDIMouse();
		// don't grab the mouse on initialization
		GrabMouseCursor( false );
	}
	else
		mpSys->Printf( "Mouse control not active.\n" );

	StartupKeyboard();
	
	mpSys->Printf( "------------------------------------\n" );
	SbInputImplWin::in_mouse.ClearModified();
	
	g_Joystick.Init();
};

/*
===========
Sys_ShutdownInput
===========
*/
void SbInputImplWin::Shutdown()
{
	DeactivateMouse();
	DeactivateKeyboard();

	if( g_pKeyboard )
	{
		g_pKeyboard->Release();
		g_pKeyboard = nullptr;
	};
	
	if( g_pMouse )
	{
		g_pMouse->Release();
		g_pMouse = nullptr;
	};
	
	if( g_pdi )
	{
		g_pdi->Release();
		g_pdi = nullptr;
	};
};

const char *SbInputImplWin::GetKeyName(keyNum_t keynum) const
{
	return "TODO";
};

#if 1
// I tried doing the full-state get to address a keyboard problem on one system,
// but it didn't make any difference

/*
====================
Sys_PollKeyboardInputEvents
====================
*/
int SbInputImplWin::PollKeyboardInputEvents()
{
	DWORD              dwElements;
	HRESULT            hr;
	
	if( g_pKeyboard == nullptr )
	{
		return 0;
	}
	
	dwElements = DINPUT_BUFFERSIZE;
	hr = g_pKeyboard->GetDeviceData( sizeof( DIDEVICEOBJECTDATA ),
										   polled_didod, &dwElements, 0 );
	if( hr != DI_OK )
	{
		// We got an error or we got DI_BUFFEROVERFLOW.
		//
		// Either way, it means that continuous contact with the
		// device has been lost, either due to an external
		// interruption, or because the buffer overflowed
		// and some events were lost.
		hr = g_pKeyboard->Acquire();
		
		
		
		// nuke the garbage
		if( !FAILED( hr ) )
		{
			//Bug 951: The following command really clears the garbage input.
			//The original will still process keys in the buffer and was causing
			//some problems.
			g_pKeyboard->GetDeviceData( sizeof( DIDEVICEOBJECTDATA ), nullptr, &dwElements, 0 );
			dwElements = 0;
		}
		// hr may be DIERR_OTHERAPPHASPRIO or other errors.  This
		// may occur when the app is minimized or in the process of
		// switching, so just try again later
	}
	
	if( FAILED( hr ) )
	{
		return 0;
	}
	
	return dwElements;
}

#else

/*
====================
Sys_PollKeyboardInputEvents

Fake events by getting the entire device state
and checking transitions
====================
*/
int SbInputImplWin::PollKeyboardInputEvents()
{
	static int diFetch;
	static byte toggleFetch[2][ 256 ];
	
	HRESULT            hr;
	
	if( g_pKeyboard == nullptr )
	{
		return 0;
	}
	
	hr = g_pKeyboard->GetDeviceState( sizeof( toggleFetch[ diFetch ] ), toggleFetch[ diFetch ] );
	if( hr != DI_OK )
	{
		// We got an error or we got DI_BUFFEROVERFLOW.
		//
		// Either way, it means that continuous contact with the
		// device has been lost, either due to an external
		// interruption, or because the buffer overflowed
		// and some events were lost.
		hr = g_pKeyboard->Acquire();
		
		// nuke the garbage
		if( !FAILED( hr ) )
		{
			hr = g_pKeyboard->GetDeviceState( sizeof( toggleFetch[ diFetch ] ), toggleFetch[ diFetch ] );
		}
		// hr may be DIERR_OTHERAPPHASPRIO or other errors.  This
		// may occur when the app is minimized or in the process of
		// switching, so just try again later
	}
	
	if( FAILED( hr ) )
	{
		return 0;
	}
	
	// build faked events
	int		numChanges = 0;
	
	for( int i = 0 ; i < 256 ; i++ )
	{
		if( toggleFetch[0][i] != toggleFetch[1][i] )
		{
			polled_didod[ numChanges ].dwOfs = i;
			polled_didod[ numChanges ].dwData = toggleFetch[ diFetch ][i] ? 0x80 : 0;
			numChanges++;
		}
	}
	
	diFetch ^= 1;
	
	return numChanges;
}

#endif

/*
====================
Sys_PollKeyboardInputEvents
====================
*/
int SbInputImplWin::ReturnKeyboardInputEvent( const int n, int& ch, bool& state )
{
	ch = polled_didod[ n ].dwOfs;
	state = ( polled_didod[ n ].dwData & 0x80 ) == 0x80;
	if( ch == K_PRINTSCREEN || ch == K_LCTRL || ch == K_LALT || ch == K_RCTRL || ch == K_RALT )
	{
		// for windows, add a keydown event for print screen here, since
		// windows doesn't send keydown events to the WndProc for this key.
		// ctrl and alt are handled here to get around windows sending ctrl and
		// alt messages when the right-alt is pressed on non-US 102 keyboards.
		Sys_QueEvent( SE_KEY, ch, state, 0, nullptr, 0 );
	};
	return ch;
};

void SbInputImplWin::EndKeyboardInputEvents()
{
};

//=====================================================================================

int SbInputImplWin::PollMouseInputEvents( int mouseEvents[sbe::MAX_MOUSE_EVENTS][2] )
{
	DWORD				dwElements;
	HRESULT				hr;
	
	if( !g_pMouse || !mouseGrabbed )
	{
		return 0;
	}
	
	dwElements = DINPUT_BUFFERSIZE;
	hr = g_pMouse->GetDeviceData( sizeof( DIDEVICEOBJECTDATA ), polled_didod, &dwElements, 0 );
	
	if( hr != DI_OK )
	{
		hr = g_pMouse->Acquire();
		// clear the garbage
		if( !FAILED( hr ) )
		{
			g_pMouse->GetDeviceData( sizeof( DIDEVICEOBJECTDATA ), polled_didod, &dwElements, 0 );
		}
	}
	
	if( FAILED( hr ) )
	{
		return 0;
	}
	
	if( dwElements > sbe::MAX_MOUSE_EVENTS )
	{
		dwElements = sbe::MAX_MOUSE_EVENTS;
	}
	
	for( DWORD i = 0; i < dwElements; i++ )
	{
		mouseEvents[i][0] = M_INVALID;
		mouseEvents[i][1] = 0;
		
		if( polled_didod[i].dwOfs >= DIMOFS_BUTTON0 && polled_didod[i].dwOfs <= DIMOFS_BUTTON7 )
		{
			const int mouseButton = ( polled_didod[i].dwOfs - DIMOFS_BUTTON0 );
			const bool mouseDown = ( polled_didod[i].dwData & 0x80 ) == 0x80;
			mouseEvents[i][0] = M_ACTION1 + mouseButton;
			mouseEvents[i][1] = mouseDown;
			Sys_QueEvent( SE_KEY, K_MOUSE1 + mouseButton, mouseDown, 0, nullptr, 0 );
		}
		else
		{
			// RB: replaced switch enum for MinGW
			int diaction = polled_didod[i].dwOfs;
			
			if( diaction == DIMOFS_X )
			{
				mouseEvents[i][0] = M_DELTAX;
				mouseEvents[i][1] = polled_didod[i].dwData;
				Sys_QueEvent( SE_MOUSE, polled_didod[i].dwData, 0, 0, nullptr, 0 );
			}
			else if( diaction == DIMOFS_Y )
			{
				mouseEvents[i][0] = M_DELTAY;
				mouseEvents[i][1] = polled_didod[i].dwData;
				Sys_QueEvent( SE_MOUSE, 0, polled_didod[i].dwData, 0, nullptr, 0 );
			}
			else if( diaction == DIMOFS_Z )
			{
				mouseEvents[i][0] = M_DELTAZ;
				mouseEvents[i][1] = ( int )polled_didod[i].dwData / WHEEL_DELTA;
				{
					const int value = ( int )polled_didod[i].dwData / WHEEL_DELTA;
					const int key = value < 0 ? K_MWHEELDOWN : K_MWHEELUP;
					const int iterations = abs( value );
					for( int i = 0; i < iterations; i++ )
					{
						Sys_QueEvent( SE_KEY, key, true, 0, nullptr, 0 );
						Sys_QueEvent( SE_KEY, key, false, 0, nullptr, 0 );
					}
				}
			}
			// RB end
		}
	}
	
	return dwElements;
};

//=====================================================================================
//	Joystick Input Handling
//=====================================================================================

void SbInputImplWin::SetRumble( int device, int low, int hi )
{
	return g_Joystick.SetRumble( device, low, hi );
};

int SbInputImplWin::PollJoystickInputEvents( int deviceNum )
{
	return g_Joystick.PollInputEvents( deviceNum );
};

int SbInputImplWin::ReturnJoystickInputEvent( const int n, int& action, int& value )
{
	return g_Joystick.ReturnInputEvent( n, action, value );
};

void SbInputImplWin::EndJoystickInputEvents()
{
};

/*
==================
IN_Frame

Called every frame, even if not generating commands
==================
*/
void SbInputImplWin::Frame()
{
	bool shouldGrab = true;
	
	if( !SbInputImplWin::in_mouse.GetBool() )
		shouldGrab = false;

	// if fullscreen, we always want the mouse
	if( !win32.cdsFullscreen )
	{
		if( mouseReleased )
			shouldGrab = false;

		if( win32.movingWindow )
			shouldGrab = false;

		if( !win32.activeApp )
			shouldGrab = false;
	};
	
	if( shouldGrab != mouseGrabbed )
	{
		if( usercmdGen != nullptr )
			usercmdGen->Clear();
		
		if( mouseGrabbed )
			DeactivateMouse();
		else
		{
			ActivateMouse();
			
#if 0	// if we can't reacquire, try reinitializing
			if( !InitDIMouse() )
			{
				SbInputImplWin::in_mouse.SetBool( false );
				return;
			};
#endif
		};
	};
};

/*
============================================================

  MOUSE CONTROL

============================================================
*/

void SbInputImplWin::GrabMouseCursor(bool grabIt)
{
	mouseReleased = !grabIt;

	if( !grabIt )
	{
		// release it right now
		Frame();
	};
};

/*
============================================================

DIRECT INPUT MOUSE CONTROL

============================================================
*/

/*
========================
IN_InitDirectInput
========================
*/
void SbInputImplWin::InitDirectInput()
{
	HRESULT		hr;
	
	mpSys->Printf( "Initializing DirectInput...\n" );
	
	if( g_pdi != nullptr )
	{
		g_pdi->Release();			// if the previous window was destroyed we need to do this
		g_pdi = nullptr;
	};
	
	// Register with the DirectInput subsystem and get a pointer
	// to a IDirectInput interface we can use.
	// Create the base DirectInput object
	if( FAILED( hr = DirectInput8Create( GetModuleHandle( nullptr ), DIRECTINPUT_VERSION, IID_IDirectInput8, ( void** )&g_pdi, nullptr ) ) )
		mpSys->Printf( "DirectInputCreate failed\n" );
};

/*
========================
IN_InitDIMouse
========================
*/
bool SbInputImplWin::InitDIMouse()
{
	HRESULT		hr;
	
	if( g_pdi == nullptr )
		return false;
	
	// obtain an interface to the system mouse device.
	hr = g_pdi->CreateDevice( GUID_SysMouse, &g_pMouse, nullptr );
	
	if( FAILED( hr ) )
	{
		mpSys->Printf( "mouse: Couldn't open DI mouse device\n" );
		return false;
	};
	
	// Set the data format to "mouse format" - a predefined data format
	//
	// A data format specifies which controls on a device we
	// are interested in, and how they should be reported.
	//
	// This tells DirectInput that we will be passing a
	// DIMOUSESTATE2 structure to IDirectInputDevice::GetDeviceState.
	if( FAILED( hr = g_pMouse->SetDataFormat( &c_dfDIMouse2 ) ) )
	{
		mpSys->Printf( "mouse: Couldn't set DI mouse format\n" );
		return false;
	};
	
	// set the cooperativity level.
	hr = g_pMouse->SetCooperativeLevel( mhWnd, DISCL_EXCLUSIVE | DISCL_FOREGROUND );
	
	if( FAILED( hr ) )
	{
		mpSys->Printf( "mouse: Couldn't set DI coop level\n" );
		return false;
	};
	
	// IMPORTANT STEP TO USE BUFFERED DEVICE DATA!
	//
	// DirectInput uses unbuffered I/O (buffer size = 0) by default.
	// If you want to read buffered data, you need to set a nonzero
	// buffer size.
	//
	// Set the buffer size to SAMPLE_BUFFER_SIZE (defined above) elements.
	//
	// The buffer size is a DWORD property associated with the device.
	DIPROPDWORD dipdw;
	dipdw.diph.dwSize       = sizeof( DIPROPDWORD );
	dipdw.diph.dwHeaderSize = sizeof( DIPROPHEADER );
	dipdw.diph.dwObj        = 0;
	dipdw.diph.dwHow        = DIPH_DEVICE;
	dipdw.dwData            = DINPUT_BUFFERSIZE; // Arbitary buffer size
	
	if( FAILED( hr = g_pMouse->SetProperty( DIPROP_BUFFERSIZE, &dipdw.diph ) ) )
	{
		mpSys->Printf( "mouse: Couldn't set DI buffersize\n" );
		return false;
	};
	
	ActivateMouse();
	
	// clear any pending samples
	int	mouseEvents[sbe::MAX_MOUSE_EVENTS][2];
	Sys_PollMouseInputEvents( mouseEvents );
	
	mpSys->Printf( "mouse: DirectInput initialized.\n" );
	return true;
};

/*
==========================
IN_ActivateMouse
==========================
*/
void SbInputImplWin::ActivateMouse()
{
	int i;
	HRESULT hr;
	
	if( !SbInputImplWin::in_mouse.GetBool() || mouseGrabbed || !g_pMouse )
		return;
	
	mouseGrabbed = true;
	for( i = 0; i < 10; i++ )
	{
		if( ::ShowCursor( false ) < 0 )
			break;
	};
	
	// we may fail to reacquire if the window has been recreated
	hr = g_pMouse->Acquire();
	if( FAILED( hr ) )
		return;
	
	// set the cooperativity level.
	hr = g_pMouse->SetCooperativeLevel( mhWnd, DISCL_EXCLUSIVE | DISCL_FOREGROUND );
};

/*
==========================
IN_DeactivateMouse
==========================
*/
void SbInputImplWin::DeactivateMouse()
{
	int i;
	
	if( !g_pMouse || !mouseGrabbed )
		return;
	
	g_pMouse->Unacquire();
	
	for( i = 0; i < 10; i++ )
	{
		if( ::ShowCursor( true ) >= 0 )
			break;
	};

	mouseGrabbed = false;
};

/*
============================================================

DIRECT INPUT KEYBOARD CONTROL

============================================================
*/

bool SbInputImplWin::StartupKeyboard()
{
	HRESULT hr;
	bool    bExclusive;
	bool    bForeground;
	bool    bImmediate;
	bool    bDisableWindowsKey;
	DWORD   dwCoopFlags;
	
	if( !g_pdi )
	{
		mpSys->Printf( "keyboard: DirectInput has not been started\n" );
		return false;
	};
	
	if( g_pKeyboard )
	{
		g_pKeyboard->Release();
		g_pKeyboard = nullptr;
	};
	
	// Detrimine where the buffer would like to be allocated
	bExclusive         = false;
	bForeground        = true;
	bImmediate         = false;
	bDisableWindowsKey = true;
	
	if( bExclusive )
		dwCoopFlags = DISCL_EXCLUSIVE;
	else
		dwCoopFlags = DISCL_NONEXCLUSIVE;
		
	if( bForeground )
		dwCoopFlags |= DISCL_FOREGROUND;
	else
		dwCoopFlags |= DISCL_BACKGROUND;
		
	// Disabling the windows key is only allowed only if we are in foreground nonexclusive
	if( bDisableWindowsKey && !bExclusive && bForeground )
		dwCoopFlags |= DISCL_NOWINKEY;
		
	// Obtain an interface to the system keyboard device.
	if( FAILED( hr = g_pdi->CreateDevice( GUID_SysKeyboard, &g_pKeyboard, nullptr ) ) )
	{
		mpSys->Printf( "keyboard: couldn't find a keyboard device\n" );
		return false;
	};
	
	// Set the data format to "keyboard format" - a predefined data format
	//
	// A data format specifies which controls on a device we
	// are interested in, and how they should be reported.
	//
	// This tells DirectInput that we will be passing an array
	// of 256 bytes to IDirectInputDevice::GetDeviceState.
	if( FAILED( hr = g_pKeyboard->SetDataFormat( &c_dfDIKeyboard ) ) )
		return false;
		
	// Set the cooperativity level to let DirectInput know how
	// this device should interact with the system and with other
	// DirectInput applications.
	hr = g_pKeyboard->SetCooperativeLevel( mhWnd, dwCoopFlags );
	if( hr == DIERR_UNSUPPORTED && !bForeground && bExclusive )
	{
		mpSys->Printf( "keyboard: SetCooperativeLevel() returned DIERR_UNSUPPORTED.\nFor security reasons, background exclusive keyboard access is not allowed.\n" );
		return false;
	};
	
	if( FAILED( hr ) )
		return false;
	
	if( !bImmediate )
	{
		// IMPORTANT STEP TO USE BUFFERED DEVICE DATA!
		//
		// DirectInput uses unbuffered I/O (buffer size = 0) by default.
		// If you want to read buffered data, you need to set a nonzero
		// buffer size.
		//
		// Set the buffer size to DINPUT_BUFFERSIZE (defined above) elements.
		//
		// The buffer size is a DWORD property associated with the device.
		DIPROPDWORD dipdw;
		
		dipdw.diph.dwSize       = sizeof( DIPROPDWORD );
		dipdw.diph.dwHeaderSize = sizeof( DIPROPHEADER );
		dipdw.diph.dwObj        = 0;
		dipdw.diph.dwHow        = DIPH_DEVICE;
		dipdw.dwData            = DINPUT_BUFFERSIZE; // Arbitary buffer size
		
		if( FAILED( hr = g_pKeyboard->SetProperty( DIPROP_BUFFERSIZE, &dipdw.diph ) ) )
			return false;
	};
	
	// Acquire the newly created device
	g_pKeyboard->Acquire();
	
	mpSys->Printf( "keyboard: DirectInput initialized.\n" );
	return true;
};

/*
==========================
IN_DeactivateKeyboard
==========================
*/
void SbInputImplWin::DeactivateKeyboard()
{
	if( !g_pKeyboard )
		return;

	g_pKeyboard->Unacquire( );
};

//}; // namespace sbe