/*
*******************************************************************************

Copyright (C) 2020 SugarBombEngine Developers

This file is part of SugarBombEngine

SugarBombEngine is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

SugarBombEngine is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with SugarBombEngine. If not, see <http://www.gnu.org/licenses/>.

*******************************************************************************
*/

/// @file

//*****************************************************************************

#pragma once

#include <windef.h>

#include "SbInputDeviceDInput8.hpp"

#include "AppFrameworks/UtilityLibs/SbInput/SbMouse.hpp"

//*****************************************************************************

struct IDirectInput8;

namespace sbe
{

struct ISystem;

namespace SbInput
{

class SbInputWin;

class SbMouseDInput8 final : public SbInputDeviceDInput8, public SbMouse
{
public:
	SbMouseDInput8(SbInputWin &aInputWin, ISystem &aSystem, IDirectInput8 *apDInput);
	
	//void Update();
	
	int Poll(int anEvents[MAX_MOUSE_EVENTS][2]);
	
	void Activate();
	void Deactivate();
private:
	ISystem &mSystem;
};

};}; // namespace sbe::SbInput