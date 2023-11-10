/*
*******************************************************************************

Copyright (C) 1993-2012 id Software LLC, a ZeniMax Media company.
Copyright (C) 2019-2020 SugarBombEngine Developers

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

//*****************************************************************************

namespace sbe::SbSound
{

struct SbSoundVoice
{
	///
	virtual void Create(const SbSoundSample *apLeadinSample, const SbSoundSample *apLoopingSample) = 0; // TODO: is this creates itself?
	
	/// Start playing at a particular point in the buffer.  Does an Update() too
	virtual void Start(int anOffsetMS, int anFlags) = 0;
	
	/// Stop playing.
	virtual void Stop() = 0;
	
	/// Stop consuming buffers
	virtual void Pause() = 0;
	
	/// Start consuming buffers again
	virtual void UnPause() = 0;
	
	/// Sends new position/volume/pitch information to the hardware
	virtual bool Update() = 0;
	
	///
	virtual void SetPosition(const SbVec3 &avPos) = 0;
	
	///
	virtual void SetCenterChannel(float afValue) = 0;
	
	///
	virtual void SetGain(float afValue) = 0;
	
	///
	virtual void SetInnerRadius(float afValue) = 0;
	
	///
	virtual void SetPitch(float afValue) = 0;
	
	///
	virtual void SetOcclusion(float afValue) = 0;
	
	/// returns the RMS levels of the most recently processed block of audio, SSF_FLICKER must have been passed to Start
	virtual float GetAmplitude() const = 0;
	
	/// returns true if we can re-use this voice
	virtual bool CompatibleFormat(SbSoundSample *apSample) const = 0;
	
	///
	virtual uint32_t GetSampleRate() const = 0; // TODO: unused?
	
	/// callback function
	virtual void OnBufferStart(SbSoundSample *apSample, int anBuffer) = 0; // TODO: what's this for?
};

}; // namespace sbe::SbSound