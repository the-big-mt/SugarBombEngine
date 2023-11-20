/*
*******************************************************************************

Copyright (C) 1993-2012 id Software LLC, a ZeniMax Media company.
Copyright (C) 2013-2014 Robert Beckebans
Copyright (C) 2019-2020 SugarBombEngine Developers

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

//*****************************************************************************

#include "SbRenderContextWGL.hpp"

#include "CoreLibs/SbSystem/ISystem.hpp"

#include <wgl.h>

//*****************************************************************************

namespace sbe::SbGLCoreRenderer
{

/*
========================
CreateOpenGLContextOnDC
========================
*/
HGLRC SbRenderContextWGL::CreateOpenGLContextOnDC(const HDC hdc, const bool debugContext)
{
	int useOpenGL32 = 2; // TODO: was r_useOpenGL32.GetInteger();
	HGLRC m_hrc = nullptr;
	
	// RB: for GLintercept 1.2.0 or otherwise we can't diff the framebuffers using the XML log
	if( !WGLEW_ARB_create_context || useOpenGL32 == 0 )
		return wglCreateContext( hdc );
	// RB end
	
	for( int i = 0; i < 2; i++ )
	{
		const int glMajorVersion = ( useOpenGL32 != 0 ) ? 3 : 2;
		const int glMinorVersion = ( useOpenGL32 != 0 ) ? 2 : 0;
		const int glDebugFlag = debugContext ? WGL_CONTEXT_DEBUG_BIT_ARB : 0;
		const int glProfileMask = ( useOpenGL32 != 0 ) ? WGL_CONTEXT_PROFILE_MASK_ARB : 0;
		const int glProfile = ( useOpenGL32 == 1 ) ? WGL_CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB : ( ( useOpenGL32 == 2 ) ? WGL_CONTEXT_CORE_PROFILE_BIT_ARB : 0 );
		const int attribs[] =
		{
			WGL_CONTEXT_MAJOR_VERSION_ARB,	glMajorVersion,
			WGL_CONTEXT_MINOR_VERSION_ARB,	glMinorVersion,
			WGL_CONTEXT_FLAGS_ARB,			glDebugFlag,
			glProfileMask,					glProfile,
			0
		};
		
		m_hrc = wglCreateContextAttribsARB( hdc, 0, attribs );
		if( m_hrc != nullptr )
		{
			mSystem.Printf( "created OpenGL %d.%d context\n", glMajorVersion, glMinorVersion );
			
			if( useOpenGL32 == 2 )
				glConfig.driverType = GLDRV_OPENGL32_CORE_PROFILE;
			else if( useOpenGL32 == 1 )
				glConfig.driverType = GLDRV_OPENGL32_COMPATIBILITY_PROFILE;
			else
				glConfig.driverType = GLDRV_OPENGL3X;
			
			break;
		};
		
		mSystem.Printf( "failed to create OpenGL %d.%d context\n", glMajorVersion, glMinorVersion );
		useOpenGL32 = 0; // fall back to OpenGL 2.0
	};
	
	if( m_hrc == nullptr )
	{
		int	err = GetLastError();
		switch( err )
		{
			case ERROR_INVALID_VERSION_ARB:
				mSystem.Printf( "ERROR_INVALID_VERSION_ARB\n" );
				break;
			case ERROR_INVALID_PROFILE_ARB:
				mSystem.Printf( "ERROR_INVALID_PROFILE_ARB\n" );
				break;
			default:
				mSystem.Printf( "unknown error: 0x%x\n", err );
				break;
		};
	};
	
	return m_hrc;
};

// TODO: abDebugContext = r_debugContext.GetBool()
SbRenderContextWGL::SbRenderContextWGL(ISystem &aSystem, HDC ahDeviceContext, const bool abDebugContext) : mSystem(aSystem)
{
	//
	// startup the OpenGL subsystem by creating a context and making it current
	//
	mSystem.Printf( "...creating GL context: " );
	mhGLRC = CreateOpenGLContextOnDC( ahDeviceContext, abDebugContext);
	if( mhGLRC == 0 )
	{
		mSystem.Printf( "^3failed^0\n" );
		return false;
	};
	mSystem.Printf( "succeeded\n" );
	
	mSystem.Printf( "...making context current: " );
	if( !wglMakeCurrent( ahDeviceContext, mhGLRC ) )
	{
		wglDeleteContext( mhGLRC );
		mhGLRC = nullptr;
		mSystem.Printf( "^3failed^0\n" );
		return false;
	};
	mSystem.Printf( "succeeded\n" );
};

SbRenderContextWGL::~SbRenderContextWGL()
{
	const char *success[] = {"failed", "success"};
	int retVal{-1};
	
	// set current context to nullptr
	//if( wglMakeCurrent )
	{
		retVal = wglMakeCurrent(nullptr, nullptr) != 0;
		mSystem.Printf("...wglMakeCurrent( nullptr, nullptr ): %s\n", success[retVal]);
	};
	
	// delete HGLRC
	if(mhGLRC)
	{
		retVal = wglDeleteContext(mhGLRC) != 0;
		mSystem.Printf("...deleting GL context: %s\n", success[retVal]);
		mhGLRC = nullptr;
	};
};

}; // namespace sbe::SbGLCoreRenderer