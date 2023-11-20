/*
===========================================================================

Copyright (C) 1999-2011 id Software LLC, a ZeniMax Media company.
Copyright (C) 2012 dhewg (dhewm3)
Copyright (C) 2012-2014 Robert Beckebans
Copyright (C) 2013 Daniel Gibson
Copyright (C) 2008-2018 OpenMW Contributors
Copyright (C) 2019-2020 SugarBombEngine Developers

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

In addition, the Doom 3 Source Code is also subject to certain additional terms. You should have received a copy of these additional terms immediately following the terms and conditions of the GNU General Public License which accompanied the Doom 3 Source Code.  If not, please request a copy in writing from id Software at the address below.

If you have questions concerning this license or the applicable additional terms, you may contact in writing id Software LLC, c/o ZeniMax Media Inc., Suite 120, Rockville, Maryland 20850 USA.

===========================================================================
*/

/// @file

//*****************************************************************************

#include <stdexcept>

#include <SDL2/SDL.h>

#include "SbWindowSDL2.hpp"

#include "CoreLibs/SbSystem/ISystem.hpp"

//*****************************************************************************

namespace sbe
{

//params.msTitle = GAME_NAME
SbWindowSDL2::SbWindowSDL2(ISystem &aSystem, const IWindow::Props &params /*int anWidth, int anHeight, const char *asTitle, bool abFullScreen*/)
	: mSystem(aSystem)
{
	if(SDL_Init(SDL_INIT_VIDEO) != 0)
		throw std::runtime_error("Failed to init SDL2 video subsystem!");
	
	// DG: make window resizable
	Uint32 flags = /*SDL_WINDOW_OPENGL |*/ SDL_WINDOW_RESIZABLE;
	// DG end
	
	if(params.mbFullScreen)
		flags |= SDL_WINDOW_FULLSCREEN;

	//if(!windowBorder)
		//flags |= SDL_WINDOW_BORDERLESS;

	// DG: set display num for fullscreen
	int windowPos = SDL_WINDOWPOS_UNDEFINED;
	if( params.mbFullScreen > 0 )
	{
		if( params.mbFullScreen > SDL_GetNumVideoDisplays() )
		{
			mSystem.Warning( "Couldn't set display to num %i because we only have %i displays",
							 params.mbFullScreen, SDL_GetNumVideoDisplays() );
		}
		else
		{
			// -1 because SDL starts counting displays at 0, while params.mbFullScreen starts at 1
			windowPos = SDL_WINDOWPOS_UNDEFINED_DISPLAY( ( params.mbFullScreen - 1 ) );
		};
	};
	
	// TODO: if params.mbFullScreen == -1 there should be a borderless window spanning multiple displays
	/*
	 * NOTE that this implicitly handles params.mbFullScreen == -2 (from r_fullscreen -2) meaning
	 * "do fullscreen, but I don't care on what monitor", at least on my box it's the monitor with
	 * the mouse cursor.
	 */
	
	mpWindow = SDL_CreateWindow( params.msTitle,
							   windowPos,
							   windowPos,
							   params.mnWidth, params.mnHeight, flags );
	// DG end
};

SbWindowSDL2::~SbWindowSDL2()
{
	if(mpWindow)
    {
        SDL_DestroyWindow(mpWindow);
        mpWindow = nullptr;
    };

    SDL_Quit();
};

}; // namespace sbe