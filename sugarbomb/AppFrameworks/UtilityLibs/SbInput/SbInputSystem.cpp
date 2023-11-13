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

#include "SbInputSystem.hpp"
#include "SbInputImpl.hpp"
#include "SbGamepad.hpp"

#include "CoreLibs/SbSystem/ISystem.hpp"

#include "AppFrameworks/UtilityLibs/SbInput/SbKeyboard.hpp"
#include "AppFrameworks/UtilityLibs/SbInput/SbMouse.hpp"

//*****************************************************************************

namespace sbe::SbInput
{

SbInputSystem::SbInputSystem(ISystem &aSystem, SbInputImpl &aImpl)
	: mSystem(aSystem), mImpl(aImpl)
{
	mKeyboard.reset(mImpl.CreateKeyboard());
	mMouse.reset(mImpl.CreateMouse());
};

void SbInputSystem::Init(const IWindow &aOwnerWindow)
{
	mSystem.Printf("Input System Initialization\n");
	
	mImpl.Init(aOwnerWindow);
	
	AttachToWindow(aOwnerWindow);
	
	mSystem.Printf("Input System Initialized\n");
};

void SbInputSystem::Shutdown()
{
	mSystem.Printf("Input System Shutdown\n");
};

void SbInputSystem::Update()
{
	mImpl.Update();
};

void SbInputSystem::AttachToWindow(const IWindow &aOwnerWindow)
{
	// TODO
};

}; // namespace sbe::SbInput