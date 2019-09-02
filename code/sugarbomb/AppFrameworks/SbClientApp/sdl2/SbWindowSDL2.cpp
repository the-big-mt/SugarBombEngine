/// @file

#include "SbWindowSDL2.hpp"

SbWindowSDL2::SbWindowSDL2(const WindowParams &params /*int anWidth, int anHeight, const char *asTitle, bool abFullScreen*/)
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