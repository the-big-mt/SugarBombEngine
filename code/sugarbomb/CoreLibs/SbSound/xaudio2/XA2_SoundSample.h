/*
*******************************************************************************

Copyright (C) 1993-2012 id Software LLC, a ZeniMax Media company.
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

#pragma once

//#include <XAudio2.h>
//#include <XAudio2fx.h>

//#include "CoreLibs/SbSystem/IFileSystem.hpp"

#include "SbWaveFile.hpp"
//#include "SbSoundDefines.hpp"

namespace sbe::SbSound
{

/*
================================================
idSoundSample_XAudio2
================================================
*/
class idSampleInfo;
class SbSoundSample_XAudio2
{
public:
	SbSoundSample_XAudio2();

	// Loads and initializes the resource based on the name.
	virtual void LoadResource();

	void SetName(const char *n)
	{
		name = n;
	}
	const char *GetName() const
	{
		return name;
	}
	ID_TIME_T GetTimestamp() const
	{
		return timestamp;
	}

	// turns it into a beep
	void MakeDefault();

	// frees all data
	void FreeData();

	int LengthInMsec() const
	{
		return SamplesToMsec(NumSamples(), SampleRate());
	}
	int SampleRate() const
	{
		return format.basic.samplesPerSec;
	}
	int NumSamples() const
	{
		return playLength;
	}
	int NumChannels() const
	{
		return format.basic.numChannels;
	}
	int BufferSize() const
	{
		return totalBufferSize;
	}

	bool IsCompressed() const
	{
		return (format.basic.formatTag != SbWaveFile::FORMAT_PCM);
	}

	bool IsDefault() const
	{
		return timestamp == FILE_NOT_FOUND_TIMESTAMP;
	}
	bool IsLoaded() const
	{
		return loaded;
	}

	void SetNeverPurge()
	{
		neverPurge = true;
	}
	bool GetNeverPurge() const
	{
		return neverPurge;
	}

	void SetLevelLoadReferenced()
	{
		levelLoadReferenced = true;
	}
	void ResetLevelLoadReferenced()
	{
		levelLoadReferenced = false;
	}
	bool GetLevelLoadReferenced() const
	{
		return levelLoadReferenced;
	}

	int GetLastPlayedTime() const
	{
		return lastPlayedTime;
	}
	void SetLastPlayedTime(int t)
	{
		lastPlayedTime = t;
	}

	float GetAmplitude(int timeMS) const;

protected:
	friend class SbSoundHardware_XAudio2;
	friend class SbSoundVoice_XAudio2;

	~SbSoundSample_XAudio2();

	bool LoadWav(const idStr &name);
	bool LoadAmplitude(const idStr &name);
	void WriteAllSamples(const idStr &sampleName);
	bool LoadGeneratedSample(const idStr &name);
	void WriteGeneratedSample(idFile *fileOut);

	struct sampleBuffer_t
	{
		void *buffer;
		int bufferSize;
		int numSamples;
	};

	idStr name;

	ID_TIME_T timestamp;
	bool loaded;

	bool neverPurge;
	bool levelLoadReferenced;
	bool usesMapHeap;

	uint32 lastPlayedTime;

	int totalBufferSize; // total size of all the buffers
	idList<sampleBuffer_t, TAG_AUDIO> buffers;

	int playBegin;
	int playLength;

	SbWaveFile::waveFmt_t format;

	idList<byte, TAG_AMPLITUDE> amplitude;
};

/*
================================================
idSoundSample

This reverse-inheritance purportedly makes working on
multiple platforms easier.
================================================
*/
class SbSoundSample : public SbSoundSample_XAudio2
{
public:
};

}; // namespace sbe::SbSound