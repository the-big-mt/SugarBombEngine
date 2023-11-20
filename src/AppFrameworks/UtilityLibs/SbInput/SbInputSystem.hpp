/*
*******************************************************************************

Copyright (C) 2019-2020 SugarBombEngine Developers

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

#include <vector>
#include <memory>

#include "AppFrameworks/UtilityLibs/SbInput/IInputSystem.hpp"

//*****************************************************************************

namespace sbe
{

struct ISystem;

namespace SbInput
{

struct SbInputImpl;

using tGamepadVec = std::vector<std::unique_ptr<SbGamepad>>;

class SbInputSystem : public IInputSystem
{
public:
	SbInputSystem(ISystem &aSystem, SbInputImpl &aImpl);
	
	void Init(const IWindow &aOwnerWindow) override;
	void Shutdown() override;
	
	void Update() override;
	
	SbKeyboard &GetKeyboard() const override {return *mKeyboard.get();}
	SbMouse &GetMouse() const override {return *mMouse.get();}
	SbGamepad &GetGamepad(int anIndex) const override {return *mvGamepads.at(anIndex).get();}
private:
	void AttachToWindow(const IWindow &aOwnerWindow);
private:
	tGamepadVec mvGamepads;
	
	std::unique_ptr<SbKeyboard> mKeyboard;
	std::unique_ptr<SbMouse> mMouse;
	
	ISystem &mSystem;
	SbInputImpl &mImpl;
};

};}; // namespace sbe::SbInput