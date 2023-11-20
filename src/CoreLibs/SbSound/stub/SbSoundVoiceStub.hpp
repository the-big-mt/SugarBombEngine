/*
*******************************************************************************

Copyright (C) 2019-2020 SugarBombEngine Developers

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

#include "SbSoundVoice.hpp"

//*****************************************************************************

namespace sbe::SbSound
{

class SbSoundVoiceStub : public SbSoundVoice
{
public:
	void Create(const SbSoundSample *apLeadinSample, const SbSoundSample *apLoopingSample) override {}
	
	void Start(int anOffsetMS, int anFlags) override {}
	void Stop() override {}
	
	void Pause() override {}
	void UnPause() override {}
	
	bool Update() override {return false;}
	
	float GetAmplitude() const override {return 0.0f}
	
	bool CompatibleFormat(SbSoundSample *apSample) const override {return false;}
	
	uint32_t GetSampleRate() const override {return 0;}
	
	void OnBufferStart(SbSoundSample *apSample, int anBuffer) override {}
};

}; // namespace sbe::SbSound