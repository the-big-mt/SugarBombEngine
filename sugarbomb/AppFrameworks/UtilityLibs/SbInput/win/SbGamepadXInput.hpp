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

#pragma once

#include <XInput.h>

//*****************************************************************************

namespace sbe
{

struct ISystem;

namespace SbInput
{

struct controllerState_t
{
	// the current states are updated by the input thread at 250 hz
	XINPUT_STATE current{};
	
	// the previous state is latched at polling time
	XINPUT_STATE previous{};
	
	// The current button bits are or'd into this at the high sampling rate, then
	// zero'd by the main thread when a usercmd_t is created.  This prevents the
	// complete missing of a button press that went down and up between two usercmd_t
	// creations, although it can add sn extra frame of latency to sensing a release.
	int buttonBits{0};
	
	// Only valid controllers will have their rumble set
	bool valid{false};
};

class SbGamepadXInput
{
public:
	SbGamepadXInput(ISystem &aSystem);
	
	int PollInputEvents();
	
	void SetRumble(int anRumbleLow, int anRumbleHigh);
private:
	static DWORD mnLastUsedID;
	
	controllerState_t mState{};
	//XINPUT_STATE mState{};
	
	ISystem &mSystem;
	
	DWORD mnID{0}; // TODO: uint32_t
	
	int mnEvents{0};
	
	//bool mbValid{false};
};

};}; // namespace sbe::SbInput