/*
*******************************************************************************

Copyright (C) 2019-2020, 2023 SugarBombEngine Developers

This file is part of SugarBombEngine

SugarBombEngine is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

SugarBombEngine is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with SugarBombEngine. If not, see <http://www.gnu.org/licenses/>.

*******************************************************************************
*/

/// @file

//*****************************************************************************

#pragma once

#include <CoreLibs/SbSound/SbSoundSystem.hpp>

#include <CoreLibs/SbMain/SbStaticList.hpp>

//*****************************************************************************

namespace sbe
{

struct SbSystem;

namespace SbSound
{

struct SbSoundHardware;
struct SbSoundWorld;

using tSoundWorldList = SbStaticList<SbSoundWorld*, 32>;

class SbSoundSystemLocal : public SbSoundSystem
{
public:
	SbSoundSystemLocal(SbSystem &aSystem, SbSoundHardware &aHardware);
	
	void Init(bool abUseCompression, int anMaxSamples) override;
	void Shutdown() override;
	
	void Update(float afTimeStep) override;
	
	SbSoundWorld *AllocWorld() override;
	void FreeWorld(SbSoundWorld *apWorld) override;
	
	void SetPlayingWorld(SbSoundWorld *apWorld) override;
	SbSoundWorld *GetPlayingWorld() const override;
	
	void BeginLevelLoad() override;
	void EndLevelLoad() override;
private:
	tSoundWorldList mlstWorlds;
	
	SbSystem &mSystem;
	SbSoundHardware &mHardware;
	
	SbSoundWorld *mpActiveWorld{nullptr};
};

//extern SbSoundSystemLocal gSoundSystemLocal;

};}; // namespace sbe::SbSound