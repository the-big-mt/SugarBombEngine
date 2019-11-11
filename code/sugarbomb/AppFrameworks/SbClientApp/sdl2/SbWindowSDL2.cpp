/*
===========================================================================

Doom 3 GPL Source Code
Copyright (C) 1999-2011 id Software LLC, a ZeniMax Media company.
Copyright (C) 2012 dhewg (dhewm3)
Copyright (C) 2012-2014 Robert Beckebans
Copyright (C) 2013 Daniel Gibson
Copyright (C) 2008-2018 OpenMW Contributors
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

In addition, the Doom 3 Source Code is also subject to certain additional terms. You should have received a copy of these additional terms immediately following the terms and conditions of the GNU General Public License which accompanied the Doom 3 Source Code.  If not, please request a copy in writing from id Software at the address below.

If you have questions concerning this license or the applicable additional terms, you may contact in writing id Software LLC, c/o ZeniMax Media Inc., Suite 120, Rockville, Maryland 20850 USA.

===========================================================================
*/

/// @file

//*****************************************************************************

#include "SbWindowSDL2.hpp"

//*****************************************************************************

namespace sbe
{

SbWindowSDL2::SbWindowSDL2(const WindowParams &params /*int anWidth, int anHeight, const char *asTitle, bool abFullScreen*/)
	: mSystem(aSystem)
{
	// DG: make window resizable
	Uint32 flags = SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE;
	// DG end
	
	if( abFullScreen )
		flags |= SDL_WINDOW_FULLSCREEN;

	// DG: set display num for fullscreen
	int windowPos = SDL_WINDOWPOS_UNDEFINED;
	if( parms.fullScreen > 0 )
	{
		if( parms.fullScreen > SDL_GetNumVideoDisplays() )
		{
			common->Warning( "Couldn't set display to num %i because we only have %i displays",
							 parms.fullScreen, SDL_GetNumVideoDisplays() );
		}
		else
		{
			// -1 because SDL starts counting displays at 0, while parms.fullScreen starts at 1
			windowPos = SDL_WINDOWPOS_UNDEFINED_DISPLAY( ( parms.fullScreen - 1 ) );
		};
	};
	
	// TODO: if parms.fullScreen == -1 there should be a borderless window spanning multiple displays
	/*
	 * NOTE that this implicitly handles parms.fullScreen == -2 (from r_fullscreen -2) meaning
	 * "do fullscreen, but I don't care on what monitor", at least on my box it's the monitor with
	 * the mouse cursor.
	 */
	
	window = SDL_CreateWindow( GAME_NAME,
							   windowPos,
							   windowPos,
							   parms.width, parms.height, flags );
	// DG end
};
}; // namespace sbe