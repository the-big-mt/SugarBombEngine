/*
*******************************************************************************

Copyright (C) 1993-2012 id Software LLC, a ZeniMax Media company.
Copyright (C) 2018-2020, 2023 SugarBombEngine Developers

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

#include "SbSoundWorldLocal.hpp"
#include "SbSoundEmitter.hpp"
//#include "SbSoundChannel.hpp"

#include <CoreLibs/SbSystem/SbSystem.hpp>

//*****************************************************************************

namespace sbe::SbSound
{

SbSoundWorldLocal::SbSoundWorldLocal(SbSystem &aSystem) : mSystem(aSystem){}

void SbSoundWorldLocal::ClearAllEmitters()
{
};

/*
========================
idSoundWorldLocal::StopAllSounds

This is called from the main thread.
========================
*/
void SbSoundWorldLocal::StopAllSounds()
{
	for(int i = 0; i < mvEmitters.Num(); ++i)
		mvEmitters[i]->Reset();
};

/*
========================
idSoundWorldLocal::AllocSoundEmitter

This is called from the main thread.
========================
*/
SbSoundEmitter *SbSoundWorldLocal::AllocEmitter()
{
	auto pEmitter{new SbSoundEmitterLocal()};
	mvEmitters.push_back(pEmitter);
	return pEmitter;
};

/*
========================
idSoundWorldLocal::EmitterForIndex
========================
*/
SbSoundEmitter *SbSoundWorldLocal::GetEmitterByIndex(int anIndex) const
{
	// This is only used by save/load code which assumes index = 0 is invalid
	// Which is fine since we use index 0 for the local sound emitter anyway
	if(anIndex <= 0)
		return nullptr;
	
	if(anIndex >= mvEmitters.Num())
		mSystem.Error("idSoundWorldLocal::EmitterForIndex: %i >= %i", anIndex, mvEmitters.Num());

	return mvEmitters.at(anIndex); // mvEmitters[anIndex]
};

void SbSoundWorldLocal::Skip(int anTime)
{
};

void SbSoundWorldLocal::Update(float afTimeStep)
{
	for(auto It : mvEmitters)
		It->Update();
};

}; // namespace sbe::SbSound