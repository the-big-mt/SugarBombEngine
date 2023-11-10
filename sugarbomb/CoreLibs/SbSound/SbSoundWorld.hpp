/*
*******************************************************************************

Copyright (C) 1993-2012 id Software LLC, a ZeniMax Media company.
Copyright (C) 2018-2020 SugarBombEngine Developers

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

#pragma once

#include <vector>

#include "CoreLibs/SbSound/ISoundWorld.hpp"

//*****************************************************************************

namespace sbe
{

struct ISystem;

namespace SbSound
{

struct SbSoundEmitter;
using tSoundEmitterVec = std::vector<SbSoundEmitter*>;

class SbSoundChannel;

class SbSoundWorld : public ISoundWorld
{
public:
	SbSoundWorld(ISystem &aSystem);
	
	//------------------------
	// Functions from ISoundWorld, implemented in SbSoundWorld.cpp
	//------------------------
	
	/// Called at map start
	/*virtual*/ void ClearAllEmitters() override;
	
	/// Stop all playing sounds
	/*virtual*/ void StopAllSounds() override;
	
	/// Get a new emitter that can play sounds in this world
	/*virtual*/ ISoundEmitter *AllocEmitter() override;
	
	/// For load games
	/*virtual*/ ISoundEmitter *GetEmitterByIndex(int anIndex) const override;
	
	/*virtual*/ void Skip(int anTime) override;
	
	/*virtual*/ void SetPaused(bool abPaused) override {mbPaused = abPaused;}
	/*virtual*/ bool IsPaused() const override {return mbPaused;}
	
	//=======================================
public:
	//------------------------
	// Random stuff that's not exposed outside the sound system
	//------------------------
	
	void Update(float afTimeStep);
	
	//void OnReloadSound(const idDecl *decl); // TODO
	
	SbSoundChannel *AllocChannel();
	void FreeChannel(SbSoundChannel *apChannel);
private:
	tSoundEmitterVec mvEmitters;
	
	// TODO
	//tSoundChannelVec mvIdleChannels;
	//tSoundChannelVec mvActiveChannels;
	
	ISystem &mSystem;
	
	bool mbPaused{false};
};

};}; // namespace sbe::SbSound