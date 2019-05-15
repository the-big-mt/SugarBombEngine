/*
*******************************************************************************

Copyright (C) 1993-2012 id Software LLC, a ZeniMax Media company.
Copyright (C) 2014-2016 Robert Beckebans
Copyright (C) 2014-2016 Kot in Action Creative Artel
Copyright (C) 2019 SugarBombEngine Developers

This file is part of SugarBombEngine.

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
subject to certain additional terms. You should have received a copy
of these additional terms immediately following the terms and conditions
of the GNU General Public License which accompanied the id Tech 4 source code.
If not, please request a copy in writing from id Software at the address below.

If you have questions concerning this license or the applicable additional
terms, you may contact in writing id Software LLC, c/o ZeniMax Media Inc.,
Suite 120, Rockville, Maryland 20850 USA.

*******************************************************************************
*/

/// @file
/// @brief game framework interface

#pragma once

namespace sbe
{

struct IGameFramework
{
	/// Initialize everything.
	/// if the OS allows, pass argc/argv directly (without executable name)
	/// otherwise pass the command line in a single string (without executable name)
	virtual void				Init( int argc, const char* const* argv, const char* cmdline ) = 0;
	
	/// Shuts down everything
	virtual void				Shutdown() = 0;
	
	/// Called repeatedly as the foreground thread for rendering and game logic
	virtual void				Frame() = 0;
	
	/// Shuts down everything
	virtual void				Quit() = 0;
	
	/// Returns true if common initialization is complete
	virtual bool				IsInitialized() const = 0;
	
	virtual bool				IsShuttingDown() const = 0;
	
	/// DG: added possibility to *not* release mouse in UpdateScreen(), it fucks up the view angle for screenshots
	/// Redraws the screen, handling games, guis, console, etc
	/// in a modal manner outside the normal frame loop
	virtual void				UpdateScreen( bool captureToImage, bool releaseMouse = true ) = 0;
	// DG end
	
	virtual void				UpdateLevelLoadPacifier() = 0;
	//virtual void				UpdateLevelLoadPacifier( int mProgress ) = 0;
	//virtual void				UpdateLevelLoadPacifier( bool updateSecondary ) = 0;
	//virtual void				UpdateLevelLoadPacifier( bool updateSecondary, int Progress ) = 0;
	
	/// Processes the given event
	virtual	bool				ProcessEvent( const sysEvent_t* event ) = 0;
};

}; // namespace sbe