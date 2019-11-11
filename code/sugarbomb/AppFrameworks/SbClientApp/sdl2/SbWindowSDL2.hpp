/*
===========================================================================

Doom 3 GPL Source Code
Copyright (C) 2019 SugarBombEngine Developers

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
along with Doom 3 Source Code.  If not, see <http://www.gnu.org/licenses/>.

===========================================================================
*/

/// @file

//*****************************************************************************

#pragma once

#include "IWindow.hpp"

//*****************************************************************************

using SDL_Window = struct SDL_Window;

namespace sbe
{

struct ISystem;

class SbWindowSDL2 : public IWindow
{
public:
	SbWindowSDL2(const WindowCreationPrefs &params /*int anWidth, int anHeight, const char *asTitle, bool abFullScreen*/);
private:
	ISystem &mSystem;
	
	SDL_Window *mpWindow{nullptr};
};

}; // namespace sbe