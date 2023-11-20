/*
*******************************************************************************

Copyright (C) 1993-2012 id Software LLC, a ZeniMax Media company.
Copyright (C) 2013-2014 Robert Beckebans
Copyright (C) 2020 SugarBombEngine Developers

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

*******************************************************************************
*/

/// @file

/*
** WIN_GLIMP.C
**
** This file contains ALL Win32 specific stuff having to do with the
** OpenGL refresh.  When a port is being made the following functions
** must be implemented by the port:
**
** GLimp_SwapBuffers
** GLimp_Init
** GLimp_Shutdown
** GLimp_SetGamma
**
** Note that the GLW_xxx functions are Windows specific GL-subsystem
** related functions that are relevant ONLY to win_glimp.c
*/

//*****************************************************************************

#include "SbRenderSystem.hpp"

#include "CoreLibs/SbSystem/ISystem.hpp"

//*****************************************************************************

namespace sbe::SbGLCoreRenderer
{

/*
========================
GLimp_GetOldGammaRamp
========================
*/
void SbRenderSystemGLW::GLimp_SaveGamma()
{
	auto hDC{GetDC( GetDesktopWindow() )};
	auto bSuccess{GetDeviceGammaRamp( hDC, win32.oldHardwareGamma )};
	
	mSystem.DPrintf( "...getting default gamma ramp: %s\n", bSuccess ? "success" : "failed" );
	
	ReleaseDC( GetDesktopWindow(), hDC );
};

/*
========================
GLimp_RestoreGamma
========================
*/
void SbRenderSystemGLW::GLimp_RestoreGamma()
{
	// if we never read in a reasonable looking
	// table, don't write it out
	if( win32.oldHardwareGamma[0][255] == 0 )
		return;
	
	auto hDC{GetDC( GetDesktopWindow() )};
	auto bSuccess{SetDeviceGammaRamp( hDC, win32.oldHardwareGamma )};
	mSystem.DPrintf( "...restoring hardware gamma: %s\n", bSuccess ? "success" : "failed" );
	ReleaseDC( GetDesktopWindow(), hDC );
};

/*
===================
GLW_ChangeDislaySettingsIfNeeded

Optionally ChangeDisplaySettings to get a different fullscreen resolution.
Default uses the full desktop resolution.
===================
*/
bool SbRenderSystemGLW::GLW_ChangeDislaySettingsIfNeeded( glimpParms_t parms )
{
	// If we had previously changed the display settings on a different monitor,
	// go back to standard.
	if( win32.cdsFullscreen != 0 && win32.cdsFullscreen != parms.fullScreen )
	{
		win32.cdsFullscreen = 0;
		ChangeDisplaySettings( 0, 0 );
		Sys_Sleep( 1000 ); // Give the driver some time to think about this change
	};
	
	// 0 is dragable mode on desktop, -1 is borderless window on desktop
	if( parms.fullScreen <= 0 )
		return true;
	
	// if we are already in the right resolution, don't do a ChangeDisplaySettings
	int x, y, width, height, displayHz;
	
	if( !GetDisplayCoordinates( parms.fullScreen - 1, x, y, width, height, displayHz ) )
		return false;

	if( width == parms.width && height == parms.height && ( displayHz == parms.displayHz || parms.displayHz == 0 ) )
		return true;
	
	DEVMODE dm{};
	
	dm.dmSize = sizeof( dm );
	
	dm.dmPelsWidth  = parms.width;
	dm.dmPelsHeight = parms.height;
	dm.dmBitsPerPel = 32;
	dm.dmFields     = DM_PELSWIDTH | DM_PELSHEIGHT | DM_BITSPERPEL;
	
	if( parms.displayHz != 0 )
	{
		dm.dmDisplayFrequency = parms.displayHz;
		dm.dmFields |= DM_DISPLAYFREQUENCY;
	};
	
	mSystem.Printf( "...calling CDS: " );
	
	const char* const deviceName = GetDisplayName( parms.fullScreen - 1 );
	
	int		cdsRet;
	if( ( cdsRet = ChangeDisplaySettingsEx(
					   deviceName,
					   &dm,
					   nullptr,
					   CDS_FULLSCREEN,
					   nullptr ) ) == DISP_CHANGE_SUCCESSFUL )
	{
		mSystem.Printf( "ok\n" );
		win32.cdsFullscreen = parms.fullScreen;
		return true;
	};
	
	mSystem.Printf( "^3failed^0, " );
	PrintCDSError( cdsRet );
	return false;
};

/*
====================
GLW_ChoosePixelFormat

Returns -1 on failure, or a pixel format
====================
*/
static int GLW_ChoosePixelFormat( const HDC hdc, const int multisamples, const bool stereo3D )
{
	FLOAT fAttributes[] = { 0, 0 };
	int iAttributes[] =
	{
		WGL_SAMPLE_BUFFERS_ARB, ( ( multisamples > 1 ) ? 1 : 0 ),
		WGL_SAMPLES_ARB, multisamples,
		WGL_DOUBLE_BUFFER_ARB, TRUE,
		WGL_STENCIL_BITS_ARB, 8,
		WGL_DEPTH_BITS_ARB, 24,
		WGL_RED_BITS_ARB, 8,
		WGL_BLUE_BITS_ARB, 8,
		WGL_GREEN_BITS_ARB, 8,
		WGL_ALPHA_BITS_ARB, 8,
		WGL_STEREO_ARB, ( stereo3D ? TRUE : FALSE ),
		0, 0
	};
	
	int	pixelFormat;
	UINT numFormats;
	
	if( !wglChoosePixelFormatARB( hdc, iAttributes, fAttributes, 1, &pixelFormat, &numFormats ) )
		return -1;

	return pixelFormat;
};

/*
====================
GLW_InitDriver

Set the pixelformat for the window before it is
shown, and create the rendering context
====================
*/
bool SbRenderSystemGLW::GLW_InitDriver(glimpParms_t parms, HWND ahWnd)
{
	mSystem.Printf( "Initializing OpenGL driver\n" );
	
	HDC hDC; // TODO
	
	//
	// get a DC for our window if we don't already have one allocated
	//
	if( hDC == nullptr )
	{
		mSystem.Printf( "...getting DC: " );
		
		if( ( hDC = GetDC( ahWnd ) ) == nullptr )
		{
			mSystem.Printf( "^3failed^0\n" );
			return false;
		};
		mSystem.Printf( "succeeded\n" );
	};
	
	// the multisample path uses the wgl
	if( WGLEW_ARB_pixel_format )
		win32.pixelformat = GLW_ChoosePixelFormat( hDC, parms.multiSamples, parms.stereo );
	else
	{
		// this is the "classic" choose pixel format path
		mSystem.Printf( "Using classic ChoosePixelFormat\n" );
		
		PIXELFORMATDESCRIPTOR pfd =
		{
			sizeof( PIXELFORMATDESCRIPTOR ),	// size of this pfd
			1,								// version number
			PFD_DRAW_TO_WINDOW |			// support window
			PFD_SUPPORT_OPENGL |			// support OpenGL
			PFD_DOUBLEBUFFER,				// double buffered
			PFD_TYPE_RGBA,					// RGBA type
			32,								// 32-bit color depth
			0, 0, 0, 0, 0, 0,				// color bits ignored
			8,								// 8 bit destination alpha
			0,								// shift bit ignored
			0,								// no accumulation buffer
			0, 0, 0, 0, 					// accum bits ignored
			24,								// 24-bit z-buffer
			8,								// 8-bit stencil buffer
			0,								// no auxiliary buffer
			PFD_MAIN_PLANE,					// main layer
			0,								// reserved
			0, 0, 0							// layer masks ignored
		};

		// eventually we may need to have more fallbacks, but for
		// now, ask for everything
		if( parms.stereo )
		{
			mSystem.Printf( "...attempting to use stereo\n" );
			pfd.dwFlags |= PFD_STEREO;
		};
		
		//
		// choose, set, and describe our desired pixel format.  If we're
		// using a minidriver then we need to bypass the GDI functions,
		// otherwise use the GDI functions.
		//
		if( ( win32.pixelformat = ChoosePixelFormat( hDC, &pfd ) ) == 0 )
		{
			mSystem.Printf( "...^3GLW_ChoosePFD failed^0\n" );
			return false;
		};
		mSystem.Printf( "...PIXELFORMAT %d selected\n", win32.pixelformat );
	};
	
	// get the full info
	DescribePixelFormat( hDC, win32.pixelformat, sizeof( win32.pfd ), &win32.pfd );
	
	glConfig.colorBits = win32.pfd.cColorBits;
	glConfig.depthBits = win32.pfd.cDepthBits;
	glConfig.stencilBits = win32.pfd.cStencilBits;
	
	// XP seems to set this incorrectly
	if( !glConfig.stencilBits )
		glConfig.stencilBits = 8;

	// the same SetPixelFormat is used either way
	if( SetPixelFormat( hDC, win32.pixelformat, &win32.pfd ) == FALSE )
	{
		mSystem.Printf( "...^3SetPixelFormat failed^0\n", hDC );
		return false;
	};
	
	// TODO: create the rendering context
	SbRenderContextWGL *pContext = new SbRenderContextWGL(mSystem);
	
	return true;
};

/*
===================
GLimp_Init

This is the platform specific OpenGL initialization function.  It
is responsible for loading OpenGL, initializing it,
creating a window of the appropriate size, doing
fullscreen manipulations, etc.  Its overall responsibility is
to make sure that a functional OpenGL subsystem is operating
when it returns to the ref.

If there is any failure, the renderer will revert back to safe
parameters and try again.
===================
*/
bool SbRenderSystemGLW::GLimp_Init(glimpParms_t parms)
{
	mSystem.Printf("Initializing OpenGL subsystem with multisamples:%i stereo:%i fullscreen:%i\n",
					parms.multiSamples, parms.stereo, parms.fullScreen);
	
	// check our desktop attributes
	auto hDC{GetDC( GetDesktopWindow() )};
	win32.desktopBitsPixel = GetDeviceCaps( hDC, BITSPIXEL );
	//win32.desktopWidth = GetDeviceCaps( hDC, HORZRES ); // TODO: unused?
	//win32.desktopHeight = GetDeviceCaps( hDC, VERTRES ); // TODO: unused?
	ReleaseDC( GetDesktopWindow(), hDC );
	
	// we can't run in a window unless it is 32 bpp
	if( win32.desktopBitsPixel < 32 && parms.fullScreen <= 0 )
	{
		mSystem.Printf( "^3Windowed mode requires 32 bit desktop depth^0\n" );
		return false;
	};
	
	// save the hardware gamma so it can be
	// restored on exit
	GLimp_SaveGamma();
	
	// create our window classes if we haven't already
	GLW_CreateWindowClasses();
	
	// this will load the dll and set all our gl* function pointers,
	// but doesn't create a window
	
	// getting the wgl extensions involves creating a fake window to get a context,
	// which is pretty disgusting, and seems to mess with the AGP VAR allocation
	GLW_GetWGLExtensionsWithFakeWindow();
	
	// Optionally ChangeDisplaySettings to get a different fullscreen resolution.
	if( !GLW_ChangeDislaySettingsIfNeeded( parms ) )
	{
		GLimp_Shutdown();
		return false;
	};
	
	// try to create a window with the correct pixel format
	// and init the renderer context
	//if( !GLW_CreateWindow( parms ) )
	{
		// makeCurrent nullptr frees the DC, so get another
		win32.hDC = GetDC( mWindow->GetProps()->mpHandle );
		if( !win32.hDC )
		{
			mSystem.Printf( "^3GLW_CreateWindow() - GetDC()failed^0\n" );
			return false;
		};
		
		glConfig.stereoPixelFormatAvailable = false;
		
		// Check to see if we can get a stereo pixel format, even if we aren't going to use it,
		// so the menu option can be
		if( GLW_ChoosePixelFormat( win32.hDC, parms.multiSamples, true ) != -1 )
			glConfig.stereoPixelFormatAvailable = true;
		
		if(!GLW_InitDriver(parms, mWindow))
		{
			GLimp_Shutdown();
			return false;
		};
		
		glConfig.isFullscreen = parms.fullScreen;
	};
	
	glConfig.isFullscreen = parms.fullScreen;
	glConfig.isStereoPixelFormat = parms.stereo;
	glConfig.nativeScreenWidth = parms.width;
	glConfig.nativeScreenHeight = parms.height;
	glConfig.multisamples = parms.multiSamples;
	
	glConfig.pixelAspect = 1.0f; // FIXME: some monitor modes may be distorted;
								 // should side-by-side stereo modes be consider aspect 0.5?
	
	// get the screen size, which may not be reliable...
	// If we use the windowDC, I get my 30" monitor, even though the window is
	// on a 27" monitor, so get a dedicated DC for the full screen device name.
	const idStr deviceName{GetDeviceName( Max( 0, parms.fullScreen - 1 ) )};
	
	// TODO: is this code doing what it should?
	auto deviceDC{CreateDC( deviceName.c_str(), deviceName.c_str(), nullptr, nullptr )};
	const int mmWide{GetDeviceCaps( win32.hDC, HORZSIZE )};
	DeleteDC( deviceDC );
	//
	
	if( mmWide == 0 )
		glConfig.physicalScreenWidthInCentimeters = 100.0f;
	else
		glConfig.physicalScreenWidthInCentimeters = 0.1f * mmWide;
	
	// RB: we probably have a new OpenGL 3.2 core context so reinitialize GLEW
	GLenum glewResult = glewInit();
	if( GLEW_OK != glewResult )
		// glewInit failed, something is seriously wrong
		mSystem.Printf( "^3GLimp_Init() - GLEW could not load OpenGL subsystem: %s", glewGetErrorString( glewResult ) );
	else
		mSystem.Printf( "Using GLEW %s\n", glewGetString( GLEW_VERSION ) );
	// RB end
	
	// wglSwapinterval, etc
	//GLW_CheckWGLExtensions( win32.hDC ); // TODO: unused in rbdoom (called earlier)
	
	// TODO: GLimp_EnableLogging was here in original BFG
	
	return true;
};

/*
===================
GLimp_Shutdown

This routine does all OS specific shutdown procedures for the OpenGL
subsystem.
===================
*/
void SbRenderSystemGLW::GLimp_Shutdown()
{
	mSystem.Printf( "Shutting down OpenGL subsystem\n" );
	
	// TODO: destroy the rendering context
	
	// release DC
	if( win32.hDC )
	{
		const char *success[] = {"failed", "success"};
		
		int retVal = ReleaseDC( mWindow.GetProps().mpHandle, win32.hDC ) != 0;
		mSystem.Printf( "...releasing DC: %s\n", success[retVal] );
		win32.hDC = nullptr;
	};
	
	// reset display settings
	if(win32.cdsFullscreen)
	{
		mSystem.Printf( "...resetting display\n" );
		ChangeDisplaySettings( 0, 0 );
		win32.cdsFullscreen = 0;
	};
	
	// restore gamma
	GLimp_RestoreGamma();
};

/*
=====================
GLimp_SwapBuffers
=====================
*/
// RB: use GLEW for V-Sync
void SbRenderSystem::GLimp_SwapBuffers()
{
	if( r_swapInterval.IsModified() )
	{
		r_swapInterval.ClearModified();
		
		int interval = 0;
		if( r_swapInterval.GetInteger() == 1 )
			interval = ( glConfig.swapControlTearAvailable ) ? -1 : 1;
		else if( r_swapInterval.GetInteger() == 2 )
			interval = 1;
		
		if( WGLEW_EXT_swap_control )
			wglSwapIntervalEXT( interval );
	};
	
	SwapBuffers( win32.hDC );
};
// RB end

/*
========================
GLimp_SetGamma

The renderer calls this when the user adjusts r_gamma or r_brightness
========================
*/
void SbRenderSystem::GLimp_SetGamma( unsigned short red[256], unsigned short green[256], unsigned short blue[256] )
{
	if( !win32.hDC )
		return;
	
	unsigned short table[3][256];
	
	for( int i = 0; i < 256; i++ )
	{
		table[0][i] = red[i];
		table[1][i] = green[i];
		table[2][i] = blue[i];
	};
	
	if( !SetDeviceGammaRamp( win32.hDC, table ) )
		mSystem.Printf( "WARNING: SetDeviceGammaRamp failed.\n" );
};

}; // namespace sbe::SbGLCoreRenderer