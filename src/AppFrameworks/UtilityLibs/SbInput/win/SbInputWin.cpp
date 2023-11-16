/*
*******************************************************************************

Copyright (C) 1993-2012 id Software LLC, a ZeniMax Media company.
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

#include "SbInputWin.hpp"
#include "SbKeyboardDInput8.hpp"
#include "SbMouseDInput8.hpp"

#include "CoreLibs/SbSystem/ISystem.hpp"

#include <dinput.h>
#include <XInput.h>

//*****************************************************************************

namespace sbe::SbInput
{

SbInputWin::SbInputWin(ISystem &aSystem) : mSystem(aSystem){}

SbInputWin::~SbInputWin(){Shutdown();}

/*
===========
Sys_InitInput
===========
*/
void SbInputWin::Init(const IWindow &aOwnerWindow)
{
	mOwnerWindow = aOwnerWindow;
	
	//XInputGetBatteryInformation(); // TODO: unused?
	//XInputGetKeystroke(); // TODO: unused?
	
	mSystem.Printf("\n------- Input Initialization -------\n");
	
	//AttachToWindow(mhWnd); // TODO
	
	InitDirectInput();
	
	//if(SbInputWin::in_mouse.GetBool()) // TODO
	{
		//InitDIMouse(); // TODO
		// don't grab the mouse on initialization
		//GrabMouseCursor(false); // TODO
	}
	//else // TODO
		mSystem.Printf("Mouse control not active.\n");
	
	//StartupKeyboard(); // TODO
	
	mSystem.Printf("------------------------------------\n");
	
	//SbInputWin::in_mouse.ClearModified(); // TODO
	
	//g_Joystick.Init(); // TODO
	
	mpKeyboard = CreateKeyboard();
	mpMouse = CreateMouse();
};

/*
==================
IN_Frame

Called every frame, even if not generating commands
==================
*/
void SbInputWin::Update()
{
	bool shouldGrab = true;
	
	if( !SbInputWin::in_mouse.GetBool() )
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
			mpMouse->Deactivate();
		else
		{
			mpMouse->Activate();
			
#if 0	// if we can't reacquire, try reinitializing
			if( !InitDIMouse() )
			{
				SbInputWin::in_mouse.SetBool( false );
				return;
			};
#endif
		};
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
void SbInputWin::InitDirectInput()
{
	mSystem.Printf("Initializing DirectInput...\n");
	
	// if the previous window was destroyed we need to do this
	ReleaseDInput();
	
	HRESULT hr{DirectInput8Create(GetModuleHandle(nullptr), DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&mpDInput, nullptr)};
	
	// Register with the DirectInput subsystem and get a pointer
	// to a IDirectInput interface we can use.
	// Create the base DirectInput object
	if(FAILED(hr))
		mSystem.Printf("DirectInputCreate failed\n");
};

void SbInputWin::ReleaseDInput()
{
	if(mpDInput != nullptr)
	{
		mpDInput->Release();
		mpDInput = nullptr;
	};
};

/*
===========
Sys_ShutdownInput
===========
*/
void SbInputWin::Shutdown()
{
	mpMouse->Deactivate(); // TODO
	mpKeyboard->Deactivate(); // TODO
	
	ReleaseDInput();
	
	//DetachFromWindow(); // TODO
};

SbKeyboard *SbInputWin::CreateKeyboard()
{
	return new SbKeyboardDInput8(mOwnerWindow.get().GetProps().mpHandle, mSystem, mpDInput);
};

SbMouse *SbInputWin::CreateMouse()
{
	return new SbMouseDInput8(mOwnerWindow.get().GetProps().mpHandle, mSystem, mpDInput);
};

SbGamepad *SbInputWin::CreateGamepad()
{
	return new SbGamepadXInput(mSystem);
};

}; // namespace sbe::SbInput