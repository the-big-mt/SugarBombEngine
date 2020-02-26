/*
*******************************************************************************

Copyright (C) 1993-2012 id Software LLC, a ZeniMax Media company.
Copyright (C) 2012 Robert Beckebans
Copyright (C) 2019 BlackPhrase

This file is part of SugarBombEngine

SugarBombEngine is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

SugarBombEngine is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with SugarBombEngine. If not, see <http://www.gnu.org/licenses/>.

In addition, SugarBombEngine is using id Tech 4 (BFG) pieces and thus
subject to certain additional terms (all header and source files which 
contains such pieces has this additional part appended to the license 
header). You should have received a copy of these additional terms 
stated in a separate file (LICENSE-idTech4) which accompanied the 
SugarBombEngine source code. If not, please request a copy in 
writing from id Software at the address below.

If you have questions concerning this license or the applicable additional
terms, you may contact in writing id Software LLC, c/o ZeniMax Media Inc.,
Suite 120, Rockville, Maryland 20850 USA.

*******************************************************************************
*/

/// @file

#pragma once

#include "SbInputTypes.hpp"

namespace sbe
{

// input is tied to windows, so it needs to be started up and shut down whenever
// the main window is recreated
void			Sys_InitInput();
void			Sys_ShutdownInput();

// keyboard input polling
int				Sys_PollKeyboardInputEvents();
int				Sys_ReturnKeyboardInputEvent( const int n, int& ch, bool& state );
void			Sys_EndKeyboardInputEvents();

// DG: currently this is only used by idKeyInput::LocalizedKeyName() for !windows
#ifndef _WIN32
// return a human readable name for the key in the current keyboard layout (keynum is a directinput scancode)
const char*		Sys_GetKeyName( keyNum_t keynum );
#endif
// DG end

// mouse input polling
static const int MAX_MOUSE_EVENTS = 256;
int				Sys_PollMouseInputEvents( int mouseEvents[MAX_MOUSE_EVENTS][2] );

// joystick input polling
void			Sys_SetRumble( int device, int low, int hi );
int				Sys_PollJoystickInputEvents( int deviceNum );
int				Sys_ReturnJoystickInputEvent( const int n, int& action, int& value );
void			Sys_EndJoystickInputEvents();

// when the console is down, or the game is about to perform a lengthy
// operation like map loading, the system can release the mouse cursor
// when in windowed mode
void			Sys_GrabMouseCursor( bool grabIt );

struct IInputSystem
{
	///
	virtual void Init() = 0;
	
	///
	virtual void Shutdown() = 0;
	
	///
	virtual const char *GetKeyName(keyNum_t keynum) const = 0;
	
	///
	virtual int PollKeyboardInputEvents() = 0;
	
	///
	virtual int ReturnKeyboardInputEvent(const int n, int &key, bool &state) = 0;
	
	///
	virtual void EndKeyboardInputEvents() = 0;
	
	///
	virtual void GrabMouseCursor(bool grabIt) = 0;
	
	///
	virtual int PollMouseInputEvents(int mouseEvents[MAX_MOUSE_EVENTS][2]) = 0;
	
	///
	virtual void SetRumble(int device, int low, int hi) = 0;
	
	///
	virtual int PollJoystickInputEvents(int deviceNum) = 0;
	
	///
	virtual int ReturnJoystickInputEvent(const int n, int &action, int &value) = 0;
	
	///
	virtual void EndJoystickInputEvents() = 0;
};

/*
===============================================================================

	Input API

===============================================================================
*/

const auto INPUT_API_VERSION{1};

struct ISys;
struct idCmdSystem;
struct idCVarSystem;

struct inputImport_t
{
	int version{-1}; ///< API version
	sbe::ISys *sys{nullptr}; ///< system
	idCmdSystem *cmdSystem{nullptr}; ///< console command system
	idCVarSystem *cvarSystem{nullptr}; ///< console variable system
	//idFileSystem *fileSystem{nullptr}; ///< file system
};

struct inputExport_t
{
	int version{-1}; ///< API version
	IInputSystem *inputSystem{nullptr};
};

extern "C" using GetInputAPI_t = inputExport_t *(*)(inputImport_t *import);

}; // namespace sbe