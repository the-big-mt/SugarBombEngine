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

class SbSoundSampleOpenAL;

class SbSoundVoiceOpenAL final : public SbSoundVoice
{
public:
	SbSoundVoiceOpenAL(ISystem &aSystem);
	~SbSoundVoiceOpenAL();
public:
	void Create(const SbSoundSample *apLeadinSample, const SbSoundSample *apLoopingSample) override;
	
	void Start(int anOffsetMS, int anFlags) override;
	void Stop() override;
	
	void Pause() override;
	void UnPause() override;
	
	bool Update() override;
	
	float GetAmplitude() const override;
	
	bool CompatibleFormat(SbSoundSample *apSample) const override;
	
	uint32_t GetSampleRate() const override;
	
	void OnBufferStart(SbSoundSample *apSample, int anBuffer) override;
public:
	void FlushSourceBuffers();
private:
	void DestroyInternal();
	
	int RestartAt(int anOffsetSamples);
	
	int SubmitBuffer();
	
	bool IsPlaying() const;
private:
	ISystem &mSystem;
	
	SbSoundSampleOpenAL *leadinSample{nullptr};
	SbSoundSampleOpenAL *loopingSample{nullptr};
	
	bool paused{true};
};

}; // namespace sbe::SbSound