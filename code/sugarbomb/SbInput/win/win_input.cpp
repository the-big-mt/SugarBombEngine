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
//#include <cstring> // for memset

#include "../sys_session_local.h"

#include "win_local.h"

//namespace BFG
//{

/*
==========================
IN_DeactivateMouseIfWindowed
==========================
*/
void IN_DeactivateMouseIfWindowed()
{
	if( !win32.cdsFullscreen )
		IN_DeactivateMouse();
}

//=====================================================================================

static DIDEVICEOBJECTDATA polled_didod[ DINPUT_BUFFERSIZE ];  // Receives buffered data

/*
========================
JoystickSamplingThread
========================
*/
static int	threadTimeDeltas[256];
static int	threadPacket[256];
static int	threadCount;
void JoystickSamplingThread( void* data )
{
	static int prevTime = 0;
	static uint64 nextCheck[MAX_JOYSTICKS] = { 0 };
	const uint64 waitTime = 5000000; // poll every 5 seconds to see if a controller was connected
	while( 1 )
	{
		// hopefully we see close to 4000 usec each loop
		int	now = Sys_Microseconds();
		int	delta;
		if( prevTime == 0 )
		{
			delta = 4000;
		}
		else
		{
			delta = now - prevTime;
		}
		prevTime = now;
		threadTimeDeltas[threadCount & 255] = delta;
		threadCount++;
		
		{
			XINPUT_STATE	joyData[MAX_JOYSTICKS];
			bool			validData[MAX_JOYSTICKS];
			for( int i = 0 ; i < MAX_JOYSTICKS ; i++ )
			{
				if( now >= nextCheck[i] )
				{
					// XInputGetState might block... for a _really_ long time..
					validData[i] = XInputGetState( i, &joyData[i] ) == ERROR_SUCCESS;
					
					// allow an immediate data poll if the input device is connected else
					// wait for some time to see if another device was reconnected.
					// Checking input state infrequently for newly connected devices prevents
					// severe slowdowns on PC, especially on WinXP64.
					if( validData[i] )
					{
						nextCheck[i] = 0;
					}
					else
					{
						nextCheck[i] = now + waitTime;
					}
				}
			}
			
			// do this short amount of processing inside a critical section
			idScopedCriticalSection cs( win32.g_Joystick.mutexXis );
			
			for( int i = 0 ; i < MAX_JOYSTICKS ; i++ )
			{
				controllerState_t* cs = &win32.g_Joystick.controllers[i];
				
				if( !validData[i] )
				{
					cs->valid = false;
					continue;
				}
				cs->valid = true;
				
				XINPUT_STATE& current = joyData[i];
				
				cs->current = current;
				
				// Switch from using cs->current to current to reduce chance of Load-Hit-Store on consoles
				
				threadPacket[threadCount & 255] = current.dwPacketNumber;
#if 0
				if( xis.dwPacketNumber == oldXis[ inputDeviceNum ].dwPacketNumber )
				{
					return numEvents;
				}
#endif
				cs->buttonBits |= current.Gamepad.wButtons;
			}
		}
		
		// we want this to be processed at least 250 times a second
		WaitForSingleObject( win32.g_Joystick.timer, INFINITE );
	}
}

//} // namespace BFG