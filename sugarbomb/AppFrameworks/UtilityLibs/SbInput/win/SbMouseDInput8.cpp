/*
*******************************************************************************

Copyright (C) 1993-2012 id Software LLC, a ZeniMax Media company.
Copyright (C) 2012 Robert Beckebans
Copyright (C) 2020 SugarBombEngine Developers

This file is part of SugarBombEngine

SugarBombEngine is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

SugarBombEngine is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with SugarBombEngine. If not, see <http://www.gnu.org/licenses/>.

In addition, SugarBombEngine is using id Tech 4 (BFG) pieces and thus subject to certain additional terms (all header and source files which 
contains such pieces has this additional part appended to the license header). You should have received a copy of these additional terms 
stated in a separate file (LICENSE-idTech4) which accompanied the SugarBombEngine source code. If not, please request a copy in 
writing from id Software at the address below.

If you have questions concerning this license or the applicable additional terms, you may contact in writing id Software LLC, c/o ZeniMax Media Inc.,
Suite 120, Rockville, Maryland 20850 USA.

*******************************************************************************
*/

/// @file

//*****************************************************************************

#include <dinput.h>

#include "SbMouseDInput8.hpp"

#include "CoreLibs/SbSystem/ISystem.hpp"

//*****************************************************************************

namespace sbe::SbInput
{

/*
========================
IN_InitDIMouse
========================
*/
SbMouseDInput8::SbMouseDInput8(SbInputWin &aInputImpl, ISystem &aSystem, IDirectInput8 *apDInput) : mSystem(aSystem)
{
	if(apDInput == nullptr)
		return; //false;
	
	// obtain an interface to the system mouse device.
	HRESULT hr{apDInput->CreateDevice(GUID_SysMouse, &mpDevice, nullptr)};
	if(FAILED(hr))
	{
		mSystem.Printf("mouse: Couldn't open DI mouse device\n");
		return; //false;
	};
	
	// Set the data format to "mouse format" - a predefined data format
	//
	// A data format specifies which controls on a device we
	// are interested in, and how they should be reported.
	//
	// This tells DirectInput that we will be passing a
	// DIMOUSESTATE2 structure to IDirectInputDevice::GetDeviceState.
	hr = mpDevice->SetDataFormat(&c_dfDIMouse2);
	if(FAILED(hr))
	{
		mSystem.Printf("mouse: Couldn't set DI mouse format\n");
		return; //false;
	};
	
	// set the cooperativity level.
	hr = mpDevice->SetCooperativeLevel(mhWnd, DISCL_EXCLUSIVE | DISCL_FOREGROUND);
	
	if(FAILED(hr))
	{
		mSystem.Printf("mouse: Couldn't set DI coop level\n");
		return; //false;
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
	DIPROPDWORD dipdw{};
	dipdw.diph.dwSize       = sizeof( DIPROPDWORD );
	dipdw.diph.dwHeaderSize = sizeof( DIPROPHEADER );
	dipdw.diph.dwObj        = 0;
	dipdw.diph.dwHow        = DIPH_DEVICE;
	dipdw.dwData            = DINPUT_BUFFERSIZE; // Arbitary buffer size
	
	hr = mpDevice->SetProperty(DIPROP_BUFFERSIZE, &dipdw.diph);
	if(FAILED(hr))
	{
		mSystem.Printf("mouse: Couldn't set DI buffersize\n");
		return; //false;
	};
	
	Activate();
	
	// clear any pending samples
	int	mouseEvents[sbe::MAX_MOUSE_EVENTS][2];
	Poll(mouseEvents);
	
	mSystem.Printf("mouse: DirectInput initialized.\n");
	//return true;
};

/*
void SbMouseDInput8::Update()
{
};
*/

int SbMouseDInput8::Poll(int anEvents[MAX_MOUSE_EVENTS][2])
{
	if(!mpDevice /*|| !mouseGrabbed*/)
		return 0;
	
	DWORD dwElements{DINPUT_BUFFERSIZE};
	HRESULT hr{mpDevice->GetDeviceData(sizeof( DIDEVICEOBJECTDATA ), polled_didod, &dwElements, 0)};
	
	if(hr != DI_OK)
	{
		hr = mpDevice->Acquire();
		// clear the garbage
		if(!FAILED(hr))
			mpDevice->GetDeviceData(sizeof( DIDEVICEOBJECTDATA ), polled_didod, &dwElements, 0);
	};
	
	if(FAILED(hr))
		return 0;
	
	if(dwElements > MAX_MOUSE_EVENTS)
		dwElements = MAX_MOUSE_EVENTS;
	
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
					};
				};
			};
			// RB end
		};
	};
	
	return dwElements;
};

/*
==========================
IN_ActivateMouse
==========================
*/
void SbMouseDInput8::Activate()
{
	//if( !SbInputImplWin::in_mouse.GetBool() || mouseGrabbed || !g_pMouse ) // TODO
		//return; // TODO
	
	//mouseGrabbed = true; // TODO
	
	for(int i = 0; i < 10; i++)
	{
		if(::ShowCursor(false) < 0)
			break;
	};
	
	// we may fail to reacquire if the window has been recreated
	HRESULT hr = mpDevice->Acquire();
	if(FAILED(hr))
		return;
	
	// set the cooperativity level.
	//hr = mpDevice->SetCooperativeLevel(mhWnd, DISCL_EXCLUSIVE | DISCL_FOREGROUND); // TODO
};

/*
==========================
IN_DeactivateMouse
==========================
*/
void SbMouseDInput8::Deactivate()
{
	if(!mpDevice /*|| !mouseGrabbed*/) // TODO
		return;
	
	mpDevice->Unacquire();
	
	for(int i = 0; i < 10; i++)
	{
		if(::ShowCursor(true) >= 0)
			break;
	};

	// mouseGrabbed = false; // TODO
};

}; // namespace sbe::SbInput