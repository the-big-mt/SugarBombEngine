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

#pragma once

#include "CoreLibs/SbSound/ISoundWorld.hpp"

namespace sbe::SbSound
{

class SbSoundWorld : public ISoundWorld
{
public:
	void ClearAllEmitters() override;
	void StopAllSounds() override;
	
	ISoundEmitter *AllocEmitter() override;
	
	ISoundEmitter *GetEmitterByIndex(int anIndex) const override;
	
	void Skip(int anTime) override;
	
	void SetPaused(bool abPaused) override;
	bool IsPaused() const override;
private:
	bool mbPaused{false};
};

}; // namespace sbe::SbSound