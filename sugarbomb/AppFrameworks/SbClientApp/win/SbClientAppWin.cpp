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

#include "AppFrameworks/SbClientApp/SbClientApp.hpp"

#include "SbWindowWin.hpp"

//*****************************************************************************

sbe::IWindow *SbClientApp::CreateMainWindow(const std::string &asTitle, int anWidth, int anHeight, bool abFullScreen)
{
	return new sbe::SbWindowWin(asTitle.c_str(), "SugarBombEngine", 0, mSystem, anWidth, anHeight, abFullScreen); // TODO: de-hardcode
};

bool SbClientApp::PreFrame()
{
	static MSG Msg{};
	
	if(PeekMessage(&Msg, nullptr, 0, 0, PM_REMOVE))
	{
		if(Msg.message == WM_QUIT)
			Stop();
		
		TranslateMessage(&Msg);
		DispatchMessage(&Msg);
		return false;
	};
	
	return true;
};