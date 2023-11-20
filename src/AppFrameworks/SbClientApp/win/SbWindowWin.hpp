/*
*******************************************************************************

Copyright (C) 2019 SugarBombEngine Developers

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

*******************************************************************************
*/

/// @file

//*****************************************************************************

#pragma once

#include <windef.h>
#include <Winuser.h>

#include "IWindow.hpp"

//*****************************************************************************

namespace sbe
{

struct ISystem;

class SbWindowWin final : public IWindow
{
public:
	SbWindowWin(LPCTSTR asWindowName, const char *asClassName, HINSTANCE ahInstance, ISystem &aSystem, int anWidth, int anHeight, bool abFullScreen);
	~SbWindowWin();
	
	void SetVisible(bool abVisible); // TODO: unused
	bool IsVisible() const; // TODO: unused
private:
	void CreateWindowClass(HINSTANCE ahInstance, const char *asClassName);
private:
	ISystem &mSystem;
	
	HWND mhWnd{nullptr};
	WNDPROC mWndProc;
	
	bool mbWindowClassRegistered{false};
};

}; // namespace sbe