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

/// @file

#include "JoystickWin.hpp"

//namespace sbe
//{

/*
========================
idJoystickWin32::idJoystickWin32
========================
*/
idJoystickWin32::idJoystickWin32()
{
	numEvents = 0;
	memset( &events, 0, sizeof( events ) );
	memset( &controllers, 0, sizeof( controllers ) );
	memset( buttonStates, 0, sizeof( buttonStates ) );
	memset( joyAxis, 0, sizeof( joyAxis ) );
}

/*
========================
idJoystickWin32::Init
========================
*/
bool idJoystickWin32::Init()
{
	idJoystick::Init();
	
	// setup the timer that the high frequency thread will wait on
	// to fire every 4 msec
	timer = CreateWaitableTimer( nullptr, FALSE, "JoypadTimer" );
	LARGE_INTEGER dueTime;
	dueTime.QuadPart = -1;
	if( !SetWaitableTimer( timer, &dueTime, 4, nullptr, nullptr, FALSE ) )
	{
		idLib::FatalError( "SetWaitableTimer for joystick failed" );
	}
	
	// spawn the high frequency joystick reading thread
	Sys_CreateThread( ( xthread_t )JoystickSamplingThread, nullptr, THREAD_HIGHEST, "Joystick", CORE_1A );
	
	return false;
}

/*
========================
idJoystickWin32::SetRumble
========================
*/
void idJoystickWin32::SetRumble( int inputDeviceNum, int rumbleLow, int rumbleHigh )
{
	if( inputDeviceNum < 0 || inputDeviceNum >= MAX_JOYSTICKS )
	{
		return;
	}
	if( !controllers[inputDeviceNum].valid )
	{
		return;
	}
	XINPUT_VIBRATION vibration;
	vibration.wLeftMotorSpeed = idMath::ClampInt( 0, 65535, rumbleLow );
	vibration.wRightMotorSpeed = idMath::ClampInt( 0, 65535, rumbleHigh );
	DWORD err = XInputSetState( inputDeviceNum, &vibration );
	if( err != ERROR_SUCCESS )
	{
		idLib::Warning( "XInputSetState error: 0x%x", err );
	}
}

/*
========================
idJoystickWin32::PostInputEvent
========================
*/
void idJoystickWin32::PostInputEvent( int inputDeviceNum, int event, int value, int range )
{
	// These events are used for GUI button presses
	if( ( event >= J_ACTION1 ) && ( event <= J_ACTION_MAX ) )
	{
		PushButton( inputDeviceNum, K_JOY1 + ( event - J_ACTION1 ), value != 0 );
	}
	else if( event == J_AXIS_LEFT_X )
	{
		PushButton( inputDeviceNum, K_JOY_STICK1_LEFT, ( value < -range ) );
		PushButton( inputDeviceNum, K_JOY_STICK1_RIGHT, ( value > range ) );
	}
	else if( event == J_AXIS_LEFT_Y )
	{
		PushButton( inputDeviceNum, K_JOY_STICK1_UP, ( value < -range ) );
		PushButton( inputDeviceNum, K_JOY_STICK1_DOWN, ( value > range ) );
	}
	else if( event == J_AXIS_RIGHT_X )
	{
		PushButton( inputDeviceNum, K_JOY_STICK2_LEFT, ( value < -range ) );
		PushButton( inputDeviceNum, K_JOY_STICK2_RIGHT, ( value > range ) );
	}
	else if( event == J_AXIS_RIGHT_Y )
	{
		PushButton( inputDeviceNum, K_JOY_STICK2_UP, ( value < -range ) );
		PushButton( inputDeviceNum, K_JOY_STICK2_DOWN, ( value > range ) );
	}
	else if( ( event >= J_DPAD_UP ) && ( event <= J_DPAD_RIGHT ) )
	{
		PushButton( inputDeviceNum, K_JOY_DPAD_UP + ( event - J_DPAD_UP ), value != 0 );
	}
	else if( event == J_AXIS_LEFT_TRIG )
	{
		PushButton( inputDeviceNum, K_JOY_TRIGGER1, ( value > range ) );
	}
	else if( event == J_AXIS_RIGHT_TRIG )
	{
		PushButton( inputDeviceNum, K_JOY_TRIGGER2, ( value > range ) );
	}
	if( event >= J_AXIS_MIN && event <= J_AXIS_MAX )
	{
		int axis = event - J_AXIS_MIN;
		int percent = ( value * 16 ) / range;
		if( joyAxis[inputDeviceNum][axis] != percent )
		{
			joyAxis[inputDeviceNum][axis] = percent;
			Sys_QueEvent( SE_JOYSTICK, axis, percent, 0, nullptr, inputDeviceNum );
		}
	}
	
	// These events are used for actual game input
	events[numEvents].event = event;
	events[numEvents].value = value;
	numEvents++;
}

