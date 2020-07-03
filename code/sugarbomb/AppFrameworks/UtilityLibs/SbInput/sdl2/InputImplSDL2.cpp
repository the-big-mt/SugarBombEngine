/*
===========================================================================

Doom 3 GPL Source Code
Copyright (C) 1999-2011 id Software LLC, a ZeniMax Media company.
Copyright (C) 2012 dhewg (dhewm3)
Copyright (C) 2012 Robert Beckebans
Copyright (C) 2013 Daniel Gibson
Copyright (C) 2019 BlackPhrase

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
along with Doom 3 Source Code.	If not, see <http://www.gnu.org/licenses/>.

In addition, the Doom 3 Source Code is also subject to certain additional terms. You should have received a copy of these additional terms immediately following the terms and conditions of the GNU General Public License which accompanied the Doom 3 Source Code.  If not, please request a copy in writing from id Software at the address below.

If you have questions concerning this license or the applicable additional terms, you may contact in writing id Software LLC, c/o ZeniMax Media Inc., Suite 120, Rockville, Maryland 20850 USA.

===========================================================================
*/

/// @file

#include "SbInputImplSDL2.hpp"

IInputSystem *CreateInputSystem()
{
	static SbInputImplSDL2 InputSystem;
	return &InputSystem;
};

#include "sdl2_scancode_mappings.h"

static SDL_Scancode KeyNumToSDLScanCode( int keyNum )
{
	if( keyNum < K_JOY1 )
	{
		for( int i = 0; i < SDL_NUM_SCANCODES; ++i )
			if( scanCodeToKeyNum[i] == keyNum )
				return SDL_Scancode( i );
	};
	return SDL_SCANCODE_UNKNOWN;
};

SbInputImplSDL2::SbInputImplSDL2(ICommon *apCommon) : mpCommon(apCommon){}

/*
=================
Sys_InitInput
=================
*/
void SbInputImplSDL2::Init()
{
	int numJoysticks, i;
	
	kbd_polls.SetGranularity( 64 );
	memset( buttonStates, 0, sizeof( buttonStates ) );
	
	in_keyboard.SetModified();
	
	// GameController
	if( SDL_Init( SDL_INIT_GAMECONTROLLER ) )
		mpCommon->Printf( "Sys_InitInput: SDL_INIT_GAMECONTROLLER error: %s\n", SDL_GetError() );
		
	SDL_GameController* controller = nullptr;
	for( int i = 0; i < SDL_NumJoysticks(); ++i )
	{
		if( SDL_IsGameController( i ) )
		{
			controller = SDL_GameControllerOpen( i );
			if( controller )
			{
				mpCommon->Printf( "GameController %i name: %s\n", i, SDL_GameControllerName( controller ) );
				mpCommon->Printf( "GameController %i is mapped as \"%s\".\n", i, SDL_GameControllerMapping( controller ) );
			}
			else
				mpCommon->Printf( "Could not open gamecontroller %i: %s\n", i, SDL_GetError() );
		};
	};
};

/*
=================
Sys_ShutdownInput
=================
*/
void SbInputImplSDL2::Shutdown()
{
	kbd_polls.Clear();
	joystick_polls.Clear();
	
	memset( buttonStates, 0, sizeof( buttonStates ) );
	
	// Close any opened SDL Joystic
	if( joy )
	{
		mpCommon->Printf( "Sys_ShutdownInput: closing SDL joystick.\n" );
		SDL_JoystickClose( joy );
	}
	else
		mpCommon->Printf( "Sys_ShutdownInput: SDL joystick not initialized. Nothing to close.\n" );
};

const char *SbInputImplSDL2::GetKeyName(keyNum_t keynum)
{
	SDL_Scancode scancode = KeyNumToSDLScanCode( ( int )keynum );
	SDL_Keycode keycode = SDL_GetKeyFromScancode( scancode );
	
	const char *ret = SDL_GetKeyName( keycode );
	if( ret != nullptr && ret[0] != '\0' )
		return ret;

	return nullptr;
};

/*
================
Sys_PollKeyboardInputEvents
================
*/
int SbInputImplSDL2::PollKeyboardInputEvents()
{
	return kbd_polls.Num();
};

/*
================
Sys_ReturnKeyboardInputEvent
================
*/
int SbInputImplSDL2::ReturnKeyboardInputEvent( const int n, int& key, bool& state )
{
	if( n >= kbd_polls.Num() )
		return 0;
		
	key = kbd_polls[n].key;
	state = kbd_polls[n].state;
	return 1;
};

/*
================
Sys_EndKeyboardInputEvents
================
*/
void SbInputImplSDL2::EndKeyboardInputEvents()
{
	kbd_polls.SetNum( 0 );
};

/*
===============
Sys_GrabMouseCursor
===============
*/
void SbInputImplSDL2::GrabMouseCursor(bool grabIt)
{
	int flags{GRAB_SETSTATE};
	
	if( grabIt )
	{
		// DG: disabling the cursor is now done once in GLimp_Init() because it should always be disabled
		flags |= GRAB_ENABLE;
		// DG end
	};
	
	GLimp_GrabInput( flags );
};

//=====================================================================================
//	Joystick Input Handling
//=====================================================================================

void SbInputImplSDL2::SetRumble(int device, int low, int hi)
{
	// TODO;
	// SDL 2.0 required (SDL Haptic subsystem)
};

int SbInputImplSDL2::PollJoystickInputEvents(int deviceNum)
{
	int numEvents = joystick_polls.Num();
	
	return numEvents;
};

// This funcion called by void idUsercmdGenLocal::Joystick( int deviceNum ) in
// file UsercmdGen.cpp
// action - must have values belonging to enum sys_jEvents (sys_public.h)
// value - must be 1/0 for button or DPAD pressed/released
//         for joystick axes must be in the range (-32769, 32768)
//         for joystick trigger must be in the range (0, 32768)
int SbInputImplSDL2::ReturnJoystickInputEvent(const int n, int &action, int &value)
{
	// Get last element of the list and copy into argument references
	const joystick_poll_t& mp = joystick_polls[n];
	action = mp.action;
	value = mp.value;
	
	return 1;
};

// This funcion called by void idUsercmdGenLocal::Joystick( int deviceNum ) in
// file UsercmdGen.cpp
void SbInputImplSDL2::EndJoystickInputEvents()
{
	// Empty the joystick event container. This is called after
	// all joystick events have been read using Sys_ReturnJoystickInputEvent()
	joystick_polls.SetNum( 0 );
};