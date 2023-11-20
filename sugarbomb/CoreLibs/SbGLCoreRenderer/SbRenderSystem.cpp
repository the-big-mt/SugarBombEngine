/*
*******************************************************************************

Copyright (C) 1993-2012 id Software LLC, a ZeniMax Media company.
Copyright (C) 2014-2016 Robert Beckebans
Copyright (C) 2014-2016 Kot in Action Creative Artel
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

#include "SbRenderSystem.hpp"
#include "SbRenderWorld.hpp"

#include "CoreLibs/SbSystem/ISystem.hpp"

#include "AppFrameworks/SbClientApp/IWindow.hpp"

//*****************************************************************************

namespace sbe::SbGLCoreRenderer
{

SbRenderSystem::SbRenderSystem(const IWindow &aWindow, ISystem &aSystem) : mWindow(aWindow), mSystem(aSystem){}

void SbRenderSystem::Init()
{
	mSystem.Printf("------- Initializing renderSystem --------\n");
	
	InitOpenGL();
	
	mbInitialized = true;
	
	mSystem.Printf("renderSystem initialized.\n");
	mSystem.Printf("--------------------------------------\n");
};

/*
===============
idRenderSystemLocal::Shutdown
===============
*/
void SbRenderSystem::Shutdown()
{
	mSystem.Printf("idRenderSystem::Shutdown()\n");

	//fonts.DeleteContents(); // TODO

	//if(IsInitialized()) // TODO
		//globalImages->PurgeAllImages(); // TODO
	
	//renderModelManager->Shutdown(); // TODO

	//SbCinematic::ShutdownCinematic(); // TODO

	//globalImages->Shutdown(); // TODO
	
	// RB begin
	//Framebuffer::Shutdown(); // TODO
	// RB end

	// free frame memory
	//R_ShutdownFrameData(); // TODO

	//UnbindBufferObjects(); // TODO
	
	// free the vertex cache, which should have nothing allocated now
	//vertexCache.Shutdown(); // TODO

	//RB_ShutdownDebugTools(); // TODO

	//delete guiModel; // TODO

	//parallelJobManager->FreeJobList(frontEndJobList); // TODO
	
	Clear();
	
	ShutdownOpenGL();
	
	mbInitialized = false;
};

void SbRenderSystem::Restart()
{
	// if OpenGL isn't started, do nothing
	if(!IsInitialized())
		return;
	
	// set the mode without re-initializing the context
	//R_SetNewMode(false); // TODO

#if 0
	bool full = true;
	bool forceWindow = false;
	for( int i = 1 ; i < args.Argc() ; i++ )
	{
		if( idStr::Icmp( args.Argv( i ), "partial" ) == 0 )
		{
			full = false;
			continue;
		}
		if( idStr::Icmp( args.Argv( i ), "windowed" ) == 0 )
		{
			forceWindow = true;
			continue;
		};
	};
	
	// this could take a while, so give them the cursor back ASAP
	Sys_GrabMouseCursor( false );
	
	// dump ambient caches
	renderModelManager->FreeModelVertexCaches();
	
	// free any current world interaction surfaces and vertex caches
	R_FreeDerivedData();
	
	// make sure the defered frees are actually freed
	R_ToggleSmpFrame();
	R_ToggleSmpFrame();
	
	// free the vertex caches so they will be regenerated again
	vertexCache.PurgeAll();
	
	// sound and input are tied to the window we are about to destroy
	
	if( full )
	{
		// free all of our texture numbers
		//Sys_ShutdownInput(); // TODO: wrong place
		globalImages->PurgeAllImages();
		// free the context and close the window
		GLimp_Shutdown();
		mbInitialized = false;
		
		// create the new context and vertex cache
		bool latch = cvarSystem->GetCVarBool( "r_fullscreen" );
		if( forceWindow )
			cvarSystem->SetCVarBool( "r_fullscreen", false );

		InitOpenGL();
		cvarSystem->SetCVarBool( "r_fullscreen", latch );
		
		// regenerate all images
		globalImages->ReloadImages( true );
	}
	else
	{
		glimpParms_t parms;
		parms.width = glConfig.nativeScreenWidth;
		parms.height = glConfig.nativeScreenHeight;
		parms.fullScreen = ( forceWindow ) ? false : r_fullscreen.GetInteger();
		parms.displayHz = r_displayRefresh.GetInteger();
		parms.multiSamples = r_multiSamples.GetInteger();
		parms.stereo = false;
		GLimp_SetScreenParms( parms );
	};
	
	// make sure the regeneration doesn't use anything no longer valid
	tr.viewCount++;
	tr.viewDef = nullptr;
	
	// check for problems
	int err = glGetError();
	if( err != GL_NO_ERROR )
		mSystem.Printf( "glGetError() = 0x%x\n", err );
#endif
};

IRenderWorld *SbRenderSystem::AllocWorld()
{
	auto pWorld{new SbRenderWorld()};
	mlstWorlds.push_back(pWorld);
	return pWorld;
};

void SbRenderSystem::FreeWorld(IRenderWorld *apWorld)
{
	if(apWorld)
	{
		delete apWorld;
		apWorld = nullptr;
	};
};

/*
===============
idRenderSystemLocal::Clear
===============
*/
void SbRenderSystem::Clear()
{
};

