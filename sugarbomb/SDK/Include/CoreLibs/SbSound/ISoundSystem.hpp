/*
*******************************************************************************

Copyright (C) 1993-2012 id Software LLC, a ZeniMax Media company.
Copyright (C) 2018-2019 SugarBombEngine Developers

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

//*****************************************************************************

#pragma once

//*****************************************************************************

namespace sbe
{

struct ISoundWorld;

struct ISoundSystem
{
	/// All non-hardware initialization
	virtual void Init(bool abUseCompression = true, int anMaxSamples = 5) = 0;
	
	/// Shutdown routine
	virtual void Shutdown() = 0;
	
	/// The renderWorld is used for visualization and light amplitude sampling
	virtual ISoundWorld *AllocWorld(/*IRenderWorld *rw*/) = 0;
	
	///
	virtual void FreeWorld(ISoundWorld *apWorld) = 0;
	
	/// Specifying nullptr will cause silence to be played
	virtual void SetPlayingWorld(ISoundWorld *apWorld) = 0;
	
	/// Some tools, like the sound dialog, may be used in both the game and the editor
	/// This can return nullptr, so check!
	virtual ISoundWorld *GetPlayingWorld() const = 0;
};

}; // namespace sbe