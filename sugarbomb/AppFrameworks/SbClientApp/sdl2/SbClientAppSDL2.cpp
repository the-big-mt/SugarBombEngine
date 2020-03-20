/*
*******************************************************************************

Copyright (C) 2019-2020 SugarBombEngine Developers

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

#include <SDL2/SDL.h>

#include "AppFrameworks/SbClientApp/SbClientApp.hpp"

#include "SbWindowSDL2.hpp"

//*****************************************************************************

sbe::IWindow *SbClientApp::CreateMainWindow(const std::string &asTitle, int anWidth, int anHeight, bool abFullScreen)
{
	sbe::IWindow::Props WinProps(asTitle.c_str(), anWidth, anHeight, abFullScreen);
	return new sbe::SbWindowSDL2(mSystem, WinProps);
};

bool SbClientApp::PreFrame()
{
	static SDL_Event Event{};
	
	while(SDL_PollEvent(&Event) != 0)
	{
		if(Event.type == SDL_QUIT)
			Stop();
		
		//return false; // TODO
	};
	
	return true;
};