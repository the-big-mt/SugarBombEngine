/*
===========================================================================

Doom 3 GPL Source Code
Copyright (C) 1999-2011 id Software LLC, a ZeniMax Media company.
Copyright (C) 2012 dhewg (dhewm3)
Copyright (C) 2012 Robert Beckebans
Copyright (C) 2013 Daniel Gibson
Copyright (C) 2019 BlackPhrase

This file is part of the Doom 3 GPL Source Code ("Doom 3 Source Code").

Doom 3 Source Code is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

Doom 3 Source Code is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with Doom 3 Source Code.	If not, see <http://www.gnu.org/licenses/>.

In addition, the Doom 3 Source Code is also subject to certain additional terms. You should have received a copy of these additional terms immediately following the terms and conditions of the GNU General Public License which accompanied the Doom 3 Source Code.  If not, please request a copy in writing from id Software at the address below.

If you have questions concerning this license or the applicable additional terms, you may contact in writing id Software LLC, c/o ZeniMax Media Inc., Suite 120, Rockville, Maryland 20850 USA.

===========================================================================
*/

/// @file

#pragma once

#include "SbInputSystem.hpp"

class SbInputImplSDL2 final : public IInputSystem
{
public:
	SbInputImplSDL2(ICommon *apCommon);
	
	void Init() override;
	void Shutdown() override;
	
	const char *GetKeyName(keyNum_t keynum) override;
	
	int PollKeyboardInputEvents() override;
	int ReturnKeyboardInputEvent(const int n, int &key, bool &state) override;
	void EndKeyboardInputEvents() override;
	
	void GrabMouseCursor(bool grabIt) override;
	
	int PollMouseInputEvents(int mouseEvents[MAX_MOUSE_EVENTS][2]) override;
	
	void SetRumble(int device, int low, int hi) override;
	
	int PollJoystickInputEvents(int deviceNum) override;
	int ReturnJoystickInputEvent(const int n, int &action, int &value) override;
	void EndJoystickInputEvents() override;
private:
	struct kbd_poll_t
	{
		int key;
		bool state;
		
		kbd_poll_t() = default;
		
		kbd_poll_t( int k, bool s )
		{
			key = k;
			state = s;
		};
	};

	struct mouse_poll_t
	{
		int action;
		int value;
		
		mouse_poll_t() = default;
		
		mouse_poll_t( int a, int v )
		{
			action = a;
			value = v;
		};
	};

	struct joystick_poll_t
	{
		int action;
		int value;
		
		joystick_poll_t() = default;
		
		joystick_poll_t( int a, int v )
		{
			action = a;
			value = v;
		};
	};

	idList<kbd_poll_t> kbd_polls;
	idList<mouse_poll_t> mouse_polls;
	idList<joystick_poll_t> joystick_polls;
	
	SDL_Joystick *joy{nullptr};
	int SDL_joystick_has_hat{0};
	bool buttonStates[K_LAST_KEY];	// For keeping track of button up/down events
};