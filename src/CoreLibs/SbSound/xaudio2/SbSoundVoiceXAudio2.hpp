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

namespace sbe
{

struct ISystem;

namespace SbSound
{

class SbSoundSampleXAudio2;

class SbSoundVoiceXAudio2 final : public SbSoundVoice
{
public:
	SbSoundVoiceXAudio2(ISystem &aSystem);
	~SbSoundVoiceXAudio2();
public:
	void Create(const SbSoundSample *apLeadinSample, const SbSoundSample *apLoopingSample) override;
	
	void Start(int anOffsetMS, int anFlags) override;
	void Stop() override;
	
	void Pause() override;
	void UnPause() override;
	
	bool Update() override;
	
	float GetAmplitude() const override;
	
	bool CompatibleFormat(SbSoundSample *apSample) const override;
	
	uint32_t GetSampleRate() const override {return sampleRate;}
	
	void OnBufferStart(SbSoundSample *apSample, int anBuffer) override;
public:
	void FlushSourceBuffers();
private:
	void DestroyInternal();
	
	int SubmitBuffer();
	int SetSampleRate();
	
	int RestartAt(int anOffsetSamples);
	
	bool IsPlaying() const;
private:
	ISystem &mSystem;
	
	SbSoundSampleXAudio2 *leadinSample{nullptr};
	SbSoundSampleXAudio2 *loopingSample{nullptr};
	
	uint32_t sampleRate{0};
	
	bool paused{true};
};

};}; // namespace sbe::SbSound