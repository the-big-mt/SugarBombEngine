/*
===========================================================================

Doom 3 BFG Edition GPL Source Code
Copyright (C) 1993-2012 id Software LLC, a ZeniMax Media company.
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

#pragma once

typedef enum
{
	INHIBIT_SESSION = 0,
	INHIBIT_ASYNC
} inhibit_t;

class idUsercmdGen
{
public:
	virtual				~idUsercmdGen() {}
	
	// Sets up all the cvars and console commands.
	virtual	void		Init() = 0;
	
	// Prepares for a new map.
	virtual void		InitForNewMap() = 0;
	
	// Shut down.
	virtual void		Shutdown() = 0;
	
	// Clears all key states and face straight.
	virtual	void		Clear() = 0;
	
	// Clears view angles.
	virtual void		ClearAngles() = 0;
	
	// When the console is down or the menu is up, only emit default usercmd, so the player isn't moving around.
	// Each subsystem (session and game) may want an inhibit will OR the requests.
	virtual void		InhibitUsercmd( inhibit_t subsystem, bool inhibit ) = 0;
	
	// Set a value that can safely be referenced by UsercmdInterrupt() for each key binding.
	virtual	int			CommandStringUsercmdData( const char* cmdString ) = 0;
	
	// Continuously modified, never reset. For full screen guis.
	virtual void		MouseState( int* x, int* y, int* button, bool* down ) = 0;
	
	// Directly sample a button.
	virtual int			ButtonState( int key ) = 0;
	
	// Directly sample a keystate.
	virtual int			KeyState( int key ) = 0;
	
	// called at vsync time
	virtual void		BuildCurrentUsercmd( int deviceNum ) = 0;
	
	// return the current usercmd
	virtual usercmd_t	GetCurrentUsercmd() = 0; // TODO: return by ref?
};