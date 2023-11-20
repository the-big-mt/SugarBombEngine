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

#include <functional>

#include <dinput.h>

#include "SbInputImpl.hpp"

//*****************************************************************************

namespace sbe
{

struct ISystem;

namespace SbInput
{

class SbInputWin final : public SbInputImpl
{
public:
	SbInputWin(ISystem &aSystem);
	~SbInputWin();
	
	void Init(const IWindow &aOwnerWindow) override;
	
	void Update() override;
	
	SbKeyboard *CreateKeyboard() override;
	SbMouse *CreateMouse() override;
	SbGamepad *CreateGamepad() override;
	
	//
	
	const IWindow &GetOwnerWindow() const {return mOwnerWindow.get();}
private:
	void InitDirectInput();
	
	void ReleaseDInput();
	
	void Shutdown();
	
	/*SbGamepadXInput *GetGamepad(int anDeviceNum) const
	{
		if(anDeviceNum < 0 || anDeviceNum >= MAX_GAMEPADS)
			return nullptr;
		
		return mvGamepads[anDeviceNum];
	};*/
private:
	//SbGamepadXInput mvGamepads[XUSER_MAX_COUNT]; // TODO
	
	ISystem &mSystem;
	
	std::reference_wrapper<const IWindow> mOwnerWindow;
	
	LPDIRECTINPUT8 mpDInput{nullptr};
};

};}; // namespace sbe::SbInput