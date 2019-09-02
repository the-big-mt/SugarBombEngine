/*
===========================================================================

Doom 3 BFG Edition GPL Source Code
Copyright (C) 1993-2012 id Software LLC, a ZeniMax Media company.
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

#pragma once

#include "SbInputSystem.hpp"
#include "JoystickWin.hpp"

#include "framework/CVar.hpp"

#include <dinput.h>

struct idUsercmdGen;

class SbInputImplWin final : public sbe::IInputSystem
{
public:
	SbInputImplWin(sbe::ISys *apSys, idUsercmdGen *apUserCmdGen, HWND ahWnd);
	
	void Init() override;
	void Shutdown() override;
	
	const char *GetKeyName(keyNum_t keynum) const override;
	
	int PollKeyboardInputEvents() override;
	int ReturnKeyboardInputEvent(const int n, int &key, bool &state) override;
	void EndKeyboardInputEvents() override;
	
	void GrabMouseCursor(bool grabIt) override;
	
	int PollMouseInputEvents(int mouseEvents[sbe::MAX_MOUSE_EVENTS][2]) override;
	
	void SetRumble(int device, int low, int hi) override;
	
	int PollJoystickInputEvents(int deviceNum) override;
	int ReturnJoystickInputEvent(const int n, int &action, int &value) override;
	void EndJoystickInputEvents() override;
private:
	void AttachToWindow(HWND ahWindow);
	void DetachToWindow();
	
	void Frame();
	
	void InitDirectInput();
	bool InitDIMouse();
	
	void ActivateMouse();
	void DeactivateMouse();
	
	bool StartupKeyboard();
	void DeactivateKeyboard();
	
	static sbe::idCVar in_mouse;
	
	sbe::ISys *mpSys{nullptr};
	idUsercmdGen *usercmdGen{nullptr};
	
	HWND mhWnd{nullptr};
	
	LPDIRECTINPUT8 g_pdi;
	LPDIRECTINPUTDEVICE8 g_pMouse;
	LPDIRECTINPUTDEVICE8 g_pKeyboard;
	idJoystickWin32 g_Joystick;
	
	bool mouseReleased{false}; // when the game has the console down or is doing a long operation
	bool mouseGrabbed{false}; // current state of grab and hide
};