/*
*******************************************************************************

Copyright (C) 1993-2012 id Software LLC, a ZeniMax Media company.
Copyright (C) 2020 SugarBombEngine Developers

This file is part of SugarBombEngine

SugarBombEngine is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

SugarBombEngine is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with SugarBombEngine. If not, see <http://www.gnu.org/licenses/>.

In addition, SugarBombEngine is using id Tech 4 (BFG) pieces and thus
subject to certain additional terms (all header and source files which 
contains such pieces has this additional part appended to the license 
header). You should have received a copy of these additional terms 
stated in a separate file (LICENSE-idTech4) which accompanied the 
SugarBombEngine source code. If not, please request a copy in 
writing from id Software at the address below.

If you have questions concerning this license or the applicable additional
terms, you may contact in writing id Software LLC, c/o ZeniMax Media Inc.,
Suite 120, Rockville, Maryland 20850 USA.

*******************************************************************************
*/

/// @file

//*****************************************************************************

#include <dinput.h>

#include "SbKeyboardDInput8.hpp"

#include "CoreLibs/SbSystem/ISystem.hpp"

//*****************************************************************************

namespace sbe::SbInput
{

/*
============================================================

DIRECT INPUT KEYBOARD CONTROL

============================================================
*/

SbKeyboardDInput8::SbKeyboardDInput8(SbInputWin &aInputWin, ISystem &aSystem, IDirectInput8 *apDInput) : mSystem(aSystem)
{
	if(!apDInput)
	{
		mSystem.Printf("keyboard: DirectInput has not been started\n");
		return; //false;
	};
	
	if(mpDevice)
	{
		mpDevice->Release();
		mpDevice = nullptr;
	};
	
	HRESULT hr{apDInput->CreateDevice(GUID_SysKeyboard, &mpDevice, nullptr)};
	
	// Obtain an interface to the system keyboard device.
	if(FAILED(hr))
	{
		mSystem.Printf("keyboard: couldn't find a keyboard device\n");
		return; //false;
	};
	
	// Set the data format to "keyboard format" - a predefined data format
	//
	// A data format specifies which controls on a device we
	// are interested in, and how they should be reported.
	//
	// This tells DirectInput that we will be passing an array
	// of 256 bytes to IDirectInputDevice::GetDeviceState.
	hr = mpDevice->SetDataFormat(&c_dfDIKeyboard);
	if(FAILED(hr))
		return; //false;
	
	DWORD dwCoopFlags{DISCL_NONEXCLUSIVE};
	
	// Determine where the buffer would like to be allocated
	bool bExclusive{false};
	bool bForeground{true};
	bool bImmediate{false};
	bool bDisableWindowsKey{true};
	
	if(bExclusive)
		dwCoopFlags = DISCL_EXCLUSIVE;
	
	if(bForeground)
		dwCoopFlags |= DISCL_FOREGROUND;
	else
		dwCoopFlags |= DISCL_BACKGROUND;
		
	// Disabling the windows key is only allowed only if we are in foreground nonexclusive
	if(bDisableWindowsKey && !bExclusive && bForeground)
		dwCoopFlags |= DISCL_NOWINKEY;
	
	// Set the cooperativity level to let DirectInput know how
	// this device should interact with the system and with other
	// DirectInput applications.
	hr = mpDevice->SetCooperativeLevel(aInputWin.GetOwnerWindow().GetProps().mpHandle, dwCoopFlags);
	if(hr == DIERR_UNSUPPORTED && !bForeground && bExclusive)
	{
		mSystem.Printf("keyboard: SetCooperativeLevel() returned DIERR_UNSUPPORTED.\nFor security reasons, background exclusive keyboard access is not allowed.\n");
		return; //false;
	};
	
	if(FAILED(hr))
		return; //false;
	
	if(!bImmediate)
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
		DIPROPDWORD dipdw{};
		
		dipdw.diph.dwSize       = sizeof( DIPROPDWORD );
		dipdw.diph.dwHeaderSize = sizeof( DIPROPHEADER );
		dipdw.diph.dwObj        = 0;
		dipdw.diph.dwHow        = DIPH_DEVICE;
		dipdw.dwData            = DINPUT_BUFFERSIZE; // Arbitary buffer size
		
		hr = mpDevice->SetProperty(DIPROP_BUFFERSIZE, &dipdw.diph);
		if(FAILED(hr))
			return; //false;
	};
	
	// Acquire the newly created device
	mpDevice->Acquire();
	
	mSystem.Printf("keyboard: DirectInput initialized.\n");
	//return true;
};

/*
void SbKeyboardDInput8::Update()
{
};
*/

/*
==========================
IN_DeactivateKeyboard
==========================
*/
void SbKeyboardDInput8::Deactivate()
{
	if(!mpDevice)
		return;

	mpDevice->Unacquire();
};

}; // namespace sbe::SbInput