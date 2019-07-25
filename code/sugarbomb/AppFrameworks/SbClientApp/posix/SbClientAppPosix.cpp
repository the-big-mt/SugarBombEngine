/*
===========================================================================

Doom 3 BFG Edition GPL Source Code
Copyright (C) 1993-2012 id Software LLC, a ZeniMax Media company. 
Copyright (C) 2012 Robert Beckebans
Copyright (C) 2019 BlackPhrase

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

/// @file

SbApplicationPosix::~SbApplicationPosix()
{
	if( window )
	{
		SDL_DestroyWindow( window );
		window = nullptr;
	};
};

void SbApplicationPosix::Init()
{
#ifdef __linux__
#ifdef USE_BREAKPAD
	google_breakpad::MinidumpDescriptor descriptor( "/tmp" );
	google_breakpad::ExceptionHandler eh( descriptor, nullptr, BFG::breakpad_dumpCallback, nullptr, true, -1 );
#endif
#endif

#if defined(__linux__) || defined(__APPLE__) // posix
	// DG: needed for Sys_ReLaunch()
	cmdargc = argc;
	cmdargv = argv;
	// DG end
#endif

#ifdef __linux__
#ifdef ID_MCHECK
	// must have -lmcheck linkage
	mcheck( abrt_func );
	Sys_Printf( "memory consistency checking enabled\n" );
#endif
#endif

#if defined(__linux__) || defined(__APPLE__) // posix
	Posix_EarlyInit( );

	if( argc > 1 )
		common->Init( argc - 1, &argv[1], nullptr );
	else
		common->Init( 0, nullptr, nullptr );

	Posix_LateInit( );
#endif // defined(__linux__) || defined(__APPLE__)
};

void SbApplicationPosix::CreateMainWindow(int anWidth, int anHeight, const char *asTitle, bool abFullScreen)
{
	GLimp_PreInit(); // DG: make sure SDL is initialized

	// DG: make window resizable
	Uint32 flags = SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE;
	// DG end
	
	if( abFullScreen )
		flags |= SDL_WINDOW_FULLSCREEN;
};