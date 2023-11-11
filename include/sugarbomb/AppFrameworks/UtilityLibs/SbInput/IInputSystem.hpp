/*
*******************************************************************************

Copyright (C) 1993-2012 id Software LLC, a ZeniMax Media company.
Copyright (C) 2012(?) Daniel Gibson // TODO
Copyright (C) 2019-2020, 2023 SugarBombEngine Developers

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
/// @brief input system interface

//*****************************************************************************

#pragma once

//#include "SbInputTypes.hpp" // TODO

//*****************************************************************************

namespace sbe
{

struct IWindow;

struct SbKeyboard;
struct SbMouse;
struct SbGamepad;

/*
// input is tied to windows, so it needs to be started up and shut down whenever
// the main window is recreated
//void Sys_InitInput();
//void Sys_ShutdownInput();

// when the console is down, or the game is about to perform a lengthy
// operation like map loading, the system can release the mouse cursor
// when in windowed mode
void			Sys_GrabMouseCursor( bool grabIt );
*/

struct IInputSystem
{
	/**
	 * Initializes the input system
	 *
	 * @param aOwnerWindow - a reference to the window that the input system will attach to
	 */
	virtual void Init(const IWindow &aOwnerWindow) = 0;

	/// Shuts down the input system
	virtual void Shutdown() = 0;

	/// Updates the system
	virtual void Update() = 0;
	
	///
	//virtual SbKeyboard &GetKeyboard() const = 0;
	
	///
	//virtual SbMouse &GetMouse() const = 0;
	
	///
	//virtual SbGamepad &GetGamepad(int anIndex) const = 0;
	
	///
	//virtual void GrabMouseCursor(bool grabIt) = 0; // TODO
	
	// DG: currently this is only used by idKeyInput::LocalizedKeyName() for !windows
#ifndef _WIN32
	/// Return a human readable name for the key in the current keyboard layout (keynum is a directinput scancode)
	virtual const char *GetKeyName(keyNum_t keynum) const = 0; // TODO: SbKeyboard::GetKeyName()?
#endif
	// DG end
};

}; // namespace sbe