/*
========================
idJoystickWin32::PollInputEvents
========================
*/
int idJoystickWin32::PollInputEvents( int inputDeviceNum )
{
	numEvents = 0;
	
	if( !win32.activeApp )
	{
		return numEvents;
	}
	
	assert( inputDeviceNum < 4 );
	
//	if ( inputDeviceNum > in_joystick.GetInteger() ) {
//		return numEvents;
//	}

	controllerState_t* cs = &controllers[ inputDeviceNum ];
	
	// grab the current packet under a critical section
	XINPUT_STATE xis;
	XINPUT_STATE old;
	int		orBits;
	{
		idScopedCriticalSection crit( mutexXis );
		xis = cs->current;
		old = cs->previous;
		cs->previous = xis;
		// fetch or'd button bits
		orBits = cs->buttonBits;
		cs->buttonBits = 0;
	}
#if 0
	if( XInputGetState( inputDeviceNum, &xis ) != ERROR_SUCCESS )
	{
		return numEvents;
	}
#endif
	for( int i = 0 ; i < 32 ; i++ )
	{
		int	bit = 1 << i;
		
		if( ( ( xis.Gamepad.wButtons | old.Gamepad.wButtons ) & bit ) == 0
				&& ( orBits & bit ) )
		{
			idLib::Printf( "Dropped button press on bit %i\n", i );
		}
	}
	
	if( session->IsSystemUIShowing() )
	{
		// memset xis so the current input does not get latched if the UI is showing
		memset( &xis, 0, sizeof( XINPUT_STATE ) );
	}
	
	int joyRemap[16] =
	{
		J_DPAD_UP,		J_DPAD_DOWN,	// Up, Down
		J_DPAD_LEFT,	J_DPAD_RIGHT,	// Left, Right
		J_ACTION9,		J_ACTION10,		// Start, Back
		J_ACTION7,		J_ACTION8,		// Left Stick Down, Right Stick Down
		J_ACTION5,		J_ACTION6,		// Black, White (Left Shoulder, Right Shoulder)
		0,				0,				// Unused
		J_ACTION1,		J_ACTION2,		// A, B
		J_ACTION3,		J_ACTION4,		// X, Y
	};
	
	// Check the digital buttons
	for( int i = 0; i < 16; i++ )
	{
		int mask = ( 1 << i );
		if( ( xis.Gamepad.wButtons & mask ) != ( old.Gamepad.wButtons & mask ) )
		{
			PostInputEvent( inputDeviceNum, joyRemap[i], ( xis.Gamepad.wButtons & mask ) > 0 );
		}
	}
	
	// Check the triggers
	if( xis.Gamepad.bLeftTrigger != old.Gamepad.bLeftTrigger )
	{
		PostInputEvent( inputDeviceNum, J_AXIS_LEFT_TRIG, xis.Gamepad.bLeftTrigger * 128 );
	}
	if( xis.Gamepad.bRightTrigger != old.Gamepad.bRightTrigger )
	{
		PostInputEvent( inputDeviceNum, J_AXIS_RIGHT_TRIG, xis.Gamepad.bRightTrigger * 128 );
	}
	
	if( xis.Gamepad.sThumbLX != old.Gamepad.sThumbLX )
	{
		PostInputEvent( inputDeviceNum, J_AXIS_LEFT_X, xis.Gamepad.sThumbLX );
	}
	if( xis.Gamepad.sThumbLY != old.Gamepad.sThumbLY )
	{
		PostInputEvent( inputDeviceNum, J_AXIS_LEFT_Y, -xis.Gamepad.sThumbLY );
	}
	if( xis.Gamepad.sThumbRX != old.Gamepad.sThumbRX )
	{
		PostInputEvent( inputDeviceNum, J_AXIS_RIGHT_X, xis.Gamepad.sThumbRX );
	}
	if( xis.Gamepad.sThumbRY != old.Gamepad.sThumbRY )
	{
		PostInputEvent( inputDeviceNum, J_AXIS_RIGHT_Y, -xis.Gamepad.sThumbRY );
	}
	
	return numEvents;
}


/*
========================
idJoystickWin32::ReturnInputEvent
========================
*/
int idJoystickWin32::ReturnInputEvent( const int n, int& action, int& value )
{
	if( ( n < 0 ) || ( n >= MAX_JOY_EVENT ) )
	{
		return 0;
	}
	
	action = events[ n ].event;
	value = events[ n ].value;
	
	return 1;
}

/*
========================
idJoystickWin32::PushButton
========================
*/
void idJoystickWin32::PushButton( int inputDeviceNum, int key, bool value )
{
	// So we don't keep sending the same SE_KEY message over and over again
	if( buttonStates[inputDeviceNum][key] != value )
	{
		buttonStates[inputDeviceNum][key] = value;
		Sys_QueEvent( SE_KEY, key, value, 0, nullptr, inputDeviceNum );
	}
}

//}; // namespace sbe