/*
========================
idRenderSystemLocal::InitOpenGL
========================
*/
/*
==================
R_InitOpenGL

This function is responsible for initializing a valid OpenGL subsystem
for rendering.  This is done by calling the system specific GLimp_Init,
which gives us a working OGL subsystem, then setting all necessary openGL
state, including images, vertex programs, and display lists.

Changes to the vertex cache size or smp state require a vid_restart.

If R_IsInitialized() is false, no rendering can take place, but
all renderSystem functions will still operate properly, notably the material
and model information functions.
==================
*/
void SbRenderSystem::InitOpenGL()
{
	// if OpenGL isn't started, start it now
	if(!IsInitialized())
	{
		//R_InitOpenGL
		{
			mSystem.Printf("----- R_InitOpenGL -----\n");
			
			//if(IsInitialized())
				//mSystem.FatalError("R_InitOpenGL called while active");

			// TODO: update the license header
			// DG: make sure SDL has setup video so getting supported modes in R_SetNewMode() works
			GLimp_PreInit();
			// DG end
			
			//R_SetNewMode(true); // TODO
			glimpParms_t InitParms{}; // TODO
			GLimp_Init(InitParms);
			
			// get our config strings
			glConfig.vendor_string = (const char *)glGetString(GL_VENDOR);
			glConfig.renderer_string = (const char *)glGetString(GL_RENDERER);
			glConfig.version_string = (const char *)glGetString(GL_VERSION);
			glConfig.shading_language_string = (const char *)glGetString(GL_SHADING_LANGUAGE_VERSION);
			glConfig.extensions_string = (const char *)glGetString(GL_EXTENSIONS);
			
			if(glConfig.extensions_string == nullptr)
			{
				// As of OpenGL 3.2, glGetStringi is required to obtain the available extensions
				//glGetStringi = ( PFNGLGETSTRINGIPROC )GLimp_ExtensionPointer( "glGetStringi" );

				// Build the extensions string
				GLint numExtensions;
				glGetIntegerv(GL_NUM_EXTENSIONS, &numExtensions);
				idStr extensions_string;
				extensions_string.Clear();
				for(int i = 0; i < numExtensions; i++)
				{
					extensions_string.Append((const char *)glGetStringi(GL_EXTENSIONS, i));
					// the now deprecated glGetString method usaed to create a single string with each extension separated by a space
					if(i < numExtensions - 1)
						extensions_string.Append(' ');
				};
				glConfig.extensions_string = extensions_string.c_str();
			};

			float glVersion = atof(glConfig.version_string);
			float glslVersion = atof(glConfig.shading_language_string);
			mSystem.Printf("OpenGL Version   : %3.1f\n", glVersion);
			mSystem.Printf("OpenGL Vendor    : %s\n", glConfig.vendor_string);
			mSystem.Printf("OpenGL Renderer  : %s\n", glConfig.renderer_string);
			mSystem.Printf("OpenGL GLSL      : %3.1f\n", glslVersion);
			mSystem.Printf("OpenGL Extensions: %s\n", glConfig.extensions_string);
			
			// OpenGL driver constants
			GLint temp;
			glGetIntegerv(GL_MAX_TEXTURE_SIZE, &temp);
			glConfig.maxTextureSize = temp;

			// stubbed or broken drivers may have reported 0...
			if(glConfig.maxTextureSize <= 0)
				glConfig.maxTextureSize = 256;
			
			// recheck all the extensions (FIXME: this might be dangerous)
			//R_CheckPortableExtensions(); // TODO

			//renderProgManager.Init(); // TODO
			
			// allocate the vertex array range or vertex objects
			//vertexCache.Init(); // TODO

			// allocate the frame data, which may be more if smp is enabled
			//R_InitFrameData(); // TODO

			// Reset our gamma
			//R_SetColorMappings(); // TODO
			
			// TODO: PostInit()?
			// RB begin
#if defined(_WIN32)
			static bool glCheck = false;
			if(!glCheck && win32.osversion.dwMajorVersion == 6)
			{
				glCheck = true;
				if(!idStr::Icmp(glConfig.vendor_string, "Microsoft") && idStr::FindText(glConfig.renderer_string, "OpenGL-D3D") != -1)
				{
					if(cvarSystem->GetCVarBool("r_fullscreen"))
					{
						cmdSystem->BufferCommandText(CMD_EXEC_NOW, "vid_restart partial windowed\n");
						Sys_GrabMouseCursor(false);
					}
					int ret = MessageBox(nullptr, "Please install OpenGL drivers from your graphics hardware vendor to run " GAME_NAME ".\nYour OpenGL functionality is limited.",
										 "Insufficient OpenGL capabilities", MB_OKCANCEL | MB_ICONWARNING | MB_TASKMODAL);
					if(ret == IDCANCEL)
					{
						cmdSystem->BufferCommandText(CMD_EXEC_APPEND, "quit\n");
						cmdSystem->ExecuteCommandBuffer();
					}
					if(cvarSystem->GetCVarBool("r_fullscreen"))
					{
						cmdSystem->BufferCommandText(CMD_EXEC_APPEND, "vid_restart\n");
					}
				}
			}
#endif
			// RB end
		};
		
		// Reloading images here causes the rendertargets to get deleted. Figure out how to handle this properly on 360
		//globalImages->ReloadImages(true); // TODO

		int err = glGetError();
		if(err != GL_NO_ERROR)
			mSystem.Printf("glGetError() = 0x%x\n", err);
	};
};

/*
========================
idRenderSystemLocal::ShutdownOpenGL
========================
*/
void SbRenderSystem::ShutdownOpenGL()
{
	// free the context
	//R_ShutdownFrameData(); // TODO
	
	//backend.Shutdown(); // TODO
};

void SbRenderSystem::BeginLevelLoad()
{
};

void SbRenderSystem::EndLevelLoad()
{
};

}; // namespace sbe::SbGLCoreRenderer