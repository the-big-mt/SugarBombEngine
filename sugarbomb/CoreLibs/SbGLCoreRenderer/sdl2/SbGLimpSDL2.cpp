/*
===========================================================================

Copyright (C) 1999-2011 id Software LLC, a ZeniMax Media company.
Copyright (C) 2012 dhewg (dhewm3)
Copyright (C) 2012-2014 Robert Beckebans
Copyright (C) 2013 Daniel Gibson
Copyright (C) 2019-2020 BlackPhrase

This file is part of SugarBombEngine

SugarBombEngine is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

SugarBombEngine is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with SugarBombEngine. If not, see <http://www.gnu.org/licenses/>.

In addition, SugarBombEngine is using id Tech 4 (BFG) pieces and thus subject to certain additional terms (all header and source files which 
contains such pieces has this additional part appended to the license header). You should have received a copy of these additional terms 
stated in a separate file (LICENSE-idTech4) which accompanied the SugarBombEngine source code. If not, please request a copy in 
writing from id Software at the address below.

If you have questions concerning this license or the applicable additional terms, you may contact in writing id Software LLC, c/o ZeniMax Media Inc.,
Suite 120, Rockville, Maryland 20850 USA.

===========================================================================
*/

/// @file

//*****************************************************************************

#include "SbRenderSystem.hpp"

//*****************************************************************************

namespace sbe::SbGLCoreRenderer
{

/*
===================
GLimp_Init
===================
*/
bool SbRenderSystem::GLimp_Init(glimpParms_t parms)
{
	mSystem.Printf("Initializing OpenGL subsystem\n");
	
	if( !mpWindow )
	{
		mSystem.DPrintf( "Couldn't set GL mode %d/%d/%d: %s",
						 channelcolorbits, tdepthbits, tstencilbits, SDL_GetError() );
		//continue; // TODO
	};
	
	mpContext = std::make_unique<SbRenderContextGLSDL2>(mpWindow, parms, 2, false, false);
	
	if( SDL_GL_SetSwapInterval( r_swapInterval.GetInteger() ) < 0 )
		mSystem.Warning( "SDL_GL_SWAP_CONTROL not supported" );
		
	// RB begin
	SDL_GetWindowSize( mpWindow, &glConfig.nativeScreenWidth, &glConfig.nativeScreenHeight );
	// RB end
	
	glConfig.isFullscreen = ( SDL_GetWindowFlags( mpWindow ) & SDL_WINDOW_FULLSCREEN ) == SDL_WINDOW_FULLSCREEN;
	
	mSystem.Printf( "Using %d color bits, %d depth, %d stencil display\n",
					channelcolorbits, tdepthbits, tstencilbits );
					
	glConfig.colorBits = tcolorbits;
	glConfig.depthBits = tdepthbits;
	glConfig.stencilBits = tstencilbits;
	
	// RB begin
	glConfig.displayFrequency = 60;
	glConfig.isStereoPixelFormat = parms.stereo;
	glConfig.multisamples = parms.multiSamples;
	
	glConfig.pixelAspect = 1.0f;	// FIXME: some monitor modes may be distorted
	// should side-by-side stereo modes be consider aspect 0.5?
	
	// RB end
	
	//break;
//};
	
	if( !mpWindow )
	{
		mSystem.Printf( "No usable GL mode found: %s", SDL_GetError() );
		return false;
	};
	
#ifdef __APPLE__
	glewExperimental = GL_TRUE;
#endif
	
	GLenum glewResult = glewInit();
	if( GLEW_OK != glewResult )
	{
		// glewInit failed, something is seriously wrong
		mSystem.Printf( "^3GLimp_Init() - GLEW could not load OpenGL subsystem: %s", glewGetErrorString( glewResult ) );
	}
	else
		mSystem.Printf( "Using GLEW %s\n", glewGetString( GLEW_VERSION ) );
	
	// DG: disable cursor, we have two cursors in menu (because mouse isn't grabbed in menu)
	SDL_ShowCursor(SDL_DISABLE);
	// DG end
	
	return true;
};

/*
===================
GLimp_Shutdown
===================
*/
void SbRenderSystem::GLimp_Shutdown()
{
	mSystem.Printf("Shutting down OpenGL subsystem\n");
	
	// TODO: delete the rendering context
};

/*
===================
GLimp_SwapBuffers
===================
*/
void SbRenderSystem::GLimp_SwapBuffers()
{
	SDL_GL_SwapWindow(mpWindow);
};

/*
=================
GLimp_SetGamma
=================
*/
void SbRenderSystem::GLimp_SetGamma(unsigned short anRed[256], unsigned short anGreen[256], unsigned short anBlue[256])
{
	if(!mpWindow)
	{
		mSystem.Warning("GLimp_SetGamma called without window");
		return;
	};
	
	if(SDL_SetWindowGammaRamp(mpWindow, anRed, anGreen, anBlue))
		mSystem.Warning("Couldn't set gamma ramp: %s", SDL_GetError());
};

}; // namespace sbe::SbGLCoreRenderer