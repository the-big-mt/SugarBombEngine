/*
*******************************************************************************

Copyright (C) 2019 SugarBombEngine Developers

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

*******************************************************************************
*/

/// @file

//*****************************************************************************

#pragma once

//*****************************************************************************

namespace sbe
{

struct ISoundSystem
{
	///
	virtual void Init() = 0;
	
	///
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