/*
===========================================================================

Doom 3 BFG Edition GPL Source Code
Copyright (C) 1993-2012 id Software LLC, a ZeniMax Media company.
Copyright (C) 2013-2016 Robert Beckebans
Copyright (C) 2014-2016 Kot in Action Creative Artel

This file is part of the Doom 3 BFG Edition GPL Source Code ("Doom 3 BFG Edition Source Code").

Doom 3 BFG Edition Source Code is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

Doom 3 BFG Edition Source Code is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with Doom 3 BFG Edition Source Code.  If not, see <http://www.gnu.org/licenses/>.

In addition, the Doom 3 BFG Edition Source Code is also subject to certain additional terms. You should have received a copy of these additional terms immediately following the terms and conditions of the GNU General Public License which accompanied the Doom 3 BFG Edition Source Code.  If not, please request a copy in writing from id Software at the address below.

If you have questions concerning this license or the applicable additional terms, you may contact in writing id Software LLC, c/o ZeniMax Media Inc., Suite 120, Rockville, Maryland 20850 USA.

===========================================================================
*/

#ifndef __RENDERER_H__
#define __RENDERER_H__

#include "renderer/IRenderSystem.hpp"

//namespace BFG
//{
enum graphicsVendor_t
{
	VENDOR_NVIDIA,
	VENDOR_AMD,
	VENDOR_INTEL
};

// RB: similar to Q3A - allow separate codepaths between OpenGL 3.x, OpenGL ES versions
enum graphicsDriverType_t
{
	GLDRV_OPENGL3X,							// best for development with legacy OpenGL tools
	GLDRV_OPENGL32_COMPATIBILITY_PROFILE,
	GLDRV_OPENGL32_CORE_PROFILE,			// best for shipping to PC
	GLDRV_OPENGL_ES2,
	GLDRV_OPENGL_ES3,
	GLDRV_OPENGL_MESA,						// fear this, it is probably the best to disable GPU skinning and run shaders in GLSL ES 1.0
	GLDRV_OPENGL_MESA_CORE_PROFILE
};

enum antiAliasingMode_t
{
	ANTI_ALIASING_NONE,
	ANTI_ALIASING_SMAA_1X,
	ANTI_ALIASING_MSAA_2X,
	ANTI_ALIASING_MSAA_4X,
	ANTI_ALIASING_MSAA_8X
};
// RB end

// Contains variables specific to the OpenGL configuration being run right now.
// These are constant once the OpenGL subsystem is initialized.
struct glconfig_t
{
	const char* 		renderer_string;
	const char* 		vendor_string;
	const char* 		version_string;
	const char* 		extensions_string;
	const char* 		wgl_extensions_string;
	const char* 		shading_language_string;
	
	float				glVersion;				// atof( version_string )
	graphicsVendor_t	vendor;
	// RB begin
	graphicsDriverType_t driverType;
	// RB end
	
	int					maxTextureSize;			// queried from GL
	int					maxTextureCoords;
	int					maxTextureImageUnits;
	int					uniformBufferOffsetAlignment;
	float				maxTextureAnisotropy;
	
	int					colorBits;
	int					depthBits;
	int					stencilBits;
	
	bool				multitextureAvailable;
	bool				directStateAccess;
	bool				textureCompressionAvailable;
	bool				anisotropicFilterAvailable;
	bool				textureLODBiasAvailable;
	bool				seamlessCubeMapAvailable;
	bool				sRGBFramebufferAvailable;
	bool				vertexBufferObjectAvailable;
	bool				mapBufferRangeAvailable;
	bool				vertexArrayObjectAvailable;
	bool				drawElementsBaseVertexAvailable;
	bool				fragmentProgramAvailable;
	bool				glslAvailable;
	bool				uniformBufferAvailable;
	bool				twoSidedStencilAvailable;
	bool				depthBoundsTestAvailable;
	bool				syncAvailable;
	bool				timerQueryAvailable;
	bool				occlusionQueryAvailable;
	bool				debugOutputAvailable;
	bool				swapControlTearAvailable;
	
	// RB begin
	bool				gremedyStringMarkerAvailable;
	bool				vertexHalfFloatAvailable;
	
	bool				framebufferObjectAvailable;
	int					maxRenderbufferSize;
	int					maxColorAttachments;
//	bool				framebufferPackedDepthStencilAvailable;
	bool				framebufferBlitAvailable;
	
	// only true with uniform buffer support and an OpenGL driver that supports GLSL >= 1.50
	bool				gpuSkinningAvailable;
	// RB end
	
	stereo3DMode_t		stereo3Dmode;
	int					nativeScreenWidth; // this is the native screen width resolution of the renderer
	int					nativeScreenHeight; // this is the native screen height resolution of the renderer
	
	int					displayFrequency;
	
	int					isFullscreen;					// monitor number
	bool				isStereoPixelFormat;
	bool				stereoPixelFormatAvailable;
	int					multisamples;
	
	// Screen separation for stereoscopic rendering is set based on this.
	// PC vid code sets this, converting from diagonals / inches / whatever as needed.
	// If the value can't be determined, set something reasonable, like 50cm.
	float				physicalScreenWidthInCentimeters;
	
	float				pixelAspect;
	
	// RB begin
#if !defined(__ANDROID__)
	GLuint				global_vao;
#endif
	// RB end
};



struct emptyCommand_t;

bool R_IsInitialized();

const int SMALLCHAR_WIDTH		= 8;
const int SMALLCHAR_HEIGHT		= 16;
const int BIGCHAR_WIDTH			= 16;
const int BIGCHAR_HEIGHT		= 16;

// all drawing is done to a 640 x 480 virtual screen size
// and will be automatically scaled to the real resolution
const int SCREEN_WIDTH			= 640;
const int SCREEN_HEIGHT			= 480;

extern idCVar r_useVirtualScreenResolution;

#include "renderer/IRenderSystem.hpp"

extern idRenderSystem* 			renderSystem;

//
// functions mainly intended for editor and dmap integration
//

// for use by dmap to do the carving-on-light-boundaries and for the editor for display
void R_LightProjectionMatrix( const idVec3& origin, const idPlane& rearPlane, idVec4 mat[4] );

// used by the view shot taker
void R_ScreenshotFilename( int& lastNumber, const char* base, idStr& fileName );

//} // namespace BFG

#endif /* !__RENDERER_H__ */
